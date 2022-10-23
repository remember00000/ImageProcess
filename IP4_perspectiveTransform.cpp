#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc\imgproc_c.h>
#include "math.h"
using namespace std;
using namespace cv;
Point2f src_points[4];
// ȡԭͼ�ĸ�������Ŀ��ͼ���ϵ�����
Point2f dst_points[4] = { Point2f(60,50), Point2f(400,50), Point2f(500,300), Point2f(50, 400) };
Mat srcImage;
Mat dstImage;
Mat transform_mat;
int selected = -1;//û���ͼ��

void transformImage() {
    //͸�ӱ任����
    transform_mat = getPerspectiveTransform(src_points, dst_points);
    //wrap��Ť������ ����͸�ӱ任
    warpPerspective(srcImage, dstImage, transform_mat, Size(srcImage.cols, srcImage.rows), CV_INTER_CUBIC);
    // ����ĸ�����ͱ߿�
    for (int i = 0; i < 4; i++) {
        circle(dstImage, dst_points[i], 2, Scalar(0, 0, 255), 2);
        line(dstImage, dst_points[i], dst_points[(i+1)%4], Scalar(0, 255, 255), 1);
    }

    imshow("src", srcImage);
    imshow("dst", dstImage);
}

void init() {
    // Դͼ��
    srcImage = imread("D:\\CV\\07.jpg");
    // Ŀ��ͼ��

    // ȡԭͼ�ĸ�����
    src_points[0] = Point2f(0, 0);
    src_points[1] = Point2f(srcImage.cols-1, 0);
    src_points[3] = Point2f(0, srcImage.rows-1);
    src_points[2] = Point2f(srcImage.cols-1, srcImage.rows-1);

}
void on_Mouse(int event, int x, int y, int flags, void* param) {
    //���������
    if (event == EVENT_LBUTTONDOWN)
    {

        for (int i = 0; i < 4; i++) {
            if ((pow(dst_points[i].x - x, 2) + pow(dst_points[i].y - y, 2)) <= 10)
                selected = i;//���ͼ�� ��ѡ���Ӧ����
        }
    }
    //�ƶ� ֻ���¶�Ӧ����
    if (event == EVENT_MOUSEMOVE) {
        if (selected >= 0 && selected <= 4) {
            dst_points[selected].x = x;
            dst_points[selected].y = y;
            transformImage();
        }
    }
    //�ͷ�
    if (event == EVENT_LBUTTONUP) {
        selected = -1;
        transformImage();

    }
}

int main()
{
    //��������
    namedWindow("dst",WINDOW_AUTOSIZE);
    init();
    transformImage();
    setMouseCallback("dst", on_Mouse, 0);
    waitKey(0);

}