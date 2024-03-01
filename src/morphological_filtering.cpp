#include <opencv2/opencv.hpp>

// Self define erode and dilate function
void customErode(const cv::Mat& inputImage, cv::Mat& outputImage, const cv::Mat& erosionKernel) {
    outputImage = inputImage.clone();
    int kernelCenterX = erosionKernel.rows / 2;
    int kernelCenterY = erosionKernel.cols / 2;

    for (int i = 0; i < inputImage.rows; i++) {
        for (int j = 0; j < inputImage.cols; j++) {
            bool shouldErode = true;
            for (int ki = -kernelCenterX; ki <= kernelCenterX && shouldErode; ki++) {
                for (int kj = -kernelCenterY; kj <= kernelCenterY; kj++) {
                    int pixelX = i + ki;
                    int pixelY = j + kj;
                    if (pixelX >= 0 && pixelX < inputImage.rows && pixelY >= 0 && pixelY < inputImage.cols) {
                        if (erosionKernel.at<uchar>(ki + kernelCenterX, kj + kernelCenterY) == 1 && inputImage.at<uchar>(pixelX, pixelY) == 0) {
                            shouldErode = false;
                            break;
                        }
                    }
                }
            }
            outputImage.at<uchar>(i, j) = shouldErode ? 255 : 0;
        }
    }
}

// Self define dilate function
void customDilate(const cv::Mat& inputImage, cv::Mat& outputImage, const cv::Mat& dilationKernel) {
    outputImage = inputImage.clone();
    int kernelCenterX = dilationKernel.rows / 2;
    int kernelCenterY = dilationKernel.cols / 2;

    for (int i = 0; i < inputImage.rows; i++) {
        for (int j = 0; j < inputImage.cols; j++) {
            bool shouldDilate = false;
            for (int ki = -kernelCenterX; ki <= kernelCenterX && !shouldDilate; ki++) {
                for (int kj = -kernelCenterY; kj <= kernelCenterY; kj++) {
                    int pixelX = i + ki;
                    int pixelY = j + kj;
                    if (pixelX >= 0 && pixelX < inputImage.rows && pixelY >= 0 && pixelY < inputImage.cols) {
                        if (dilationKernel.at<uchar>(ki + kernelCenterX, kj + kernelCenterY) == 1 && inputImage.at<uchar>(pixelX, pixelY) == 255) {
                            shouldDilate = true;
                            break;
                        }
                    }
                }
            }
            outputImage.at<uchar>(i, j) = shouldDilate ? 255 : 0;
        }
    }
}

// Use the custom erode and dilate function to clean the thresholded image
void cleanThresholdedImage(const cv::Mat& src, cv::Mat& dst) {
    // Define a 3x3 kernel
    cv::Mat morphKernel = cv::Mat::ones(3, 3, CV_8U); // 3x3 kernel, could be changed to other size

    cv::Mat tempImage;
    customErode(src, tempImage, morphKernel); // erode first
    customDilate(tempImage, dst, morphKernel); // then dilate
}
