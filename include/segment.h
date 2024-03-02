/**
 * Ronak Bhanushali and Ruohe Zhou
 * Spring 2024
 * @file filters.hpp
 * @brief This file contains functions for image processing and object segmentation.
 */

#ifndef INCLUDE_SEGMENT_H
#define INCLUDE_SEGMENT_H




#include <opencv2/opencv.hpp>
#include <stdio.h>
#include<iostream>

/**
 * @brief Struct to store information about a segmented region.
 */
// Structure to hold region features
struct RegionInfo {
    cv::Point2d centroid; // Centroid of the region
    cv::Vec3b color; // Color of the region
    float percentFilled; // Percentage of the region filled
    float bboxAspectRatio; // Aspect ratio of the bounding box
    double huMoments[7]; // Hu moments for shape analysis
};

/**
 * @brief Segments objects in the input image and returns the segmented image.
 * @param src Input image.
 * @param dst Output segmented image.
 * @param minRegionSize Minimum size of a region to be considered an object.
 * @param prevRegions Map containing information about previously segmented regions.
 * @return Returns the segmented image.
 */
cv::Mat segmentObjects(cv::Mat &src, cv::Mat &dst, int minRegionSize, std::map<int, RegionInfo>& prevRegions);

/**
 * @brief Retrieves the color for a segmented region based on its centroid.
 * @param centroid Centroid of the region.
 * @param prevRegions Map containing information about previously segmented regions.
 * @return Returns the color for the region.
 */
cv::Vec3b getColorForRegion(cv::Point2d centroid, std::map<int, RegionInfo>& prevRegions);

#endif  // INCLUDE_SEGMENT_H  