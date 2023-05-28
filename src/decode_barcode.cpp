#include "bga/decode_barcode.hpp"
#include <iostream>
std::vector<DecodedBarcode> decodeBarcode(cv::Mat& frame)
{   
    std::vector<DecodedBarcode> decoded;
    zbar::ImageScanner scanner;
    scanner.set_config(zbar::ZBAR_QRCODE, zbar::ZBAR_CFG_ENABLE, 1);

    cv::Mat frameGray;
    cv::cvtColor(frame, frameGray, cv::COLOR_BGR2GRAY);

    zbar::Image image(frame.cols, frame.rows, "Y800", (uchar*)frameGray.data, frame.cols * frame.rows);

    int scan = scanner.scan(image);

    for(zbar::Image::SymbolIterator barcode = image.symbol_begin(); barcode != image.symbol_end(); ++barcode)
    {
        DecodedBarcode qr;

        qr.type = barcode->get_type();
        qr.data = barcode->get_data();

        for(int i = 0; i < barcode->get_location_size(); i++)
        {
            qr.location.push_back(
                cv::Point(
                    barcode->get_location_x(i),
                    barcode->get_location_y(i)
                )
            );
        }

        decoded.push_back(qr);
    }


    return decoded;
}

void displayOnFrame(cv::Mat& frame, const std::vector<DecodedBarcode>& barcodes)
{
    for(int i = 0; i < barcodes.size(); i++)
    {
        std::vector<cv::Point> points = barcodes.at(i).location;
        std::vector<cv::Point> hull;

        if(points.size() > 4)
        {
            cv::convexHull(points, hull);
        }
        else
        {
            hull = points;
        }

        for(int j = 0; j < hull.size(); j++)
        {
            cv::line(frame, hull[j], hull[j+1 % hull.size()], cv::Scalar(0, 255, 0), 3);
        }
    }
}