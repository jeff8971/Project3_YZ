#ifndef MORPHOLOGICAL_FILTERING_H
#define MORPHOLOGICAL_FILTERING_H

#include <opencv2/core/mat.hpp>

// Self define erode and dilate function
void customErode(const cv::Mat& inputImage, cv::Mat& outputImage, const cv::Mat& erosionKernel);
// Self define dilate function
void customDilate(const cv::Mat& inputImage, cv::Mat& outputImage, const cv::Mat& dilationKernel);
void cleanThresholdedImage(const cv::Mat& src, cv::Mat& dst, int erosionIterations = 2, int dilationIterations = 2);





#endif // MORPHOLOGICAL_FILTERING_H