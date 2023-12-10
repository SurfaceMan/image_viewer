#ifndef EDGESUBPIX2_H
#define EDGESUBPIX2_H

#include <opencv2/opencv.hpp>

void EdgePoint(const cv::Mat &img, std::vector<std::vector<cv::Point2d>> &points,
               std::vector<std::vector<cv::Vec2d>> &dirs, double sigma, double low, double high);
#endif // EDGESUBPIX2_H
