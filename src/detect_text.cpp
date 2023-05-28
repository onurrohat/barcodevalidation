/**
 * @file detect_text.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-05-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "bga/detect_text.hpp"

std::string detectTexts(cv::Mat& frame)
{
    tesseract::TessBaseAPI* ocr = new tesseract::TessBaseAPI();

    ocr->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);

    ocr->SetPageSegMode(tesseract::PSM_SPARSE_TEXT);
    ocr->SetImage(frame.data, frame.cols, frame.rows, 3, frame.step);
    
    const auto found = std::string(ocr->GetUTF8Text());
    std::string clearText = "";
    for(int i = 0; i < found.size(); i++)
    {
        if(found[i] == '\n')
            break;
        clearText += found[i];
    }

/* 
    std::cout << found << std::endl; */

    ocr->End();

    return clearText;
}