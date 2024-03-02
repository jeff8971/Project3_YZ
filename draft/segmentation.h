/**
 * @file segmentation.h
 * @author Yuan Zhao zhao.yuan2@northeatern.edu
 * @brief header file for segmentation.cpp
 * @version 0.1
 * @date 2024-02-23
*/


#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <opencv2/opencv.hpp>
#include <iostream>

// check if the image is binary
bool isBinaryImg(const cv::Mat& src);
// Function to generate a fixed color based on the label
cv::Vec3b generateFixedColor(int label);
// function to tag connected binary filtered regions 
cv::Mat tagConnectedRegion(const cv::Mat& src, int thresholdSize, cv::Mat& regionStats, cv::Mat& regionCentroids);
// Function to visualize tagged regions with different colors
cv::Mat visualizeTaggedRegions(const cv::Mat& regionLabels, const cv::Mat& regionCentroids);

#endif // SEGMENTATION_H