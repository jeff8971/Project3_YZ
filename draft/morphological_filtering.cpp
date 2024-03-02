#include <opencv2/opencv.hpp>

// Adjusted custom erode function
void customErode(const cv::Mat& inputImage, cv::Mat& outputImage, const cv::Mat& erosionKernel, int iterations = 1) {
    outputImage = inputImage.clone();
    int kernelCenterX = erosionKernel.rows / 2;
    int kernelCenterY = erosionKernel.cols / 2;

    for (int iter = 0; iter < iterations; iter++) {
        cv::Mat tempImage = outputImage.clone();
        for (int i = 0; i < inputImage.rows; i++) {
            for (int j = 0; j < inputImage.cols; j++) {
                bool shouldErode = true;
                for (int ki = -kernelCenterX; ki <= kernelCenterX && shouldErode; ki++) {
                    for (int kj = -kernelCenterY; kj <= kernelCenterY; kj++) {
                        int pixelX = i + ki;
                        int pixelY = j + kj;
                        if (pixelX >= 0 && pixelX < inputImage.rows && pixelY >= 0 && pixelY < inputImage.cols) {
                            if (erosionKernel.at<uchar>(ki + kernelCenterX, kj + kernelCenterY) == 1 && tempImage.at<uchar>(pixelX, pixelY) == 0) {
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
}

// Adjusted custom dilate function
void customDilate(const cv::Mat& inputImage, cv::Mat& outputImage, const cv::Mat& dilationKernel, int iterations = 1) {
    outputImage = inputImage.clone();
    int kernelCenterX = dilationKernel.rows / 2;
    int kernelCenterY = dilationKernel.cols / 2;

    for (int iter = 0; iter < iterations; iter++) {
        cv::Mat tempImage = outputImage.clone();
        for (int i = 0; i < inputImage.rows; i++) {
            for (int j = 0; j < inputImage.cols; j++) {
                bool shouldDilate = false;
                for (int ki = -kernelCenterX; ki <= kernelCenterX && !shouldDilate; ki++) {
                    for (int kj = -kernelCenterY; kj <= kernelCenterY; kj++) {
                        int pixelX = i + ki;
                        int pixelY = j + kj;
                        if (pixelX >= 0 && pixelX < inputImage.rows && pixelY >= 0 && pixelY < inputImage.cols) {
                            if (dilationKernel.at<uchar>(ki + kernelCenterX, kj + kernelCenterY) == 1 && tempImage.at<uchar>(pixelX, pixelY) == 255) {
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
}

// Adjusted function to clean the thresholded image
void cleanThresholdedImage(const cv::Mat& src, cv::Mat& dst, int erosionIterations = 2, int dilationIterations = 2) {
    // Define a larger kernel for more obvious effects
    cv::Mat morphKernel = cv::Mat::ones(5, 5, CV_8U); // 5x5 kernel for more pronounced effect
    cv::Mat tempImage;
    
    customErode(src, tempImage, morphKernel, erosionIterations); // Erode multiple times
    customDilate(tempImage, dst, morphKernel, dilationIterations); // Dilate multiple times


}
