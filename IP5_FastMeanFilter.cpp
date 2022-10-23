#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc\imgproc_c.h>
#include "math.h"
using namespace std;
using namespace cv;

const int N = 800;
int integral_mat[3][N][N];
Mat ori_img, des_img;

void MeanFilter(int window_size, void*) {
	//�˲���ֻ��Ϊ������ż��ת��Ϊ����
	if (window_size %2 == 0) {
		//cout << "MeanFilter window size must be odd but size=" << window_size << "\n";
		window_size++;
	}
	//boxFilter
	Mat boxfilter_img;
	double time_record = getTickCount();
	boxFilter(ori_img, boxfilter_img, -1, Size(window_size, window_size));
	time_record = ((double)getTickCount() - time_record) / getTickFrequency();
	printf("boxFilter time: %f    ", time_record);
	imshow("BoxFilter_IMG", boxfilter_img);
	//end boxFilter test
	time_record = getTickCount();
	//��ͼ���padding�������Ե���⴦��
	Mat padding_img;
	int half_filter_size = window_size / 2;
	cv::copyMakeBorder(ori_img, padding_img, half_filter_size, half_filter_size, half_filter_size, half_filter_size, BORDER_DEFAULT);
	//��άǰ׺�� ����ͼ����
	memset(integral_mat, 0, sizeof(integral_mat));
	for (int c = 0; c < 3; c++) {
		for (int y = 0; y < padding_img.rows; y++) {
			for (int x = 0; x < padding_img.cols; x++) {
				//������0��ʼ���е�Խ��
				integral_mat[c][y + 1][x + 1] = integral_mat[c][y][x + 1] + integral_mat[c][y + 1][x] - integral_mat[c][y][x] + padding_img.ptr<uchar>(y, x)[c];//��ʹ��0�ռ䲻�ÿ��ǳ�ʼ��
			}
		}
	}

	//���û���ͼ����ֵ�˲�
	des_img = Mat::zeros(ori_img.size(), ori_img.type());//�ȳ�ʼ����������ʾ���λ��
	for (int c = 0; c < 3; c++) {
		for (int y = 0; y < ori_img.rows; y++) {
			for (int x = 0; x < ori_img.cols; x++) {
				int right, left, bottom, top;
				right = x + half_filter_size + half_filter_size, left = max(0, x + half_filter_size - half_filter_size - 1), bottom = y + half_filter_size + half_filter_size, top = max(0, y + half_filter_size - half_filter_size - 1);
				int new_val = integral_mat[c][bottom][right] - integral_mat[c][top][right] - integral_mat[c][bottom][left] + integral_mat[c][top][left];
				new_val = new_val / (window_size * window_size);
				des_img.ptr<uchar>(y, x)[c] = new_val;
			}
		}
	}
	time_record = ((double)getTickCount() - time_record) / getTickFrequency();
	printf("my mean function time : %f\n", time_record);

	cv::imshow("New Image", des_img);
}

//�������δ��� ��άǰ׺�ʹ��� ���
int main() {
	string path = "D:\\CV\\shapechange.png";//"D:\CV\02.jpg"
	ori_img = imread(path);
	int win_size = 5;

	cv::namedWindow("Origin Image", WINDOW_AUTOSIZE);
	cv::namedWindow("New Image");

	createTrackbar("win_size", "New Image", &win_size, 30, MeanFilter);
	cv::imshow("Origin Image", ori_img);

	cout << "show image over\n";
	cv::waitKey(0);
	return 0;
}
