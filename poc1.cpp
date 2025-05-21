#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include "config.h"

using namespace cv;
using namespace std;

// Define global variables
bool payloadReleaseStatus = false; // Global payload release status

// Function to detect the most likely white square
void detectWhiteSquares(Mat& img) {
    Mat gray, thresh;
    cvtColor(img, gray, COLOR_BGR2GRAY);

    // Threshold to isolate white regions
    threshold(gray, thresh, THRESHOLD_VALUE, 255, THRESH_BINARY);

    // Find contours
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(thresh, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    vector<Point> bestSquare; // Store the most likely square
    double maxArea = 0;       // Track the largest area

    for (auto& contour : contours) {
        double area = contourArea(contour);
        if (area < MIN_CONTOUR_AREA) continue;  // Filter out noise

        vector<Point> approx;
        approxPolyDP(contour, approx, 0.02 * arcLength(contour, true), true);

        // Check for square (4 vertices and roughly square-shaped)
        if (approx.size() == 4 && isContourConvex(approx)) {
            bool isSquare = true;

            for (int i = 0; i < 4; i++) {
                Point2f p1 = approx[i];
                Point2f p2 = approx[(i + 1) % 4];
                Point2f p3 = approx[(i + 2) % 4];

                Point2f v1 = p1 - p2;
                Point2f v2 = p3 - p2;
                double cos_angle = abs(v1.dot(v2) / (norm(v1) * norm(v2)));

                if (cos_angle > COSINE_THRESHOLD) {
                    isSquare = false;
                    break;
                }
            }

            // If it's a square and has the largest area so far, update the best square
            if (isSquare && area > maxArea) {
                maxArea = area;
                bestSquare = approx;
            }
        }
    }

    // Highlight the most likely square
    if (!bestSquare.empty()) {
        polylines(img, bestSquare, true, SQUARE_COLOR, SQUARE_THICKNESS); // Draw blue outline for the best square
    }

    // Draw the red circle in the center of the frame AFTER the detection logic
    Point center(img.cols / 2, img.rows / 2);  // Center of the frame
    circle(img, center, CIRCLE_RADIUS, CIRCLE_COLOR, -1);  // Solid red circle

    // Display the word "HOLD" in red letters at the top-left corner
    if (!payloadReleaseStatus) {
        putText(img, "HOLD", Point(10, 30), FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
    }

    // Show the frame
    imshow("White Squares", img);
}

// Placeholder function for timePayloadRelease
bool releasePayload(Mat& img) {
    // Example logic to update payloadReleaseStatus
    payloadReleaseStatus = false; // Set to true when payload is released
    return payloadReleaseStatus;
}


int main() {
    string path = "resources/test_video2.mp4";
    VideoCapture cap(path);
    if (!cap.isOpened()) {
        cerr << "Error opening video file!" << endl;
        return -1;
    }

    Mat frame;
    while (cap.read(frame)) {
        detectWhiteSquares(frame);  // Detect white squares
        releasePayload(frame);     // Placeholder function

        // Wait for key press and check which key was pressed
        int key = waitKey(30);  // Wait 30ms for a key press (if no key is pressed, continue)

        if (key == 27) {  // ESC key: Exit
            break;
        }
        else if (key == 32) {  // Spacebar: Pause (keep showing the current frame)
            // Wait until a key is pressed to continue
            while (true) {
                int pauseKey = waitKey(0);  // Wait for any key to continue
                if (pauseKey != -1) break;  // Continue when any key is pressed
            }
        }
    }

    cap.release();
    destroyAllWindows();
    return 0;
}