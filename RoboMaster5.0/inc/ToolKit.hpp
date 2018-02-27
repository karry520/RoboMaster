#ifndef TOOLKIT_H
#define TOOLKIT_H
/**
 * 程序工具包
 * */
#include <opencv2/opencv.hpp>

#define BEGIN 	1
#define END		0

using namespace cv;

typedef struct locat
{
	float pitch_angle;
	float horizontal_angle;
	int flag;
}Location;

//打开串口
int openserial(int speed);

//接收信息
int receive_message(int fd);

//发送信息
int send_message(Location l,int fd);

void bright_adjust(IplImage* src,IplImage* dst,double dconstrast,double dbright);

void getdiff_image(IplImage* src1,IplImage* src2,IplImage* dst,int threshold);

#endif
