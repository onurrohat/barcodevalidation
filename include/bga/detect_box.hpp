/**
 * @file detect_box.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-05-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef DETECT_BOX_HPP_
#define DETECT_BOX_HPP_

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <optional>
#include <vector>
#include <iostream>

using Contour = std::vector<cv::Point>;
using Contours = std::vector<Contour>;
using Hierarchy = std::vector<cv::Vec4i>;

struct ContourInfo
{
    int approxLength;
    double area;
    Contour approx;
    cv::Rect bBox;
    Contour contour;
};

struct ContourOperationResult
{
    cv::Mat frame;
    std::vector<ContourInfo> contours;
};

struct Threshold
{
    double min = 100.0;
    double max = 100.0;
};

ContourOperationResult findContours(
    cv::Mat& frame,
    Threshold threshold,
    const int minArea,
    const int filter
);

void findBox(cv::Mat& frame, cv::Mat& segmented);

#endif 