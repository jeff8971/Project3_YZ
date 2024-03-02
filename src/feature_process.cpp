/**
 * @file clean_process.cpp
 * @author Yuan Zhao zhao.yuan2@northeatern.edu
 * @brief Task 2 main entry, clean the binary video
 * @version 0.1
 * @date 2024-02-17
*/


#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <map>
#include "thresholding.h"
#include "morphological_filtering.h"
#include "segment.h"
#include "feature_extraction.h"

int main(int argc, char** argv) {
    // Initialize video capture with the external camera
    cv::VideoCapture cap(1); // This project using iphone 15 as the camera
    if (!cap.isOpened()) {
        std::cerr << "Error opening video stream or file" << std::endl;
        return EXIT_FAILURE;
    }

    int frameCounter = 0; // Counter to name the saved images uniquely

    std::map<int, RegionInfo> prevRegions;

    for (;;) {
        cv::Mat rawFrame, thresholdFrame, cleanedFrame, segmentFrame;
        cap >> rawFrame;
        if (rawFrame.empty()) {
            std::cerr << "Error: rawFrame is empty" << std::endl;
            break;
        }

        // Apply dynamic thresholding to the raw frame
        applyDynamicThreshold(rawFrame, thresholdFrame);
        // Clean the thresholded image using morphological filtering
        cleanThresholdedImage(thresholdFrame, cleanedFrame);

        cv::Mat labels = segmentObjects(cleanedFrame, segmentFrame, 500, prevRegions); // Adjust minRegionSize as needed

        // Inside the loop after computing features
for (const auto& reg : prevRegions) {
    RegionInfo info = computeFeatures(segmentFrame, labels, reg.first, reg.second.centroid, reg.second.color);

    // Display the additional features on the frame
    std::string infoText = "Fill: " + std::to_string(info.percentFilled) + 
                           ", Aspect: " + std::to_string(info.bboxAspectRatio);
    cv::putText(segmentFrame, infoText, cv::Point(info.centroid.x + 10, info.centroid.y + 10),
                cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255, 255, 255), 1);
}





        // Display the original and processed video frames
        cv::imshow("Original Video", rawFrame);
        cv::imshow("Segmented Video", segmentFrame);





        char key = cv::waitKey(100);
        if (key == 'q') {
            break;  // Exit the loop if 'q' is pressed
        } else if (key == 's') {
            // Save the displayed images when 's' is pressed
            std::string rawFilename = "rawSegmentFrame_" + std::to_string(frameCounter) + ".jpg";
            std::string segmentFilename = "featureProcessFrame_" + std::to_string(frameCounter) + ".jpg";
            
            // Save images
            if (!cv::imwrite(rawFilename, rawFrame)) {
                std::cerr << "Failed to save " << rawFilename << std::endl;
            } else {
                std::cout << "Saved " << rawFilename << std::endl;
            }

            if (!cv::imwrite(segmentFilename, segmentFrame)) {
                std::cerr << "Failed to save " << segmentFilename << std::endl;
            } else {
                std::cout << "Saved " << segmentFilename << std::endl;
            }

            frameCounter++; // Increment the counter to ensure filenames are unique
        }
    }

    cv::destroyAllWindows();
    return 0;
}
