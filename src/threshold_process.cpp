/**
 * @file threshold_process.cpp
 * @author Yuan Zhao zhao.yuan2@northeatern.edu
 * @brief Task 1 main entry, threshold the input video
 * @version 0.1
 * @date 2024-02-17
*/


#include <opencv2/opencv.hpp>
#include <iostream>
#include "thresholding.h"

int main(int argc, char** argv) {
    // Initialize video capture with the external camera
    cv::VideoCapture cap(1); // This project using iphone 15 as the camera
    if (!cap.isOpened()) {
        std::cerr << "Error opening video stream or file" << std::endl;
        return EXIT_FAILURE;
    }

    int frameCounter = 0; // Counter to name the saved images uniquely

    for (;;) {
        cv::Mat rawFrame, thresholdFrame;
        cap >> rawFrame;
        if (rawFrame.empty()) {
            std::cerr << "Error: rawFrame is empty" << std::endl;
            break;
        }

        // Apply dynamic thresholding to the raw frame
        applyDynamicThreshold(rawFrame, thresholdFrame);

        // Display the original and processed video frames
        cv::imshow("Original Video", rawFrame);
        cv::imshow("Threshold (Binary) Video", thresholdFrame);

        char key = cv::waitKey(10);
        if (key == 'q') {
            break;  // Exit the loop if 'q' is pressed
        } else if (key == 's') {
            // Save the displayed images when 's' is pressed
            std::string rawFilename = "rawFrame_" + std::to_string(frameCounter) + ".jpg";
            std::string thresholdFilename = "thresholdFrame_" + std::to_string(frameCounter) + ".jpg";
            
            // Save images
            if (!cv::imwrite(rawFilename, rawFrame)) {
                std::cerr << "Failed to save " << rawFilename << std::endl;
            } else {
                std::cout << "Saved " << rawFilename << std::endl;
            }
            if (!cv::imwrite(thresholdFilename, thresholdFrame)) {
                std::cerr << "Failed to save " << thresholdFilename << std::endl;
            } else {
                std::cout << "Saved " << thresholdFilename << std::endl;
            }

            frameCounter++; // Increment the counter to ensure filenames are unique
        }
    }

    cv::destroyAllWindows();
    return 0;
}
