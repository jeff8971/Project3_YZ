

#include "segment.h"



// Function to get color for a region based on its centroid
cv::Vec3b getColorForRegion(cv::Point2d centroid, std::map<int, RegionInfo>& prevRegions) {
    // Iterate through previous regions
    for (const auto& reg : prevRegions) {
        // Get centroid of previous region
        cv::Point2d prevCentroid = reg.second.centroid;
        
        // Calculate distance between centroids
        double distance = cv::norm(centroid - prevCentroid);

        // If distance is within threshold, return color of previous region
        if (distance < 50) {
            return reg.second.color;
        }
    }

    // If no matching region found, return a random color
    return cv::Vec3b(rand() % 256, rand() % 256, rand() % 256);
}

// Function to segment objects in an image
cv::Mat segmentObjects(cv::Mat &src, cv::Mat &dst, int minRegionSize, std::map<int, RegionInfo>& prevRegions) {
    // Variables for connected components analysis
    cv::Mat labels, stats, centroids;
    int nLabels = cv::connectedComponentsWithStats(src, labels, stats, centroids, 8, CV_32S);

    // Initialize destination image
    dst = cv::Mat::zeros(src.size(), CV_8UC3);

    // Map to store current regions
    std::map<int, RegionInfo> currentRegions;

    // Iterate through labels
    for (int i = 1; i < nLabels; i++) {
        // Get area and centroid of region
        int area = stats.at<int>(i, cv::CC_STAT_AREA);
        cv::Point2d centroid(centroids.at<double>(i, 0), centroids.at<double>(i, 1));

        // Check if region meets minimum size requirement
        if (area > minRegionSize) {
            // Get color for region based on centroid
            cv::Vec3b color = getColorForRegion(centroid, prevRegions);
            
            // Add current region to map
            currentRegions[i] = {centroid, color};

            // Assign color to pixels of current region in destination image
            for (int y = 0; y < labels.rows; y++) {
                for (int x = 0; x < labels.cols; x++) {
                    if (labels.at<int>(y, x) == i) {
                        dst.at<cv::Vec3b>(y, x) = color;
                    }
                }
            }
        }
    }

    // Update previous regions with current regions
    prevRegions = std::move(currentRegions);
    
    // Return labels (connected components)
    return labels;
}