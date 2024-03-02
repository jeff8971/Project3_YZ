


#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include "thresholding.h"
#include "morphological_filtering.h"
#include "segment.h"
#include "feature_extraction.h"



int append_image_data_csv(char *csv_file_name, std::string object_name, std::vector<float> &image_data, int reset_file)
{
  char buffer[256];
  char mode[8];

  FILE *fp;

  std::strcpy(mode, "a");

  if (reset_file)
  {
    std::strcpy(mode, "w");
  }

  fp = fopen(csv_file_name, mode);

  if (!fp)
  {
    // printf("Unable to open output file %s\n", argv[2] );
    std::cout << "Unable to open CSV" << std::endl;
    exit(-1);
  }

  // write the filename and the feature vector to the CSV file
  // std::cout << image_filename << std::endl;
  std::strcpy(buffer, object_name.c_str());
  std::fwrite(buffer, sizeof(char), strlen(buffer), fp);
  for (int i = 0; i < image_data.size(); i++)
  {
    char tmp[256];
    sprintf(tmp, ",%.4f", image_data[i]);
    std::fwrite(tmp, sizeof(char), strlen(tmp), fp);
  }

  std::fwrite("\n", sizeof(char), 1, fp); // EOL

  fclose(fp);
  return (0);
}


int main(int argc, char** argv) {
    // Initialize video capture with the external camera
    cv::VideoCapture cap(1); // This project using iphone 15 as the camera
    if (!cap.isOpened()) {
        std::cerr << "Error opening video stream or file" << std::endl;
        return EXIT_FAILURE;
    }

    int frameCounter = 0; // Counter to name the saved images uniquely

    std::map<int, RegionInfo> prevRegions;
    std::string csvFileName = "objectDB.csv"; // 特征向量数据库文件名

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
        } else if (key == 'n' || key == 'N') {
            std::string objectLabel;
            std::cout << "Enter label for the current object: ";
            std::cin >> objectLabel; // 用户输入标签

            for (const auto& reg : prevRegions) {
                RegionInfo info = computeFeatures(segmentFrame, labels, reg.first, reg.second.centroid, reg.second.color);
                std::vector<float> featureVector;
                featureVector.push_back(info.percentFilled);
                featureVector.push_back(info.bboxAspectRatio);
                for (int i = 0; i < 7; ++i) {
                    featureVector.push_back(info.huMoments[i]);
                }
                append_image_data_csv((char*)csvFileName.c_str(), objectLabel, featureVector, 0);
            }
            std::cout << "Feature vector saved for label: " << objectLabel << std::endl;
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
