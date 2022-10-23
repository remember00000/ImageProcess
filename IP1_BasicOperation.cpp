#include<opencv2/opencv.hpp>
#include<iostream>
#include<math.h>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
//#include "opencv2/nonfree/nonfree.hpp"
//#include"opencv2/legacy/legacy.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

const char* src_win = "Source Image";
const string src_path = "D:\\CV\\IP_data\\a.png";
//const string src_path = "D:\\CV\\IP_data\\bean.bmp";
//const string src_path = "D:\\CV\\IP_data\\ex3.png";
const int N = 5e5;
Mat src_img;
int Show_img() {
	//读取图片
	Mat src_img = imread(src_path, IMREAD_COLOR);
	//确认图片不空
	if (src_img.empty()) {
		cout << "The image is empty!\n";
		return EXIT_FAILURE;
	}
	//定义显示窗口(可省略, 这里可以设置窗口尺寸)
	namedWindow(src_win, WINDOW_AUTOSIZE);
	//显示图片
	imshow(src_win, src_img);
	//保持窗口始终存在
	waitKey(0);
	return 0;
}
void getChannel(const uchar* input, int width, int height, int inStep, int inChannels, uchar*& output, int outStep, int channelToGet) {
	//输出uchar数组 先设为较大的数组 防止溢出
	uchar ans[N] = {0};
	int poi = 0;//src_img对应的input指针指向访问位置
	int cnt = 0;//ans中指针 用于指向存储位置
	//图像交叉存储方式下访问每个像素的固定channel
	for (int y = 0; y < height; y++) {
		//考虑图像对齐填充下 每次找到a row的第一个位置
		poi = channelToGet+y*inStep;
		//cout <<y<<":"<< poi << ' ';
		//遍历一行像素的特定channel
		for (int x = 0; x < width; x++) {
			//poi += x * inChannels;
			ans[cnt] = input[poi];
			poi +=  inChannels;
			//cout <<  y << "-" <<"  x  " << x << " poi " << poi << "\n";
			cnt++;
		}
	}
	//这种紧密存储方式下, 没有为了对齐填充, 所以step等于行像素值
	outStep = width;
	//ans.resize()
	//输出output指针指向输出uchar ans[]
	output = ans;
	cout <<"getchannel "<<getChannel<<" value:"<<int(output[0])<<" get ok!\n";// << output;
}
void getAlpha(Mat& img) {
	//创建大小相同的Mat
	Mat ans = Mat::zeros(img.size(), CV_8UC1);
	//每个像素提取第四通道
	for (int x = 0; x < img.cols; x++) {
		for (int y = 0; y < img.rows; y++) {
			ans.at<uchar>(y, x) = img.at<Vec4b>(y, x)[3];
		}
	}
	imshow("alpha", ans);
	waitKey(0);
	return;
}
void changeBackground(Mat& src_img) {
	//读取背景图片
	string bg_path = "D:\\CV\\IP_data\\background.jpg";
	Mat bg_img = imread(bg_path);
	//背景混合图片
	Mat ans_img = Mat::zeros(src_img.size(), src_img.type());
	cout << ans_img.channels() << "\n";
	//对于任意大小背景的图片 更换为目标图片的尺寸
	resize(bg_img, bg_img, src_img.size());
	imshow("bg_img", bg_img);
	waitKey(0);
	cout << "bg_img.size:" << bg_img.size() << "ans_img.size:" << ans_img.size() << "\n";
	int cal_val = 0;

	for (int x = 0; x < src_img.cols; x++) {
		for (int y = 0; y < src_img.rows; y++) {
			int alpha = src_img.at<Vec4b>(y, x)[3];
			for (int ch = 0; ch < 3; ch++) {
				//alpha混合公式
				cal_val = src_img.at<Vec4b>(y, x)[ch] * (float(alpha) / 255.0) + bg_img.at<Vec3b>(y, x)[ch] * ((255.0 - float(alpha)) / 255.0);
				//防止数据溢出 等效于saturate_cast
				if (cal_val > 255) cal_val = 255;
				ans_img.at<Vec4b>(y, x)[ch] = cal_val;
			}
		}
	}
	//namedWindow("after",)
	imshow("after", ans_img);
	waitKey(0);
	return;
}
int main(int argc, char** argv) {
	//Show_img();
	Mat src_img = imread(src_path, IMREAD_UNCHANGED);
	//cout << src_img.channels();
	cout << src_img.cols << 'x' << src_img.rows << " step: " << src_img.step << "\n";
	imshow("test", src_img);
	waitKey(0);
	//getChannel 任意读取通道
	//Mat转为uchar*
	const uchar* input = src_img.data;
	uchar* output=NULL;
	int outStep=0;
	getChannel(input, src_img.cols, src_img.rows, src_img.step, src_img.channels(), output, outStep, 2);
	//uchar*转为Mat
	Mat des_img(src_img.rows, src_img.cols, CV_8UC1, output);
	//cout << output << endl;
	cout << des_img.cols << 'x' << des_img.rows << " step: " << des_img.step << "\n";
	namedWindow("des", WINDOW_AUTOSIZE);
	imshow("des", des_img);
	waitKey(0);
	//getchannel---------end

	//cout <<"===="<<src_img.at<Vec3b>(0, 570)[0];
	//得到alpha
//	getAlpha(src_img);
	//图像更换背景
//	changeBackground(src_img);
	return 0;
}