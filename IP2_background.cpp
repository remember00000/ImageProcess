#include<opencv2/core/core.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/core/saturate.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream> 
#include <opencv2/imgproc.hpp>
#include<cmath>
using namespace std;
using namespace cv;

int alpha = 10; //< Simple contrast control 
int beta = 0;       //< Simple brightness control 
string impath = "D:\\CV\\13.jpg";  //1 2 7 8 13
string im_bgpath = "D:\\CV\\13_bg.jpg";
Mat image = imread(impath); //"D:\\CV\\01.jpg"
Mat new_image = Mat::zeros(image.size(), image.type()); //创建新的图像
int value = 10;
Mat image_bg, front_mask;


void Backgroundsustraction(int,void*) {
	//cout << "value:" << value << "\n";
	//计算两幅图片差异位置的RGB值输出到front_mask中
	cv::absdiff(image, image_bg, front_mask);
/*	imshow("New Image", front_mask);
	waitKey(0);*/
	//通过每个通道的平方和最后开方设定阈值
	for (int y = 0; y < image.rows; y++) {
		for (int x = 0; x < image.cols; x++) {
			int tsum = 0;
			for (int c = 0; c < 3; c++) {
				tsum += front_mask.at<Vec3b>(y, x)[c]* front_mask.at<Vec3b>(y, x)[c];
			}
			tsum = sqrt(tsum);
			if (tsum < value) {
				front_mask.at<Vec3b>(y, x)[0] = 0; front_mask.at<Vec3b>(y, x)[1] = 0; front_mask.at<Vec3b>(y, x)[2] = 0;
			}
			else {
				front_mask.at<Vec3b>(y, x)[0] = 255; front_mask.at<Vec3b>(y, x)[1] = 255; front_mask.at<Vec3b>(y, x)[2] = 255;
			}
		}
	}
	Mat new_front_mask = Mat::zeros(front_mask.size(), front_mask.type());
	Mat new_front_mask2 = Mat::zeros(front_mask.size(), front_mask.type());
	cv::medianBlur(front_mask, new_front_mask, 3);
	//cv::Canny(new_front_mask, new_front_mask2, 1, 3, 3);
	namedWindow("After blur Image", WINDOW_AUTOSIZE);//去噪声
	imshow("New Image", front_mask);
	imshow("After blur Image", new_front_mask);
}

int main(int, char** argv)
{
	image_bg = imread(im_bgpath);
	front_mask = Mat::zeros(image.size(), image.type());
	namedWindow("Original Image", WINDOW_AUTOSIZE);
	namedWindow("New Image", WINDOW_AUTOSIZE);
	createTrackbar("threshold", "New Image", &value, 1000, Backgroundsustraction);
	Backgroundsustraction(value,0);
	
	imshow("Original Image", image);
	waitKey(0);

	return 0;
}



/*
	createTrackbar("alpha", "New Image", &alpha, 1000, IntensityandConstrast_change);
	IntensityandConstrast_change(alpha, 0);

void IntensityandConstrast_change(int,void*) {
	//cout << "* Enter the alpha value [1.0-3.0]: "; cin >> alpha;
	//cout << "* Enter the beta value [0-100]: ";    cin >> beta;
	cout << "alpha=" << alpha << "\n";
	for (int y = 0; y < image.rows; y++) {
		for (int x = 0; x < image.cols; x++) {
			for (int c = 0; c < 3; c++) {
				new_image.at<Vec3b>(y, x)[c] = saturate_cast <uchar>(255 * (1 / (exp( alpha * (0.5 - (image.at<Vec3b>(y, x)[c] / 255.0))) + 1)));
				//new_image.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(alpha * (image.at<Vec3b>(y, x)[c]) + beta);
			}
		}
	}
	imshow("New Image", new_image);
}
*/
