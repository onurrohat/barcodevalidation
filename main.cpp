//bildgestützte automatisierung
#include <zbar.h>
#include <opencv2/opencv.hpp>
#include <iostream>  


using namespace cv;
using namespace std;
using namespace zbar;


typedef struct {

	string type;
	string data;
	vector <Point> location;

} decodedProduct;

	


	//Scanning process
void decode(Mat& im, vector<decodedProduct>& decodedProducts)
{

	// VideoCapture cap(0);

	ImageScanner scanner;
	scanner.set_config(ZBAR_QRCODE, ZBAR_CFG_ENABLE, 1); //read just qr code

	Mat imGray;
	cvtColor(im, imGray, COLOR_BGR2GRAY);

	Image image(im.cols, im.rows, "Y800", (uchar*)imGray.data, im.cols * im.rows); // 8 bit image format

	int s = scanner.scan(image); //scanning

	for (Image::SymbolIterator qrcode = image.symbol_begin(); qrcode != image.symbol_end(); ++qrcode)
	{

		decodedProduct pro;

		pro.type = qrcode->get_type();
		pro.data = qrcode->get_data();

		cout << "Output : " << pro.data << endl << endl;


		for (int i = 0; i < qrcode->get_location_size(); i++)
		{

			pro.location.push_back(Point(qrcode->get_location_x(i), qrcode->get_location_y(i)));
		}


		decodedProducts.push_back(pro);


	}

}
//Displaying

void display(Mat& im, vector<decodedProduct>& decodedProducts)
{

	for (int i = 0; i < decodedProducts.size(); i++)
	{
		vector <Point> points = decodedProducts[i].location;
		vector <Point> hull;


		if (points.size() > 4)

			convexHull(points, hull); // find the smallest convex polygon
		else
			hull = points;


		int n = hull.size();

		for (int j = 0; j < n; j++)
		{
			line(im, hull[j], hull[j + 1 % n], Scalar(0, 255, 0), 3);
		}


	}

	imshow("QR Code Scanner", im);
	waitKey(0);


}

int main(int argc, char* argv[])

{
	//VideoCapture cap(0);
	

	Mat im = imread("ee.png");

	//cap.set(CAP_PROP_FRAME_WIDTH, 640);
	//cap.set(CAP_PROP_FRAME_WIDTH, 480);

	//namedWindow("Results", -1);

	//while (1)
	//{

		//cap >> im;



	vector<decodedProduct> decodedProducts;

	decode(im, decodedProducts);

	display(im, decodedProducts);
	imshow("Results", im);

	return EXIT_SUCCESS;

	//}
}














	

