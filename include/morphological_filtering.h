#ifndef MORPHOLOGICAL_FILTERING_H
#define MORPHOLOGICAL_FILTERING_H

#include <opencv2/core/mat.hpp>

// Self define erode and dilate function
void customErode(const cv::Mat& inputImage, cv::Mat& outputImage, const cv::Mat& erosionKernel);
// Self define dilate function
void customDilate(const cv::Mat& inputImage, cv::Mat& outputImage, const cv::Mat& dilationKernel);
// Use the custom erode and dilate function to clean the thresholded image
void cleanThresholdedImage(const cv::Mat& src, cv::Mat& dst);





#endif // MORPHOLOGICAL_FILTERING_H