#include<opencv2/core/core.hpp>
//#include<opencv2/imgcodecs.hpp>
//#include<opencv2/core/saturate.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream> 
#include <opencv2/imgproc.hpp>
#include<cmath>

//#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/core.hpp"

using namespace std;
using namespace cv;
string path = "D:\\CV\\shapechange.png";
string path_video = "D:\\CV\\source4.mp4";
Mat image = imread(path);
Mat new_img;
Mat& haha_mirror(Mat& src);

Mat& Mirror_Magnify(Mat& src)
{
	/*
	图像放大：凸透镜
	x = (dx / 2)*(sqrt(pow(dx, 2) + pow(dy, 2)) / r) + cx;
	y = (dy / 2)*(sqrt(pow(dx, 2) + pow(dy, 2)) / r) + cy;
	*/

	Mat canvas = Mat::zeros(src.size(), src.type()); //画布，重新生成哈哈图像
	src.copyTo(canvas);
	//图像中心
	int cx = src.cols / 2;
	int cy = src.rows / 2;
	//决定哈哈镜大小
	int radius = 200;

	//图像像素修改
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			//任意像素点到图像中心距离
			int dx = j - cx;
			int dy = i - cy;
			//重新映射像素点位置
			int x = (dx / 2) * (sqrt(pow(dx, 2) + pow(dy, 2)) / radius) + cx;
			int y = (dy / 2) * (sqrt(pow(dx, 2) + pow(dy, 2)) / radius) + cy;
			int w = src.cols, h = src.rows;
			Vec3b mid1, mid2;
			for (int c = 0; c < 3; c++)
			{
				//防止越界
				if ((x > 0 && x < src.cols) && (y > 0 && y < src.rows))
				{
					//双线性插值
					//canvas.at<Vec3b>(i, j)[c] = src.at<Vec3b>(y, x)[c];
					uchar aa = src.at<Vec3b>(y, x)[c];
					uchar bb = src.at<Vec3b>(min(h - 1, y + 1),x)[c];
					uchar cc = src.at<Vec3b>(y,min(x + 1, w - 1))[c];
					uchar dd = src.at<Vec3b>(min(h - 1, y + 1),min(x + 1, w - 1))[c];
					mid1[c] = saturate_cast<uchar>(aa + (float)dy/h * (bb - aa));
					mid2[c] = saturate_cast<uchar>(cc + (float)dy / h * (dd - cc));
					canvas.at<Vec3b>(i, j)[c] = saturate_cast<uchar>(mid1[c] + (float)dx / w * (mid2[c] - mid1[c]));
				}
			}
		}
	}
	canvas.copyTo(new_img);
//	imshow("Mirror_Magnify", canvas);

	return new_img;
}
// 凹透镜
Mat& haha_change(Mat& img) {
	//new_img = image;
	int h = img.rows, w = img.cols;
	int center_r = h / 2, center_c = w / 2;
	int radius = 50;
	float x_, y_, r, theta, x_norm_img, y_norm_img, x_img, y_img;
	for (int i = 0; i < h; i++) {//x===h
		cout << "i=" << i << ":";
		for (int j = 0; j < w; j++) {
			//cout <<"j="<< j << " ";
			x_ = ((float)i - 0.5 * h) / 0.5 / (float)h, y_ = ((float)j - 0.5 * w) / 0.5 / (float)w;
			//cout << "x_:" << x_ << "," << y_ << "\n";

			r = sqrt(x_ * x_ + y_ * y_);
			theta = (1 - r) * (1 - r);
			x_norm_img, y_norm_img;
			if (r >= 1) {
				x_norm_img = x_, y_norm_img = y_;
			}
			else
			{
				x_norm_img = cos(theta) * x_ - sin(theta) * y_;
				y_norm_img = sin(theta) * x_ + cos(theta) * y_;
			}
			x_img = (x_norm_img + 1) * 0.5 * h, y_img = (y_norm_img + 1) * 0.5 * w;
			//cout << "x_img:" << x_img << "," << y_img << "\n";
			if (x_img > h - 1) x_img = h - 1;
			if (y_img > w - 1) y_img = w - 1;
			for (int cc = 0; cc < 3; cc++) {
				new_img.at<Vec3b>(i, j)[cc] = img.at<Vec3b>((int)x_img, (int)y_img)[cc];
			}
		}
	}
/* for (int i = center_r; i >0; i--) {
for (int j = center_c - radius; j < center_c + radius; j++) {
for (int cc = 0; cc < 3; cc++) {
new_img.at<Vec3b>(i , j)[cc] = img.at<Vec3b>(int(i/2), j)[cc];
}
}
}*/
return new_img;
}

