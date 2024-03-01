#ifndef CLASSIFICATION_H
#define CLASSIFICATION_H

#include <opencv2/opencv.hpp>
#include "feature_extraction.h"

void train(const RegionFeatures& f, const std::string& label);
std::string classify(const RegionFeatures& f);

#endif // CLASSIFICATION_H