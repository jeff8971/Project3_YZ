/**
 * @file feature_extraction.h
 * @author Yuan Zhao zhao.yuan2@northeatern.edu
 * @brief header file for the feature_extraction.cpp
 * @version 0.1
 * @date 2024-02-17
*/


#ifndef FEATURE_EXTRACTION_H
#define FEATURE_EXTRACTION_H


#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include "segment.h"

// Function to compute features for a region
RegionInfo computeFeatures(cv::Mat &src, const cv::Mat &labels, int label, const cv::Point2d &centroid, const cv::Vec3b &color);

#endif // FEATURE_EXTRACTION_H