int main_video() {
	//string path = "D:\\CV\\source2.mp4";
	cv::VideoCapture cap;
	cv::VideoWriter outv;
	cap.open(0);
	if (cap.isOpened()) cout << "camere opened!!!!!!!!!!!!!!!\n";

	cap.set(CV_CAP_PROP_FRAME_WIDTH, 1000);      
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1000);
	cap.set(CV_CAP_PROP_FPS, 40);

	namedWindow("live", WINDOW_NORMAL);

	double fps = cap.get(CV_CAP_PROP_FPS);//CV_CAP_PROP_FRAME_WIDTH
	//cv::Size image_size = cv::Size((int)(cap.get(CV_CAP_PROP_FRAME_WIDTH)), (int)(cap.get(CV_CAP_PROP_FRAME_HEIGHT)));
	string path_out = "D:\\CV\\IP3_4.avi";
	
	//int ex = static_cast<int>(cap.get(CV_CAP_PROP_FOURCC));
	double frameWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	double frameHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	outv.open(path_out, CV_FOURCC('D', 'I', 'V', 'X'), fps, Size(frameWidth, frameHeight));
	if (outv.isOpened()) cout << "out ok*******************\n";
	else cout << "cannot out！！！！！！！！！！！！！！！！！！！\n";

	Mat vframe;
	while (true)
	{
		cap >> vframe;
		if (!vframe.empty()) {
			new_img = Mat::zeros(vframe.size(), vframe.type());
			//haha_mirror(vframe);
			Mirror_Magnify(vframe);
			outv << new_img;
			imshow("live", new_img);
			//cout << "showing\n";
		}
		else {
			cout << "cannot\n";
			break;
		}
		if (waitKey(30) == 'q') break;
		//if (waitKey(5) >= 0) break;
	}
	cap.release();
	destroyAllWindows();
	return 0;
}
int main() {
	main_video();
	return 0;
}
/*
*
*
*

*  for (int i = 0; i < center_r + radius; i++) {
for (int j = center_c - radius; j < center_c + radius; j++) {
cout << "i:" << i << "j:" << j << "\n";
int tr = center_r - i, tc = center_c - j;
int dis = sqrt((center_r - i) * (center_r - i) + (center_c - j) * (center_c - j));
int newr, newc;
if (dis < radius) {
newr = int(int(tr / 2.0) * dis + center_r);
newc = int(int(tc / 2.0) * dis + center_c);
}
else {
newr = i;
newc = j;
}
cout << "newr:" << newr << "newc:" << newc << "\n";
if (newr < rows_&&newr>0&&newc>0 && newc < cols_) {
for (int cc = 0; cc < 3; cc++) {
new_img.at<Vec3b>(i, j)[cc] = img.at<Vec3b>(newr, newc)[cc];
}
}
}
}
int main()
{
VideoCapture capture; //声明视频读入类
capture.open(0, cv::CAP_DSHOW); //从摄像头读入视频 0表示从摄像头读入

if (!capture.isOpened()) //先判断是否打开摄像头
{
cout << "can not open";
cin.get();
return -1;
}

namedWindow("name");

while (1) {
Mat cap; //定义一个Mat变量，用于存储每一帧的图像
capture >> cap; //读取当前帧
if (!cap.data) continue;
if (!cap.empty()) //判断当前帧是否捕捉成功 **这步很重要
{

imshow("name", cap); //若当前帧捕捉成功，显示
cout << "catch\n";
}
else
cout << "can not ";
waitKey(30); //延时30毫秒
}

return 0;
}
*/