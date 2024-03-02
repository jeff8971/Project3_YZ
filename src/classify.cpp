
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <map>
#include "thresholding.h"
#include "morphological_filtering.h"
#include "segment.h"
#include "feature_extraction.h"


int getstring(FILE *fp, char os[]){
  int p = 0;
  int eol = 0;

  for (;;){
    char ch = fgetc(fp);
    if (ch == ','){
      break;
    }
    else if (ch == '\n' || ch == EOF){
      eol = 1;
      break;
    }
    // printf("%c", ch ); // uncomment for debugging
    os[p] = ch;
    p++;
  }
  // printf("\n"); // uncomment for debugging
  os[p] = '\0';

  return (eol); // return true if eol
}

int getint(FILE *fp, int *v){
  char s[256];
  int p = 0;
  int eol = 0;

  for (;;){
    char ch = fgetc(fp);
    if (ch == ','){
      break;
    }
    else if (ch == '\n' || ch == EOF){
      eol = 1;
      break;
    }

    s[p] = ch;
    p++;
  }
  s[p] = '\0'; // terminator
  *v = atoi(s);

  return (eol); // return true if eol
}

/*
  Utility function for reading one float value from a CSV file

  The value is stored in the v parameter

  The function returns true if it reaches the end of a line or the file
 */
int getfloat(FILE *fp, float *v){
  char s[256];
  int p = 0;
  int eol = 0;

  for (;;){
    char ch = fgetc(fp);
    if (ch == ','){
      break;
    }
    else if (ch == '\n' || ch == EOF){
      eol = 1;
      break;
    }

    s[p] = ch;
    p++;
  }
  s[p] = '\0'; // terminator
  *v = atof(s);

  return (eol); // return true if eol
}

int read_image_data_csv(char *filename, std::vector<char *> &labels, std::vector<std::vector<float>> &data, int echo_file){
  FILE *fp;
  float fval;
  char img_file[256];

  fp = fopen(filename, "r");
  if (!fp)
  {
    printf("Unable to open feature file\n");
    return (-1);
  }

  printf("Reading %s\n", filename);
  for (;;){
    std::vector<float> dvec;

    // read the filename
    if (getstring(fp, img_file)){
      break;
    }

    // read the whole feature file into memory
    for (;;){
      // get next feature
      float eol = getfloat(fp, &fval);
      dvec.push_back(fval);
      if (eol)
        break;
    }

    data.push_back(dvec);

    char *fname = new char[strlen(img_file) + 1];
    strcpy(fname, img_file);
    labels.push_back(fname);
  }
  fclose(fp);
  printf("Finished reading CSV file\n");

  if (echo_file){
    for (int i = 0; i < data.size(); i++) {
      for (int j = 0; j < data[i].size(); j++){
        printf("%.4f  ", data[i][j]);
      }
      printf("\n");
    }
    printf("\n");
  }

  return (0);
}

std::vector<std::pair<float, std::string>> calculate_scaled_euclidean_distances(const std::vector<char *>& labels, const std::vector<std::vector<float>>& known_data, const std::vector<float>& new_value) {
    std::vector<std::pair<float, std::string>> scaled_distances;

    for (size_t i = 0; i < labels.size(); ++i) {
        const std::vector<float>& data_for_label = known_data[i];

        float mean = 0;
        float std_dev = 0;

        for (float value : data_for_label) {
            mean += value;
        }
        mean /= data_for_label.size();

        for (float value : data_for_label) {
            float diff = value - mean;
            std_dev += diff * diff;
        }
        std_dev = sqrt(std_dev / data_for_label.size());

        float scaled_diff = (new_value[i] - mean) / std_dev;

        float distance = sqrt(scaled_diff * scaled_diff);

        scaled_distances.push_back(std::make_pair(distance, labels[i]));
    }

    return scaled_distances;
}

std::vector<std::pair<float, std::string>> calculate_euclidean_distances(const std::vector<char *> &labels, const std::vector<std::vector<float>> &known_data, const std::vector<float> &new_value)
{
    std::vector<std::pair<float, std::string>> distances;

    for (size_t i = 0; i < labels.size(); ++i)
    {
        const std::vector<float> &data_for_label = known_data[i];
        size_t num_features = data_for_label.size();
        float temp = 0;

        for (int j = 0; j < num_features; j++)
        {
            // calculating SSD
            float diff = data_for_label[j] - new_value[j];
            temp += diff * diff;
        }
        // taking square root
        float distance = std::sqrt(temp);

        distances.push_back(std::make_pair(distance, labels[i]));
    }

    return distances;
}

