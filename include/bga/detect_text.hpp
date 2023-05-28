/**
 * @file detect_text.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-05-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef DETECT_TEXT_HPP_
#define DETECT_TEXT_HPP_

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <tesseract/baseapi.h>

#include <string>
#include <iostream>

std::string detectTexts(cv::Mat& frame);

#endif // DETECT_TEXT_HPP_