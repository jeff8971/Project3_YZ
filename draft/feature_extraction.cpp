


#include <opencv2/opencv.hpp>
#include "feature_extraction.h"
#include <iostream> 
#include <fstream>


// Function to compute the contour of a specific region in an image based on label IDs
std::vector<cv::Point> computeRegionContour(const cv::Mat& labels, int regionID) {
    std::vector<std::vector<cv::Point>> contours;
    cv::Mat regionMask;
    cv::compare(labels, regionID, regionMask, cv::CMP_EQ);

    cv::findContours(regionMask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    if (contours.size() > 0) {
        return contours[0];
    }
    else {
        return {};
    }
}

// Function to compute various features for a specific region in an image, including oriented bounding box, 
// aspect ratio, percent filled, and image moments
RegionFeatures computeRegionFeatures(const cv::Mat& labels, int regionID, const cv::Mat& regionStats) {
    RegionFeatures features;
    std::vector<cv::Point> points;

    // Extract all points that belong to the region
    for (int i = 0; i < labels.rows; ++i) {
        for (int j = 0; j < labels.cols; ++j) {
            if (labels.at<int>(i, j) == regionID) {
                points.push_back(cv::Point(j, i));
            }
        }
    }

    if (points.empty()) {
        std::cerr << "Error: No points found in region " << regionID << std::endl;
        return features; 
    }

    // Calculate the oriented bounding box
    cv::RotatedRect rect = cv::minAreaRect(points);
    features.orientedBoundingBox = rect;

    // Calculate percent filled and aspect ratio
    int area = regionStats.at<int>(regionID, cv::CC_STAT_AREA);
    int bboxArea = (int)rect.size.width * (int)rect.size.height;
    features.percentFilled = static_cast<float>(area) / static_cast<float>(bboxArea);
    features.bboxAspectRatio = rect.size.width / rect.size.height;

    // Calculate image moments
    cv::Moments m = cv::moments(points, true);
    double hu[7];
    cv::HuMoments(m, hu);
    // Copy the hu moments into the features structure
    for (int i = 0; i < 7; ++i) {
        features.huMoments[i] = hu[i];
    }

    features.contour = points;

    return features;
}


void saveFeatureVector(const RegionFeatures& features, const std::string& label, const std::string& filename) {
    std::ofstream file(filename, std::ios::app);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    file << label << ",";
    for (const auto& moment : features.huMoments) {
        file << moment << ",";
    }
    file << features.percentFilled << ",";
    file << features.bboxAspectRatio << std::endl;
    file.close();
}