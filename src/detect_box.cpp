/**
 * @file detect_box.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-05-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "bga/detect_box.hpp"


ContourOperationResult findContours(
    cv::Mat& frame,
    Threshold threshold,
    const int minArea,
    const int filter)
{
    cv::Mat clonedFrame;
    cv::RNG rng(12345);

    // Convert to Gray-Scale
    cv::cvtColor(
        frame,
        clonedFrame,
        cv::COLOR_BGR2GRAY
    );
    std::cout << "Converted to gray scale\n";
    // Blur the image via a 5x5 Gaussian Blur
    cv::GaussianBlur(
        clonedFrame,
        clonedFrame,
        cv::Size(5, 5),
        1.0
    );
    std::cout << "Blurred via Gaussian.\n";

    cv::Canny(
        clonedFrame,
        clonedFrame,
        threshold.min,
        threshold.max
    );
    std::cout << "Canny\n";

    /* const cv::Mat kernel = (cv::Mat_<char>(5, 5) << 
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 1); */

    const cv::Mat kernel = cv::Mat::ones(5, 5, CV_8SC1);

    // Dilate the image
    cv::dilate(
        clonedFrame,
        clonedFrame,
        kernel,
        cv::Point(-1, -1),
        3
    );
    std::cout << "Dilation\n";

    // Erosion
    cv::erode(
        clonedFrame,
        clonedFrame,
        kernel,
        cv::Point(-1, -1),
        2
    );
    std::cout << "Erosion\n";

    ContourOperationResult result;

    Contours contours;
    Hierarchy hierarchy;

    cv::findContours(
        clonedFrame,
        contours,
        hierarchy,
        cv::RETR_EXTERNAL,
        cv::CHAIN_APPROX_NONE
    );
    
    for(std::size_t i = 0; i < contours.size(); i++)
    {
        const double contourArea = cv::contourArea(
            contours[i]
        );

        if(contourArea > minArea)
        {
            const double arcLenth = cv::arcLength(contours[i], true);
            Contour approximation;
            cv::approxPolyDP(contours[i], approximation, (0.02*arcLenth), true);
            cv::Rect bBox = cv::boundingRect(
                approximation
            );

            if(filter > 0)
            {
                if(approximation.size() == filter)
                {
                    result.contours.push_back(
                        {
                            (int)approximation.size(),
                            contourArea,
                            approximation,
                            bBox,
                            contours[i]
                        }
                    );
                }
            }
            else
            {
                if(approximation.size() == filter)
                {
                    result.contours.push_back(
                        {
                            (int)approximation.size(),
                            contourArea,
                            approximation,
                            bBox,
                            contours[i]
                        }
                    );
                }
            }

            cv::Scalar color = cv::Scalar(
                rng.uniform(0, 256),
                rng.uniform(0, 256),
                rng.uniform(0, 256)
            );

            cv::rectangle(frame, bBox.tl(), bBox.br(), color, 2);
        }

    }

    std::sort(result.contours.begin(), result.contours.end(), [](const ContourInfo& c1, const ContourInfo& c2) -> bool
    {
        return c1.area > c2.area;
    });



    result.frame = clonedFrame;
    return result;

}

void findBox(cv::Mat& frame, cv::Mat& segmented)
{
    cv::Mat frameInHSV;
    cv::cvtColor(
        frame,
        frameInHSV,
        cv::COLOR_BGR2HSV
    );

    std::vector<cv::Mat> channelsOfHSV;
    channelsOfHSV.reserve(3);
    cv::split(frameInHSV, channelsOfHSV);
    auto hueChannel = channelsOfHSV.at(0);
    
    cv::Mat threshold;
    // original 127-255
    cv::threshold(
        hueChannel,
        threshold,
        127,
        255,
        cv::THRESH_BINARY_INV | cv::THRESH_OTSU    
    );

    // Find largest contour

    Contours contours;
    Hierarchy hierarchy;

    cv::findContours(
        threshold,
        contours,
        hierarchy,
        cv::RETR_EXTERNAL,
        cv::CHAIN_APPROX_NONE
    );

    std::sort(contours.begin(), contours.end(), [](
        const Contour& c1, const Contour& c2
    ) -> bool
    {
        double c1_area = cv::contourArea(c1);
        double c2_area = cv::contourArea(c2);
        return c1_area > c2_area;
    });

    cv::Mat black = cv::Mat::zeros(threshold.size(), CV_8SC1);

    cv::drawContours(black, contours, 0, 255, -1);
    
    cv::bitwise_and(frame, frame, segmented, black);

}