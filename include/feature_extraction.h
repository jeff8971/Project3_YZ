#ifndef FEATURE_EXTRACTION_H
#define FEATURE_EXTRACTION_H


#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include "segment.h"





RegionInfo computeFeatures(cv::Mat &src, const cv::Mat &labels, int label, const cv::Point2d &centroid, const cv::Vec3b &color);


#endif // FEATURE_EXTRACTION_H