//-----------------------------------【头文件包含部分】---------------------------------------
//  描述：包含程序所依赖的头文件
//---------------------------------------------------------------------------------------------- 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

//-----------------------------------【命名空间声明部分】---------------------------------------
//  描述：包含程序所使用的命名空间
//-----------------------------------------------------------------------------------------------   
using namespace std;
using namespace cv;

//-----------------------------------【全局函数声明部分】--------------------------------------
//  描述：全局函数声明
//-----------------------------------------------------------------------------------------------
static void ContrastAndBright(int, void*);

//-----------------------------------【全局变量声明部分】--------------------------------------
//  描述：全局变量声明
//-----------------------------------------------------------------------------------------------
int g_nContrastValue; //对比度值
int g_nBrightValue;  //亮度值
Mat g_srcImage, g_dstImage;
//-----------------------------------【main( )函数】--------------------------------------------
//  描述：控制台应用程序的入口函数，我们的程序从这里开始
//-----------------------------------------------------------------------------------------------
int main1()
{
    //改变控制台前景色和背景色
    //system("color 2F");
    // 读入用户提供的图像
    g_srcImage = imread("D:\\CV\\IP_data\\bgs_data\\13.jpg");//2 7 8 13
    if (!g_srcImage.data) { printf("读取g_srcImage图片错误~！ \n"); return false; }
    g_dstImage = Mat::zeros(g_srcImage.size(), g_srcImage.type());

    //设定对比度和亮度的初值
    g_nContrastValue = 80;
    g_nBrightValue = 80;

    //创建窗口
    namedWindow("【效果图窗口】", 1);

    //创建轨迹条
    createTrackbar("对比度：", "【效果图窗口】", &g_nContrastValue, 300, ContrastAndBright); // 取值0~300
    createTrackbar("亮   度：", "【效果图窗口】", &g_nBrightValue, 200, ContrastAndBright); // 取值0~200

    //调用回调函数
    ContrastAndBright(g_nContrastValue, 0);
    ContrastAndBright(g_nBrightValue, 0);

    //输出一些帮助信息
    cout << endl << "\t运行成功，请调整滚动条观察图像效果\n\n"
        << "\t按下“q”键时，程序退出\n";

    //按下“q”键时，程序退出
    while (char(waitKey(1)) != 'q') {}
    return 0;
}

//-----------------------------【ContrastAndBright( )函数】------------------------------------
//  描述：改变图像对比度和亮度值的回调函数
//-----------------------------------------------------------------------------------------------
static void ContrastAndBright(int, void*)
{

    // 创建窗口
    namedWindow("【原始图窗口】", 1);

    // 三个for循环，执行运算 g_dstImage(i,j) = a*g_srcImage(i,j) + b
    for (int y = 0; y < g_srcImage.rows; y++)
    {
        for (int x = 0; x < g_srcImage.cols; x++)
        {
            for (int c = 0; c < 3; c++)
            {
                // sigmoid 函数控制对比度, 常量g_nBrightValue控制亮度
                double t = ((g_srcImage.at<Vec3b>(y, x)[c] - 127) / 225.00) * g_nContrastValue * 0.1;
                g_dstImage.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(g_srcImage.at<Vec3b>(y, x)[c] * ((1.00 / (1.00 + exp(-t))) + 0.5) + g_nBrightValue - 100);
            }
        }
    }

    // 显示图像
    imshow("【原始图窗口】", g_srcImage);
    imshow("【效果图窗口】", g_dstImage);
}