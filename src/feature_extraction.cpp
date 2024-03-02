#include "feature_extraction.h"
#include <opencv2/imgproc.hpp>
#include <vector>
#include <iostream>



RegionInfo computeFeatures(cv::Mat &src, const cv::Mat &labels, int label, const cv::Point2d &centroid, const cv::Vec3b &color) {
    RegionInfo info;
    info.centroid = centroid;
    info.color = color;

    // Create mask for the specified label
    cv::Mat mask = cv::Mat::zeros(labels.size(), CV_8U);
    for (int y = 0; y < labels.rows; ++y) {
        for (int x = 0; x < labels.cols; ++x) {
            if (labels.at<int>(y, x) == label) {
                mask.at<uchar>(y, x) = 255;
            }
        }
    }

    // Calculate moments of the mask
    cv::Moments m = cv::moments(mask, true);
    cv::HuMoments(m, info.huMoments); // Calculate Hu Moments

    // Calculate percent filled and aspect ratio
    std::vector<cv::Point> points;
    cv::findNonZero(mask, points);
    cv::RotatedRect rotRect = cv::minAreaRect(points);
    double area = cv::contourArea(points);
    double bboxArea = rotRect.size.width * rotRect.size.height;
    info.percentFilled = static_cast<float>(area / bboxArea);
    info.bboxAspectRatio = static_cast<float>(rotRect.size.width / rotRect.size.height);

    // Draw rectangle around the region
    cv::Point2f rectPoints[4];
    rotRect.points(rectPoints);
    for (int j = 0; j < 4; j++) {
        cv::line(src, rectPoints[j], rectPoints[(j+1)%4], cv::Scalar(color), 2);
    }

    // Draw the centroid
    cv::circle(src, centroid, 5, cv::Scalar(color), -1);

    // Optionally, draw a line indicating the direction of the major axis
    double angle = atan2(2 * m.mu11, m.mu20 - m.mu02) / 2;
    cv::Point2f endpoint(centroid.x + cos(angle) * 100, centroid.y - sin(angle) * 100); // Adjusted for correct orientation
    cv::line(src, centroid, endpoint, cv::Scalar(color), 2);

    return info;
}
