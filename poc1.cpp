#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

// Function to detect white squares
void detectWhiteSquares(Mat& img) {
    Mat gray, thresh;
    cvtColor(img, gray, COLOR_BGR2GRAY);

    // Threshold to isolate white regions
    threshold(gray, thresh, 165, 255, THRESH_BINARY);

    // Find contours
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(thresh, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    for (auto& contour : contours) {
        double area = contourArea(contour);
        if (area < 500) continue;  // filter out noise

        vector<Point> approx;
        approxPolyDP(contour, approx, 0.02 * arcLength(contour, true), true);

        // Check for square (4 vertices and roughly square-shaped)
        if (approx.size() == 4 && isContourConvex(approx)) {
            double cos_thresh = 0.12;
            bool isSquare = true;

            for (int i = 0; i < 4; i++) {
                Point2f p1 = approx[i];
                Point2f p2 = approx[(i + 1) % 4];
                Point2f p3 = approx[(i + 2) % 4];

                Point2f v1 = p1 - p2;
                Point2f v2 = p3 - p2;
                double cos_angle = abs(v1.dot(v2) / (norm(v1) * norm(v2)));

                if (cos_angle > cos_thresh) {
                    isSquare = false;
                    break;
                }
            }

            if (isSquare) {
                polylines(img, approx, true, Scalar(255, 0, 0), 3); // Draw blue outline for square
            }
        }
    }

    // Draw the red circle in the center of the frame AFTER the detection logic
    Point center(img.cols / 2, img.rows / 2);  // Center of the frame
    int radius = 10;  // Radius of the circle
    Scalar color(0, 0, 255);  // Red color
    circle(img, center, radius, color, -1);  // Solid red circle
    imshow("White Squares", img);
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
        detectWhiteSquares(frame);  // Detect white squares iweiuqweiu

        // Wait for key press and check which key was pressed
        int key = waitKey(30);  // Wait 30ms for a key press (if no key is pressed, continue)

        if (key == 27) {  // ESC key: Exit
            break;
        }
        else if (key == 32) {  // Spacebar: Pause (keep showing the current frame)
            // Wait until a key is pressed to continue
            while (true) {
                int pauseKey = waitKey(0);  // Wait for any key to continue
                if (pauseKey != -1) break;  // Continue when any key is pressed //
            }
        }
    }

    cap.release();
    destroyAllWindows();
    return 0;
}