// Function to compare the feature vector of the target image with the feature vectors in the CSV file
int compareFeatures(std::vector<float> targetVector, char* csvFileName)
{
  std::vector<char *> labels;
  std::vector<std::vector<float>> data;
  int echo = 0;
  std::vector<std::pair<float, std::string>> image_ranks; // defining a vector pair (float,string)

  // read the csv file
  int result = read_image_data_csv(csvFileName, labels, data, echo);

  if (result == 0)
  {
    image_ranks = calculate_euclidean_distances(labels,data,targetVector);
    // image_ranks = calculate_scaled_euclidean_distances(labels,data,targetVector);
    // sorting the vector pair in ascending order of the float values
    sort(image_ranks.begin(), image_ranks.end());

    std::cout<<image_ranks[0].second<<","<<image_ranks[0].first<<std::endl;
    std::cout<<image_ranks[1].second<<","<<image_ranks[1].first<<std::endl;
    std::cout<<image_ranks[2].second<<","<<image_ranks[2].first<<std::endl;
    // Free allocated memory in the filenames vector
    for (char *fname : labels)
    {
      delete[] fname;
    }
  }

  else
  {
    std::cerr << "Error reading CSV file.\n";
  }
  printf("Terminating second program\n");
  return (0);
}



int main(int argc, char** argv) {
    cv::VideoCapture cap(1);
    if (!cap.isOpened()) {
        std::cerr << "Error opening video stream or file" << std::endl;
        return -1;
    }

    std::string csvFileName = "/Users/jeff/Desktop/Project3_YZ/bin/objectDB.csv"; 
    // set up the video recorder
    int width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));

    // prepare the video writer
    cv::VideoWriter writer;
    bool isRecording = false;

    int frameCounter = 0; // Counter to name the saved images uniquely

    std::map<int, RegionInfo> prevRegions;

    std::vector<char *> labels;
    std::vector<std::vector<float>> data;
    read_image_data_csv((char*)csvFileName.c_str(), labels, data, 0); 

    while (true) {
        cv::Mat rawFrame, thresholdFrame, cleanedFrame, segmentFrame;
        cap >> rawFrame;
        if (rawFrame.empty()) break;

        // process the frame
        applyDynamicThreshold(rawFrame, thresholdFrame);
        cleanThresholdedImage(thresholdFrame, cleanedFrame);
        cv::Mat labelsMat = segmentObjects(cleanedFrame, segmentFrame, 500, prevRegions);

        // process the regions
        for (const auto& reg : prevRegions) {
            RegionInfo info = computeFeatures(segmentFrame, labelsMat, reg.first, reg.second.centroid, reg.second.color);
            std::vector<float> featureVector {info.percentFilled, info.bboxAspectRatio};
            for (int i = 0; i < 7; ++i) {
                featureVector.push_back(info.huMoments[i]);
            }

            auto distances = calculate_euclidean_distances(labels, data, featureVector);
            std::sort(distances.begin(), distances.end());
            std::string objectLabel = distances.front().second; // 最相似的对象标签

            // show the id and label of the object
            std::string idText = "ID: " + std::to_string(reg.first) + ", Label: " + objectLabel;
            cv::putText(segmentFrame, idText, cv::Point(info.centroid.x - 10, info.centroid.y - 10),
                        cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
        }

        // show the video stream
        cv::imshow("Original Video", rawFrame);
        cv::imshow("Segmented Video", segmentFrame);

        char key = cv::waitKey(100);
        if (key == 's') {
            // Save the displayed images when 's' is pressed
            std::string rawFilename = "rawClassSegmentFrame_" + std::to_string(frameCounter) + ".jpg";
            std::string segmentFilename = "classFrame_" + std::to_string(frameCounter) + ".jpg";
            
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
        } else if (key == 'r' && !isRecording) {
            // start recording
            writer.open("recordedVideo.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, cv::Size(width, height));
            if (!writer.isOpened()) {
                std::cerr << "Could not open the output video file for write\n";
                return -1;
            }
            isRecording = true;
        } else if (key == 'e' && isRecording) {
            // end recording
            writer.release();
            isRecording = false;
        } else if (key == 'q') {
            break;  
        }

        if (isRecording) {
            writer.write(segmentFrame); 
        }
    }

    cv::destroyAllWindows();
    return 0;
}