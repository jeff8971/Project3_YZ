/**
 * @file thresholding.cpp
 * @author Yuan Zhao zhao.yuan2@northeatern.edu
 * @brief threshold the images and videos
 * @version 0.1
 * @date 2024-02-17
*/


#include "thresholding.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream> // For error logging

/**
 * Utility function to check if the input image is valid for processing.
 * Validates the image by checking if it is empty, ensuring it is 8-bit,
 * and confirming it is either grayscale or BGR (3 channels).
 *
 * @param image The input image to validate.
 * @return true if the image is valid for processing, false otherwise.
 */
bool isValidImage(const cv::Mat& image) {
    if (image.empty()) {
        std::cerr << "Error: Image is empty\n";
        return false;
    }
    if (image.depth() != CV_8U) {
        std::cerr << "Error: Image is not 8-bit\n";
        return false;
    }
    if (image.channels() != 1 && image.channels() != 3) {
        std::cerr << "Error: Image is not grayscale or BGR\n";
        return false;
    }
    return true;
}

/**
 * Applies dynamic thresholding to an image using k-means clustering.
 * The process involves blurring the image to reduce noise, converting it to the L*a*b color space,
 * and then using k-means clustering to find the average brightness of the clusters.
 * This average brightness is then used as a threshold to binarize the image.
 *
 * @param src The source image to be thresholded. It should be a BGR image.
 * @param dst The destination image that will contain the thresholded result.
 */
void applyDynamicThreshold(const cv::Mat& src, cv::Mat& dst) {
    if (!isValidImage(src)) return; // Error handling

    // Use Gaussian blur to reduce noise in the image
    cv::Mat blurred;
    cv::GaussianBlur(src, blurred, cv::Size(5, 5), 0);

    // Convert the blurred image to the L*a*b color space for better color differentiation
    cv::Mat lab;
    cv::cvtColor(blurred, lab, cv::COLOR_BGR2Lab);

    // Flatten the image to a 1D array and convert to floating point for k-means
    cv::Mat data = lab.reshape(1, lab.total());
    data.convertTo(data, CV_32F);

    // Perform k-means clustering to find two dominant colors in the image
    const int K = 2;
    cv::Mat labels, centers;
    cv::kmeans(data, K, labels, cv::TermCriteria(cv::TermCriteria::EPS+cv::TermCriteria::MAX_ITER, 10, 1.0), 3, cv::KMEANS_PP_CENTERS, centers);

    // Calculate the average brightness of the clusters and use it as a threshold
    float avgBrightness = 0;
    for(int i = 0; i < K; i++) {
        avgBrightness += (centers.at<float>(i, 0) + centers.at<float>(i, 1) + centers.at<float>(i, 2)) / 3.0f;
    }
    avgBrightness /= K;

    // Apply the calculated threshold to binarize the image
    simpleThreshold(blurred, dst, static_cast<int>(avgBrightness));
}

/**
 * Applies a simple thresholding operation to an image.
 * Converts the source image to grayscale if it is not already, and then applies a fixed threshold
 * to binarize the image. Pixels with values above the threshold are set to 255 (white), and all others are set to 0 (black).
 *
 * @param src The source image, which can be a grayscale or BGR image.
 * @param dst The destination image that will contain the binary result.
 * @param thresholdValue The threshold value to use for binarization.
 */
void simpleThreshold(const cv::Mat& src, cv::Mat& dst, int thresholdValue) {
    if (!isValidImage(src)) return; // Error handling

    // Convert the source image to grayscale if it is a BGR image
    cv::Mat grayscale;
    if (src.channels() == 3) {
        cv::cvtColor(src, grayscale, cv::COLOR_BGR2GRAY);
    } else {
        grayscale = src;
    }
    
    // Apply the threshold to the grayscale image to obtain a binary image
    cv::threshold(grayscale, dst, thresholdValue, 255, cv::THRESH_BINARY);
}
