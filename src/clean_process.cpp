/**
 * @file clean_process.cpp
 * @author Yuan Zhao zhao.yuan2@northeatern.edu
 * @brief Task 2 main entry, clean the binary video
 * @version 0.1
 * @date 2024-02-17
*/


#include <opencv2/opencv.hpp>
#include <iostream>
#include "thresholding.h"
#include "morphological_filtering.h"

int main(int argc, char** argv) {
    // Initialize video capture with the external camera
    cv::VideoCapture cap(1); // This project using iphone 15 as the camera
    if (!cap.isOpened()) {
        std::cerr << "Error opening video stream or file" << std::endl;
        return EXIT_FAILURE;
    }

    int frameCounter = 0; // Counter to name the saved images uniquely

    for (;;) {
        cv::Mat rawFrame, thresholdFrame, cleanedFrame;
        cap >> rawFrame;
        if (rawFrame.empty()) {
            std::cerr << "Error: rawFrame is empty" << std::endl;
            break;
        }

        // Apply dynamic thresholding to the raw frame
        applyDynamicThreshold(rawFrame, thresholdFrame);
        // Clean the thresholded image using morphological filtering
        cleanThresholdedImage(thresholdFrame, cleanedFrame);

        // Display the original and processed video frames
        cv::imshow("Original Video", rawFrame);
        cv::imshow("Threshold (Binary) Video", thresholdFrame);
        cv::imshow("Cleaned Video", cleanedFrame);

        char key = cv::waitKey(100);
        if (key == 'q') {
            break;  // Exit the loop if 'q' is pressed
        } else if (key == 's') {
            // Save the displayed images when 's' is pressed
            std::string rawFilename = "rawFrame_" + std::to_string(frameCounter) + ".jpg";
            std::string thresholdFilename = "thresholdFrame_" + std::to_string(frameCounter) + ".jpg";
            std::string cleanedFilename = "cleanedFrame_" + std::to_string(frameCounter) + ".jpg";

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
            if (!cv::imwrite(cleanedFilename, cleanedFrame)) {
                std::cerr << "Failed to save " << cleanedFilename << std::endl;
            } else {
                std::cout << "Saved " << cleanedFilename << std::endl;
            }

            frameCounter++; // Increment the counter to ensure filenames are unique
        }
    }

    cv::destroyAllWindows();
    return 0;
}
