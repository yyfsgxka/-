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
	//��ֵ��
	cv::threshold(srcMat, binaryMat, 0, 255, THRESH_OTSU);
	//����


	Mat elementSizel = getStructuringElement(MORPH_ELLIPSE, Size(2, 5));


	morphologyEx(binaryMat, binaryMat, MORPH_OPEN, elementSizel);
	//ԭͼ����ͼ��kai���㣬����
	//�����ͨ��
	int nComp = cv::connectedComponentsWithStats(binaryMat,
		labelMat,
		statsMat,
		centrMat,
		8,
		CV_32S);

	//�����ͨ����Ϣ

	for (int i = 0; i < nComp; i++)
	{
		//������ͨ���ͳ����Ϣ������stasMat��
		cout << "connected Components NO. " << i << endl;
		cout << "pixels = " << statsMat.at<int>(i, 4) << endl;
		cout << "width = " << statsMat.at<int>(i, 2) << endl;
		cout << "height = " << statsMat.at<int>(i, 3) << endl;
		cout << endl;

	}


	//-1,nComp��������
	cout << "the total of connected Components = " << nComp - 1 << endl;
	//ͨ����ɫ������ͨ��
	//��ʾ��ͼ��
	resultMat = cv::Mat::zeros(srcMat.size(), CV_8UC3);
	std::vector<cv::Vec3b> colors(nComp);
	//����ʹ�ú�ɫ
	colors[0] = cv::Vec3b(0, 0, 0);
	//ʹ���������������randu�����������ɫ
	for (int n = 1; n < nComp; n++)
	{
		colors[n] = cv::Vec3b(rand() / 255, rand() / 255, rand() / 255);
	}

	//���������ذ�����ͨ���Ž�����ɫ
	for (int y = 0; y < srcMat.rows; y++)
	{
		for (int x = 0; x < srcMat.cols; x++)
		{
			int label = labelMat.at<int>(y, x);
			CV_Assert(0 <= label && label <= nComp);

			resultMat.at<cv::Vec3b>(y, x) = colors[label];


		}
	}

	//����bounding box


	for (int i = 1; i < nComp; i++)
	{
		//�ߴ���300
		if (statsMat.at <int>(i, 3) > 300)
		{
			continue;
		}
		else
			count++;

		char num[10];
		sprintf_s(num, "%d", count);

		Rect bndbox;
		//bounding box���Ͻ�����
		bndbox.x = statsMat.at<int>(i, 0);
		bndbox.y = statsMat.at<int>(i, 1);
		//bouding box�Ŀ�ͳ� 
		bndbox.width = statsMat.at<int>(i, 2);
		bndbox.height = statsMat.at<int>(i, 3);
		//����
		rectangle(resultMat, bndbox, CV_RGB(255, 255, 255), 1, 8, 0);
		//��ͨ����
		cv::putText(resultMat, num, Point(bndbox.x, bndbox.y), font_face, 1, cv::Scalar(0, 255, 255), thickness, 8, 0);
	}

	//����ͨ������������ͼƬ��
	//���û����ı�����ز���
	char text[20];
	int length = sprintf_s(text, "%d", count);

	//��ȡ�ı���ĳ���
	cv::Size text_size = cv::getTextSize(text, font_face, font_scale, thickness, &baseline);
	//���ı�����л���
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
