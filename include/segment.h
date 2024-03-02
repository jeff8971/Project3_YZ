/**
 * @file segment.h
 * @author Yuan Zhao zhao.yuan2@northeatern.edu
 * @brief header file for the segment.cpp
 * @version 0.1
 * @date 2024-02-17
*/


#ifndef INCLUDE_SEGMENT_H
#define INCLUDE_SEGMENT_H

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>

// Structure to hold region features
struct RegionInfo {
    cv::Point2d centroid; // Centroid of the region
    cv::Vec3b color; // Color of the region
    float percentFilled; // Percentage of the region filled
    float bboxAspectRatio; // Aspect ratio of the bounding box
    double huMoments[7]; // Hu moments for shape analysis
};

// Function to segment objects in an image
cv::Mat segmentObjects(cv::Mat &src, cv::Mat &dst, int minRegionSize, std::map<int, RegionInfo>& prevRegions);
// Function to get color for a region based on its centroid
cv::Vec3b getColorForRegion(cv::Point2d centroid, std::map<int, RegionInfo>& prevRegions);

#endif  // INCLUDE_SEGMENT_H  