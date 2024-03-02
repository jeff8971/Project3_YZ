/**
 * @file segmentation.cpp
 * @author Yuan Zhao zhao.yuan2@northeatern.edu
 * @brief segment the binary, connected, filtered images into different regions
 * @version 0.1
 * @date 2024-02-23
*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <map>
#include "segmentation.h"

// Global variables to maintain color and centroid information across frames
std::map<int, cv::Vec3b> regionColors;
std::map<int, cv::Point2d> lastFrameCentroids;

// Function to check if the input image is binary
bool isBinaryImg(const cv::Mat& src) {
    if (src.channels() != 1) {
        std::cerr << "Error: Image is not single-channel\n";
        return false;
    }
    double minVal, maxVal;
    cv::minMaxLoc(src, &minVal, &maxVal);
    return (minVal == 0 && maxVal == 255);
}

// Function to generate a fixed color for a given label
cv::Vec3b generateFixedColor(int label) {
    unsigned char r = (label * 40) % 255;
    unsigned char g = (label * 70) % 255;
    unsigned char b = (label * 90) % 255;
    return cv::Vec3b(b, g, r);
}

// Function to tag connected regions in a binary image and filter by size
cv::Mat tagConnectedRegion(const cv::Mat& src, int thresholdSize, cv::Mat& regionStats, cv::Mat& regionCentroids) {
    cv::Mat regionLabels, negatedImage;

    if (src.empty()) {
        std::cerr << "Error: The source image is empty!" << std::endl;
        return regionLabels;
    }

    if (!isBinaryImg(src)) {
        std::cerr << "Error: The source image is not binary." << std::endl;
        return regionLabels;
    }

    // Invert the binary image for connected components analysis
    cv::bitwise_not(src, negatedImage);
    int numberOfLabels = cv::connectedComponentsWithStats(negatedImage, regionLabels, regionStats, regionCentroids);

    // Filter out small regions
    for (int i = 1; i < numberOfLabels; i++) {
        if (regionStats.at<int>(i, cv::CC_STAT_AREA) < thresholdSize) {
            for (int row = 0; row < src.rows; row++) {
                for (int col = 0; col < src.cols; col++) {
                    if (regionLabels.at<int>(row, col) == i) {
                        regionLabels.at<int>(row, col) = 0; // Set to background
                    }
                }
            }
        }
    }

    return regionLabels;
}

// Function to visualize tagged regions with consistent colors across frames
cv::Mat visualizeTaggedRegions(const cv::Mat& regionLabels, const cv::Mat& regionCentroids) {
    std::map<int, cv::Vec3b> newRegionColors;
    cv::Mat coloredRegion = cv::Mat(regionLabels.size(), CV_8UC3);

    // Update colors based on matching centroids to last frame
    for (int i = 0; i < regionCentroids.rows; i++) {
        double x = regionCentroids.at<double>(i, 0);
        double y = regionCentroids.at<double>(i, 1);
        int label = i + 1; // Labels are 1-indexed in connectedComponentsWithStats

        // Placeholder for finding the closest centroid in the last frame
        // int matchedLabel = findClosestCentroid(lastFrameCentroids, cv::Point2d(x, y));
        // Assume findClosestCentroid function exists and returns the matched label or -1 if no close match

        // For simplicity, we're directly assigning new colors. Implement findClosestCentroid as needed.
        newRegionColors[label] = generateFixedColor(label);
        
        // Update lastFrameCentroids with current frame's centroids
        lastFrameCentroids[label] = cv::Point2d(x, y);
    }

    // Assign colors to regions
    for (int row = 0; row < regionLabels.rows; row++) {
        for (int col = 0; col < regionLabels.cols; col++) {
            int label = regionLabels.at<int>(row, col);
            coloredRegion.at<cv::Vec3b>(row, col) = newRegionColors[label];
        }
    }

    // Update global color mapping
    regionColors = newRegionColors;
    return coloredRegion;
}
