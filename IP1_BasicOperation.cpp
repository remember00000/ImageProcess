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
	//��ȡͼƬ
	Mat src_img = imread(src_path, IMREAD_COLOR);
	//ȷ��ͼƬ����
	if (src_img.empty()) {
		cout << "The image is empty!\n";
		return EXIT_FAILURE;
	}
	//������ʾ����(��ʡ��, ����������ô��ڳߴ�)
	namedWindow(src_win, WINDOW_AUTOSIZE);
	//��ʾͼƬ
	imshow(src_win, src_img);
	//���ִ���ʼ�մ���
	waitKey(0);
	return 0;
}
void getChannel(const uchar* input, int width, int height, int inStep, int inChannels, uchar*& output, int outStep, int channelToGet) {
	//���uchar���� ����Ϊ�ϴ������ ��ֹ���
	uchar ans[N] = {0};
	int poi = 0;//src_img��Ӧ��inputָ��ָ�����λ��
	int cnt = 0;//ans��ָ�� ����ָ��洢λ��
	//ͼ�񽻲�洢��ʽ�·���ÿ�����صĹ̶�channel
	for (int y = 0; y < height; y++) {
		//����ͼ���������� ÿ���ҵ�a row�ĵ�һ��λ��
		poi = channelToGet+y*inStep;
		//cout <<y<<":"<< poi << ' ';
		//����һ�����ص��ض�channel
		for (int x = 0; x < width; x++) {
			//poi += x * inChannels;
			ans[cnt] = input[poi];
			poi +=  inChannels;
			//cout <<  y << "-" <<"  x  " << x << " poi " << poi << "\n";
			cnt++;
		}
	}
	//���ֽ��ܴ洢��ʽ��, û��Ϊ�˶������, ����step����������ֵ
	outStep = width;
	//ans.resize()
	//���outputָ��ָ�����uchar ans[]
	output = ans;
	cout <<"getchannel "<<getChannel<<" value:"<<int(output[0])<<" get ok!\n";// << output;
}
void getAlpha(Mat& img) {
	//������С��ͬ��Mat
	Mat ans = Mat::zeros(img.size(), CV_8UC1);
	//ÿ��������ȡ����ͨ��
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
	//��ȡ����ͼƬ
	string bg_path = "D:\\CV\\IP_data\\background.jpg";
	Mat bg_img = imread(bg_path);
	//�������ͼƬ
	Mat ans_img = Mat::zeros(src_img.size(), src_img.type());
	cout << ans_img.channels() << "\n";
	//���������С������ͼƬ ����ΪĿ��ͼƬ�ĳߴ�
	resize(bg_img, bg_img, src_img.size());
	imshow("bg_img", bg_img);
	waitKey(0);
	cout << "bg_img.size:" << bg_img.size() << "ans_img.size:" << ans_img.size() << "\n";
	int cal_val = 0;

	for (int x = 0; x < src_img.cols; x++) {
		for (int y = 0; y < src_img.rows; y++) {
			int alpha = src_img.at<Vec4b>(y, x)[3];
			for (int ch = 0; ch < 3; ch++) {
				//alpha��Ϲ�ʽ
				cal_val = src_img.at<Vec4b>(y, x)[ch] * (float(alpha) / 255.0) + bg_img.at<Vec3b>(y, x)[ch] * ((255.0 - float(alpha)) / 255.0);
				//��ֹ������� ��Ч��saturate_cast
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
	//getChannel �����ȡͨ��
	//MatתΪuchar*
	const uchar* input = src_img.data;
	uchar* output=NULL;
	int outStep=0;
	getChannel(input, src_img.cols, src_img.rows, src_img.step, src_img.channels(), output, outStep, 2);
	//uchar*תΪMat
	Mat des_img(src_img.rows, src_img.cols, CV_8UC1, output);
	//cout << output << endl;
	cout << des_img.cols << 'x' << des_img.rows << " step: " << des_img.step << "\n";
	namedWindow("des", WINDOW_AUTOSIZE);
	imshow("des", des_img);
	waitKey(0);
	//getchannel---------end

	//cout <<"===="<<src_img.at<Vec3b>(0, 570)[0];
	//�õ�alpha
//	getAlpha(src_img);
	//ͼ���������
//	changeBackground(src_img);
	return 0;
}