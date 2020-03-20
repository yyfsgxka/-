#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
	cv::Mat binaryMat;
	cv::Mat labelMat;
	cv::Mat statsMat;
	cv::Mat centrMat;
	cv::Mat resultMat;
	cv::Mat srcMat = imread("clips.png", 0);
	int font_face = cv::FONT_HERSHEY_COMPLEX;
	double font_scale = 1.5;
	int thickness = 1;
	int baseline;

	int elementSize = 4;
	int count = 0;
	cout << "------program starts------ " << endl;

	if (srcMat.empty())
	{
		cout << "load image error!" << endl;
	}

	cv::bitwise_not(srcMat, srcMat);
	//二值化
	cv::threshold(srcMat, binaryMat, 0, 255, THRESH_OTSU);
	//定义


	Mat elementSizel = getStructuringElement(MORPH_ELLIPSE, Size(2, 5));


	morphologyEx(binaryMat, binaryMat, MORPH_OPEN, elementSizel);
	//原图，后图，kai运算，参数
	//获得连通域
	int nComp = cv::connectedComponentsWithStats(binaryMat,
		labelMat,
		statsMat,
		centrMat,
		8,
		CV_32S);

	//输出连通域信息

	for (int i = 0; i < nComp; i++)
	{
		//各个连通域的统计信息保存在stasMat中
		cout << "connected Components NO. " << i << endl;
		cout << "pixels = " << statsMat.at<int>(i, 4) << endl;
		cout << "width = " << statsMat.at<int>(i, 2) << endl;
		cout << "height = " << statsMat.at<int>(i, 3) << endl;
		cout << endl;

	}


	//-1,nComp包括背景
	cout << "the total of connected Components = " << nComp - 1 << endl;
	//通过颜色表现连通域
	//显示用图像
	resultMat = cv::Mat::zeros(srcMat.size(), CV_8UC3);
	std::vector<cv::Vec3b> colors(nComp);
	//背景使用黑色
	colors[0] = cv::Vec3b(0, 0, 0);
	//使用随机数产生函数randu，随机产生颜色
	for (int n = 1; n < nComp; n++)
	{
		colors[n] = cv::Vec3b(rand() / 255, rand() / 255, rand() / 255);
	}

	//对所有像素按照连通域编号进行着色
	for (int y = 0; y < srcMat.rows; y++)
	{
		for (int x = 0; x < srcMat.cols; x++)
		{
			int label = labelMat.at<int>(y, x);
			CV_Assert(0 <= label && label <= nComp);

			resultMat.at<cv::Vec3b>(y, x) = colors[label];


		}
	}

	//绘制bounding box


	for (int i = 1; i < nComp; i++)
	{
		//高大于300
		if (statsMat.at <int>(i, 3) > 300)
		{
			continue;
		}
		else
			count++;

		char num[10];
		sprintf_s(num, "%d", count);

		Rect bndbox;
		//bounding box左上角坐标
		bndbox.x = statsMat.at<int>(i, 0);
		bndbox.y = statsMat.at<int>(i, 1);
		//bouding box的宽和长 
		bndbox.width = statsMat.at<int>(i, 2);
		bndbox.height = statsMat.at<int>(i, 3);
		//绘制
		rectangle(resultMat, bndbox, CV_RGB(255, 255, 255), 1, 8, 0);
		//连通域编号
		cv::putText(resultMat, num, Point(bndbox.x, bndbox.y), font_face, 1, cv::Scalar(0, 255, 255), thickness, 8, 0);
	}

	//将连通域数量绘制在图片上
	//设置绘制文本的相关参数
	char text[20];
	int length = sprintf_s(text, "%d", count);

	//获取文本框的长宽
	cv::Size text_size = cv::getTextSize(text, font_face, font_scale, thickness, &baseline);
	//将文本框居中绘制
	cv::Point origin;
	origin.x = 0;
	origin.y = text_size.height;
	cv::putText(resultMat, text, origin, font_face, font_scale, cv::Scalar(0, 255, 255), thickness, 8, 0);

	imshow("binaryMat", binaryMat);
	imshow("results", resultMat);
	imshow("frame", srcMat);
	//moveWindow("frame", 0, 20);
	//moveWindow("binaryMat", srcMat.cols, 20);
	//moveWindow("results", srcMat.cols * 2, 20);
	waitKey(0);
	return 0;
}
