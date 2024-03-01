

#ifndef FEATURE_EXTRACTION_H
#define FEATURE_EXTRACTION_H


#include <opencv2/opencv.hpp>
#include <vector>

// Structure to hold region features
struct RegionFeatures {
    cv::RotatedRect orientedBoundingBox;
    cv::Vec2f axisOfLeastMoment;
    cv::Vec2f orthogonalVector;
    float percentFilled;
    float bboxAspectRatio;
    double huMoments[7];
    std::vector<cv::Point> contour;
};

// Computes various features for a specific region in an image
RegionFeatures computeRegionFeatures(const cv::Mat& labels, int regionID, const cv::Mat& regionStats);


// Computes the contour of a specific region in an image
std::vector<cv::Point> computeRegionContour(const cv::Mat& labels, int regionID);

// Computes the Hausdorff distance between two contours
float hausdorffDistance(const std::vector<cv::Point>& contour1, const std::vector<cv::Point>& contour2);

void saveFeatureVector(const RegionFeatures& features, const std::string& label, const std::string& filename);



#endif // FEATURE_EXTRACTION_H
