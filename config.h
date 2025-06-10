#ifndef CONFIG_H
#define CONFIG_H

#include <opencv2/opencv.hpp> // Include OpenCV for cv::Scalar
#include <cmath>

using namespace cv;

// Threshold values
const int THRESHOLD_VALUE = 165; // Intensity of color that will be included in the region, the higher the stricter
const int MIN_CONTOUR_AREA = 500;

// Cosine threshold for square detection
const double COSINE_THRESHOLD = 0.12; // Bigger means more forgiving vertex angles

// Circle properties
const int CIRCLE_RADIUS = 10;
const cv::Scalar CIRCLE_COLOR_1(0, 0, 255); // Red color
const cv::Scalar CIRCLE_COLOR_2(255, 0, 0); // 

// Square outline properties
const cv::Scalar SQUARE_COLOR(255, 0, 0); // Blue color
const int SQUARE_THICKNESS = 3;

// Plane physical properties
const double SPEED = 0.7;    // m/s
const double ALTITUDE = 1.15; // m
const double GRAVITY = 9.81; // m/s^2
const double RELEASE_TIME = 0; // s
const double AIR_RESISTANCE = 0; // distance change due to air resistance (m)
const double AOV = 120; //degrees

// Global payload release status
extern bool payloadReleaseStatus;


#endif // CONFIG_H