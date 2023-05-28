#include "bga/detect_box.hpp"
#include "bga/decode_barcode.hpp"
#include "bga/detect_text.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>


int main(int argc, char** argv)
{
    const std::string filename = argv[1];
    cv::Mat sourceImg = cv::imread("/home/naci/Projects/bga/" + filename);
    if(sourceImg.empty())
    {
        std::cout << "Can not load source image" << std::endl;
        return 0;
    }
    cv::Mat segmented = cv::Mat(sourceImg.size(), CV_8SC3, cv::Scalar(0));
    findBox(sourceImg, segmented);
    
    std::vector<cv::Mat> channelsOfBGR;
    cv::split(segmented, channelsOfBGR);
    cv::Mat whiteThresh;
    cv::threshold(channelsOfBGR[2], whiteThresh, 230, 255, cv::THRESH_BINARY);
    cv::Mat barcodeIsolated;
    cv::cvtColor(whiteThresh, barcodeIsolated, cv::COLOR_GRAY2BGR);

    const auto barcodes = decodeBarcode(segmented);
    if(barcodes.empty())
    {
        std::cout << "No barcodes found\n";
    }
    else
    {
        for(const auto barcode : barcodes)
        {
            std::cout 
                << "Type: " << barcode.type
                << "Data: " << barcode.data << std::endl;
        }

        displayOnFrame(segmented, barcodes);
    }

    const std::string detectedViaOCR = detectTexts(sourceImg);

    cv::putText(segmented, "Detected via OCR: "+detectedViaOCR, cv::Point(10, segmented.cols / 8),
                cv::FONT_HERSHEY_COMPLEX, 1.0, CV_RGB(118, 185, 0), 2);
    cv::putText(segmented, "Read from Barcode data: " + barcodes[0].data, cv::Point(10, segmented.cols / 16),
                cv::FONT_HERSHEY_COMPLEX, 1.0, CV_RGB(118, 185, 0), 2);

    cv::imshow("Box", segmented);
    cv::waitKey();
    

    return 0;

}