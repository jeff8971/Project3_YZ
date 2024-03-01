/**
 * @file thresholding.h
 * @author Yuan Zhao zhao.yuan2@northeatern.edu
 * @brief header file for threshold.cpp
 * @version 0.1
 * @date 2024-02-17
*/

#ifndef THRESHOLDING_H
#define THRESHOLDING_H

#include <opencv2/core/mat.hpp>


/**
 * Applies dynamic thresholding to an image using k-means clustering.
 * The process involves blurring the image to reduce noise, converting it to the L*a*b color space,
 * and then using k-means clustering to find the average brightness of the clusters.
 * This average brightness is then used as a threshold to binarize the image.
 *
 * @param src The source image to be thresholded. It should be a BGR image.
 * @param dst The destination image that will contain the thresholded result.
 */
void applyDynamicThreshold(const cv::Mat& source, cv::Mat& destination);

/**
 * Applies a simple thresholding operation to an image.
 * Converts the source image to grayscale if it is not already, and then applies a fixed threshold
 * to binarize the image. Pixels with values above the threshold are set to 255 (white), and all others are set to 0 (black).
 *
 * @param src The source image, which can be a grayscale or BGR image.
 * @param dst The destination image that will contain the binary result.
 * @param thresholdValue The threshold value to use for binarization.
 */
void simpleThreshold(const cv::Mat& source, cv::Mat& destination, int thresholdValue);

#endif // THRESHOLDING_H
