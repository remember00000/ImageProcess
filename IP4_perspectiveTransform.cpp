#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc\imgproc_c.h>
#include "math.h"
using namespace std;
using namespace cv;
Point2f src_points[4];
// 取原图四个顶点在目标图像上的坐标
Point2f dst_points[4] = { Point2f(60,50), Point2f(400,50), Point2f(500,300), Point2f(50, 400) };
Mat srcImage;
Mat dstImage;
Mat transform_mat;
int selected = -1;//没点击图像

void transformImage() {
    //透视变换矩阵
    transform_mat = getPerspectiveTransform(src_points, dst_points);
    //wrap：扭曲变形 进行透视变换
    warpPerspective(srcImage, dstImage, transform_mat, Size(srcImage.cols, srcImage.rows), CV_INTER_CUBIC);
    // 标记四个顶点和边框
    for (int i = 0; i < 4; i++) {
        circle(dstImage, dst_points[i], 2, Scalar(0, 0, 255), 2);
        line(dstImage, dst_points[i], dst_points[(i+1)%4], Scalar(0, 255, 255), 1);
    }

    imshow("src", srcImage);
    imshow("dst", dstImage);
}

void init() {
    // 源图像
    srcImage = imread("D:\\CV\\07.jpg");
    // 目标图像

    // 取原图四个顶点
    src_points[0] = Point2f(0, 0);
    src_points[1] = Point2f(srcImage.cols-1, 0);
    src_points[3] = Point2f(0, srcImage.rows-1);
    src_points[2] = Point2f(srcImage.cols-1, srcImage.rows-1);

}
void on_Mouse(int event, int x, int y, int flags, void* param) {
    //鼠标左键点击
    if (event == EVENT_LBUTTONDOWN)
    {

        for (int i = 0; i < 4; i++) {
            if ((pow(dst_points[i].x - x, 2) + pow(dst_points[i].y - y, 2)) <= 10)
                selected = i;//点击图像 并选择对应顶点
        }
    }
    //移动 只更新对应顶点
    if (event == EVENT_MOUSEMOVE) {
        if (selected >= 0 && selected <= 4) {
            dst_points[selected].x = x;
            dst_points[selected].y = y;
            transformImage();
        }
    }
    //释放
    if (event == EVENT_LBUTTONUP) {
        selected = -1;
        transformImage();

    }
}

int main()
{
    //创建窗口
    namedWindow("dst",WINDOW_AUTOSIZE);
    init();
    transformImage();
    setMouseCallback("dst", on_Mouse, 0);
    waitKey(0);

}