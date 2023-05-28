/**
 * @file decode_barcode.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-05-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef DECODE_BARCODE_HPP_
#define DECODE_BARCODE_HPP_

#include <zbar.h>

#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

struct DecodedBarcode
{
    std::vector<cv::Point> location;
    std::string type;
    std::string data;
};

std::vector<DecodedBarcode> decodeBarcode(cv::Mat& frame);

void displayOnFrame(cv::Mat& frame, const std::vector<DecodedBarcode>& barcodes);

#endif 