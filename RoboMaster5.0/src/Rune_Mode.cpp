#include "Rune_Mode.hpp" 
#include "ToolKit.hpp"
#include "Identify.hpp"
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <iostream>


using namespace std;
using namespace cv;

/** 定位球的坐标与缩放的尺度 */
//#define DINGWEI_X	240	
//#define DINGWEI_Y	0 
#define DINGWEI_H	200
#define SCALE		3

/** 训练`测试图库的位置 */
string train_path  = "./img/train_image/";
string test_path  = "./img/test_image/";


/** SVM相关 */
Ptr<ml::SVM> svm;

/** 正在打击的数码管 */
static int times = 0;

/** 前一个打击的坐标 */
static int old_x = 0;
static int old_y = 0;

/** 格子在图像中的坐标 */
int coordinate[9][2] ={
	0
};

/** 数码管的数字 */
int number[5] = {
	0
};
/**********************本地函数申明*************************/

/**
 * 训练SVM
 * */
void Train();

/**
 * 格式化图片数据 
 * */
Mat formatImagesForANN(const vector<Mat> &data);

/**********************本地函数申明*************************/

/**
 * 打击神符模式 
 * */
void rune_mode(IplImage* frame,int fd)
{

	/** 寻找定位球的位置 */
	CvPoint ball = identify_ball(frame);
	int DINGWEI_X = ball.x;
	int DINGWEI_Y = ball.y;


	CvRect roi[14] = {

		cvRect(DINGWEI_X-254/SCALE,DINGWEI_Y+DINGWEI_H/SCALE,90/SCALE,122/SCALE),
		cvRect(DINGWEI_X-150/SCALE,DINGWEI_Y+DINGWEI_H/SCALE,90/SCALE,122/SCALE),
		cvRect(DINGWEI_X-45/SCALE,DINGWEI_Y+DINGWEI_H/SCALE,90/SCALE,122/SCALE),
		cvRect(DINGWEI_X+59/SCALE,DINGWEI_Y+DINGWEI_H/SCALE,90/SCALE,122/SCALE),
		cvRect(DINGWEI_X+164/SCALE,DINGWEI_Y+DINGWEI_H/SCALE,90/SCALE,122/SCALE),

		cvRect(DINGWEI_X-510/SCALE,DINGWEI_Y+(DINGWEI_H+122+74)/SCALE,280/SCALE,160/SCALE),
		cvRect(DINGWEI_X-140/SCALE,DINGWEI_Y+(DINGWEI_H+122+74)/SCALE,280/SCALE,160/SCALE),
		cvRect(DINGWEI_X+230/SCALE,DINGWEI_Y+(DINGWEI_H+122+74)/SCALE,280/SCALE,160/SCALE),

		cvRect(DINGWEI_X-510/SCALE,DINGWEI_Y+(DINGWEI_H+122+74+220)/SCALE,280/SCALE,160/SCALE),
		cvRect(DINGWEI_X-140/SCALE,DINGWEI_Y+(DINGWEI_H+122+74+220)/SCALE,280/SCALE,160/SCALE),
		cvRect(DINGWEI_X+230/SCALE,DINGWEI_Y+(DINGWEI_H+122+74+220)/SCALE,280/SCALE,160/SCALE),

		cvRect(DINGWEI_X-510/SCALE,DINGWEI_Y+(DINGWEI_H+122+74+440)/SCALE,280/SCALE,160/SCALE),
		cvRect(DINGWEI_X-140/SCALE,DINGWEI_Y+(DINGWEI_H+122+74+440)/SCALE,280/SCALE,160/SCALE),
		cvRect(DINGWEI_X+230/SCALE,DINGWEI_Y+(DINGWEI_H+122+74+440)/SCALE,280/SCALE,160/SCALE),
	};

	vector<Mat> test_temple;
	test_temple.clear();

	for(int i = 0 ;i < 14; i++)
	{
		//cvRectangle(frame,cvPoint(roi[i].x,roi[i].y),cvPoint(roi[i].x+roi[i].width,roi[i].y+roi[i].height),cvScalar(255,0,0));
		cvSetImageROI(frame,roi[i]);
		IplImage *result = cvCreateImage(cvSize(roi[i].width,roi[i].height),frame->depth,frame->nChannels);
		cvCopy(frame,result);
		//cvShowImage("src_div",result);
		cvResetImageROI(frame);

		IplImage *gray = cvCreateImage(cvGetSize(result),8,1);
		cvCvtColor(result,gray,CV_BGR2GRAY);

		//将图片二值化
		cvThreshold(gray,gray,100,255,CV_THRESH_BINARY);
		IplImage *test = cvCreateImage(cvSize(32,32),8,1);

		//将测试图转化成标准格式 
		cvResize(gray,test,CV_INTER_CUBIC);
		Mat sg = cvarrToMat(test);
		test_temple.push_back(sg);	
	}

	Mat test_data;
	test_data = formatImagesForANN(test_temple);

	Mat testresult(test_data.rows, 1, CV_32FC1);
	svm->predict(test_data, testresult);

	if(times == 0)
	{
		for(int j = 0;j < 5;j++)
		{
			number[j] = testresult.at<uchar>(j,0);
		}
		
		CvPoint cross = identify_cross(frame);
		old_x = cross.x;
		old_y = cross.y;
	}
	if(times == 4)
		times = 0;


	if(number[times] == testresult.at<uchar>(times,0))
	{
		for(int i = 5;i < 14;i++)
		{
			if(testresult.at<uchar>(times,0) == testresult.at<uchar>(i,0))
			{
				//指到指定位置进行打击
				Location location;
				location.horizontal_angle = (int)((old_x - coordinate[i-5][0]) * 50 /480);
				location.pitch_angle = (int)((old_y - coordinate[i-5][1]) * 72 /630 );
				location.flag = 0;

				old_x = coordinate[i-5][0];
				old_y = coordinate[i-5][1];
				times ++;

				send_message(location,fd);
				break;
			}
		}
	}
}


void Train()
{
	cout << "Train_Begin" << endl;
	vector<string> Train_pathnames;
	vector<Mat> Image;
	vector<int> PredictResult;

	Image.clear();
	PredictResult.clear();

	char t[10];
	string s;

	string m;
	for (int i = 0; i < 10; i++)
	{

		sprintf(t, "%d", i);
		s = t;
		for (int j = 1; j <= 20; j++)
		{
			sprintf(t, "%d", j);
			m = t;
			cout << train_path + s + "/" + m +".bmp" << endl;
			Image.push_back(imread(train_path + s + "/" + m +".bmp",0));
			PredictResult.push_back(i);
		}
	}

	Mat data = formatImagesForANN(Image);

	Mat TrainResultTemp(PredictResult);
	Mat responses(data.rows, 1,CV_32S);
	TrainResultTemp.reshape(1, data.rows).convertTo(responses,CV_32S);

	svm = ml::SVM::create();
	svm->setType(ml::SVM::C_SVC);
	svm->setKernel(ml::SVM::LINEAR);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));

	svm->train(data,ml::ROW_SAMPLE, responses);
	cout << "Train_End" << endl;

}

Mat formatImagesForANN(const vector<Mat> &data)
{

	Mat dst(static_cast<int>(data.size()), data[0].rows*data[0].cols, CV_32FC1);
	for (unsigned int i = 0; i < data.size(); i++)
	{
		Mat image_row = data[i].clone().reshape(1, 1);
		Mat row_i = dst.row(i);
		image_row.convertTo(row_i, CV_32FC1); 
	}
	return dst;
}
