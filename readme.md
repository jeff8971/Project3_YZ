# Project3_YZ: Project 3: Real-time 2-D Object Recognition 

[Project Repository](https://github.com/jeff8971/Project3_YZ)

[DEMO](https://drive.google.com/file/d/1jM_PgVTPZHYQJVXBxExOj3yt8gHMWUUf/view?usp=drive_link)

## Overview
Project3_YZ is developed as part of the CS5330 Computer Vision course, focusing on Real-time 2-D Object Recognition. This project aims to implement a system capable of recognizing and classifying objects from a video stream in real-time. It involves processes such as dynamic thresholding, morphological filtering, segmentation, feature extraction, and classification.



## System Environment
- **IDE**: Visual Studio Code or any preferred C++ IDE
- **Compiler**: C++ Compiler supporting C++11 standard
- **System Configuration**: Compatible with various operating systems, including macOS, Linux, and Windows.
- **Dependencies**: Requires OpenCV for image processing and feature extraction functionalities.

## Project Structure
- `src/`: Source files implementing the core functionality of the project.
  - `classify.cpp`: Main entry for object classification.
  - `clean_process.cpp`: Implements the cleaning process of the binary video.
  - `feature_extraction.cpp`: Extracts features from segmented objects.
  - `feature_process.cpp`: Processes features for classification.
  - `morphological_filtering.cpp`: Applies morphological operations to clean images.
  - `save_feature.cpp`: Saves extracted features into a CSV file.
  - `segment.cpp`: Segments objects from the background.
  - `segment_process.cpp`: Processes segmentation.
  - `threshold_process.cpp`: Applies dynamic thresholding.
  - `thresholding.cpp`: Implements thresholding operations.
- `include/`: Contains header files for the project.
- `bin/`: Executables and other binary files.
- `CMakeLists.txt`: Configuration file for building the project using CMake.
- `data`: Contains results screenshots and videos.

## Features
- **Dynamic Thresholding**: Applies dynamic thresholding to adaptively segment objects from the background.
- **Morphological Filtering**: Cleans the binary images using morphological operations.
- **Segmentation**: Segments objects from the background for further processing.
- **Feature Extraction**: Extracts relevant features from segmented objects for classification.
- **Real-time Classification**: Classifies objects in real-time from a video stream.



## Getting Started
### Prerequisites
- Install [OpenCV](https://opencv.org/releases/) library.
- Install CMake.
- Ensure C++11 support in your compiler.

### Installation
1. Clone the repository:
```git clone https://github.com/jeff8971/Project3_YZ.git```
2. Navigate to the project directory:```cd Project3_YZ```
3. Build the project using CMake:
```
cd build
cmake ..
make
```

### Running the Application
After building, the project generates executables for different tasks within the `bin/` directory. Run these executables to perform tasks like cleaning the binary video, segmenting objects, extracting features, and classifying objects in real-time.

### Example Commands
- To start the real-time object recognition, run:
```./bin/classify```
- To apply dynamic thresholding, run:
```./bin/threshold_process```
- To clean the binary video, run:
```./bin/clean_process```
- To segment objects from the background, run:
```./bin/segment_process```
- To save features from segmented objects, run:
```./bin/save_feature```




### Note
- Ensure that your camera or video input device is correctly configured and accessible by the application.
- The project is configured to use an external camera (e.g., an iPhone camera) for video input. Adjust the `cv::VideoCapture` index accordingly if using a different video source.




## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Utilization of Travel Days
No travel days were utilized for this project. 
I've had a challenging for 2 weeks due to the demands of my full-time job, working 12 hours a day.



