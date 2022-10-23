//-----------------------------------��ͷ�ļ��������֡�---------------------------------------
//  ����������������������ͷ�ļ�
//---------------------------------------------------------------------------------------------- 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

//-----------------------------------�������ռ��������֡�---------------------------------------
//  ����������������ʹ�õ������ռ�
//-----------------------------------------------------------------------------------------------   
using namespace std;
using namespace cv;

//-----------------------------------��ȫ�ֺ����������֡�--------------------------------------
//  ������ȫ�ֺ�������
//-----------------------------------------------------------------------------------------------
static void ContrastAndBright(int, void*);

//-----------------------------------��ȫ�ֱ����������֡�--------------------------------------
//  ������ȫ�ֱ�������
//-----------------------------------------------------------------------------------------------
int g_nContrastValue; //�Աȶ�ֵ
int g_nBrightValue;  //����ֵ
Mat g_srcImage, g_dstImage;
//-----------------------------------��main( )������--------------------------------------------
//  ����������̨Ӧ�ó������ں��������ǵĳ�������￪ʼ
//-----------------------------------------------------------------------------------------------
int main1()
{
    //�ı����̨ǰ��ɫ�ͱ���ɫ
    //system("color 2F");
    // �����û��ṩ��ͼ��
    g_srcImage = imread("D:\\CV\\IP_data\\bgs_data\\13.jpg");//2 7 8 13
    if (!g_srcImage.data) { printf("��ȡg_srcImageͼƬ����~�� \n"); return false; }
    g_dstImage = Mat::zeros(g_srcImage.size(), g_srcImage.type());

    //�趨�ԱȶȺ����ȵĳ�ֵ
    g_nContrastValue = 80;
    g_nBrightValue = 80;

    //��������
    namedWindow("��Ч��ͼ���ڡ�", 1);

    //�����켣��
    createTrackbar("�Աȶȣ�", "��Ч��ͼ���ڡ�", &g_nContrastValue, 300, ContrastAndBright); // ȡֵ0~300
    createTrackbar("��   �ȣ�", "��Ч��ͼ���ڡ�", &g_nBrightValue, 200, ContrastAndBright); // ȡֵ0~200

    //���ûص�����
    ContrastAndBright(g_nContrastValue, 0);
    ContrastAndBright(g_nBrightValue, 0);

    //���һЩ������Ϣ
    cout << endl << "\t���гɹ���������������۲�ͼ��Ч��\n\n"
        << "\t���¡�q����ʱ�������˳�\n";

    //���¡�q����ʱ�������˳�
    while (char(waitKey(1)) != 'q') {}
    return 0;
}

//-----------------------------��ContrastAndBright( )������------------------------------------
//  �������ı�ͼ��ԱȶȺ�����ֵ�Ļص�����
//-----------------------------------------------------------------------------------------------
static void ContrastAndBright(int, void*)
{

    // ��������
    namedWindow("��ԭʼͼ���ڡ�", 1);

    // ����forѭ����ִ������ g_dstImage(i,j) = a*g_srcImage(i,j) + b
    for (int y = 0; y < g_srcImage.rows; y++)
    {
        for (int x = 0; x < g_srcImage.cols; x++)
        {
            for (int c = 0; c < 3; c++)
            {
                // sigmoid �������ƶԱȶ�, ����g_nBrightValue��������
                double t = ((g_srcImage.at<Vec3b>(y, x)[c] - 127) / 225.00) * g_nContrastValue * 0.1;
                g_dstImage.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(g_srcImage.at<Vec3b>(y, x)[c] * ((1.00 / (1.00 + exp(-t))) + 0.5) + g_nBrightValue - 100);
            }
        }
    }

    // ��ʾͼ��
    imshow("��ԭʼͼ���ڡ�", g_srcImage);
    imshow("��Ч��ͼ���ڡ�", g_dstImage);
}