#include <iostream>
#include<cmath>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

//定义计算两点距离的函数
double Distance(Point pt1, Point pt2)
{
	return sqrt((pt1.x - pt2.x)*(pt1.x - pt2.x) + (pt1.y - pt2.y)*(pt1.y - pt2.y));
}

int main()
{
	cv::Mat dstMat;
	cv::Mat binMat;
	cv::Mat rgryMat;
	cv::Mat rsrcMat;
	cv::Mat hsvMat;
	cv::Mat resMat;
	cv::Mat hresMat;
	cv::Mat gresMat;
	cv::Mat gryMat = cv::imread("D:\\Files\\topic1.jpg", 0);
	cv::Mat srcMat = cv::imread("D:\\Files\\topic1.jpg");//读取图像
	//检测图像是否读取成功
	if (srcMat.empty())
	{
		std::cout << "Can't read image" << endl;
		return -1;
	}

	//缩放
	cv::resize(gryMat, rgryMat, Size(1280, 960));
	cv::resize(srcMat, rsrcMat, Size(1280, 960));

	rsrcMat.copyTo(dstMat);

	cv::cvtColor(rsrcMat, hsvMat, CV_BGR2HSV);
	cv::inRange(rsrcMat, Scalar(70, 0, 0), Scalar(76.2195, 0, 0), hresMat);
	cv::cvtColor(hresMat, resMat, CV_HSV2BGR);
	cv::cvtColor(hresMat, gresMat, CV_BGR2GRAY);

	//反二值化
	cv::threshold(gresMat, binMat, 100, 255, THRESH_BINARY);

	//寻找连通域
	vector<vector<Point>>contours;
	cv::findContours(binMat, contours, RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	//循环画出结果
	for (int i = 0; i < contours.size(); i++)
	{
		//获得最小外接矩形
		cv::RotatedRect rbox = minAreaRect(contours[i]);
		//计算宽长比
		cv::Point2f vtx[4];
		rbox.points(vtx);
		double bLR = Distance(vtx[0], vtx[1]) / Distance(vtx[1], vtx[2]);
		//筛选
		//if (bLR > 0.9&&bLR < 1.1)
		//{
		for (int j = 0; j < 4; j++)
		{
			cv::line(dstMat, vtx[j], vtx[j < 3 ? j + 1 : 0], Scalar(0, 0, 255), 2, CV_AA);
		}
		//}
	}
	//显示结果
	cv::imshow("src", rsrcMat);
	cv::imshow("dst", dstMat);
	cv::imshow("gry", gresMat);
	cv::imshow("bin", binMat);
	waitKey(0);
}