#ifndef INDENTIFY_H
#define INDENTIFY_H

/**
 * 识别一些特殊的形状 
 * */

#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

#define T_ANGLE_THRE 10
#define T_SIZE_THRE 5

/**
 * 识别十字
 * */
CvPoint identify_cross(IplImage* frame); 

/**
 * 识别裁判系统矩形
 * */
CvPoint identify_rectangle(IplImage* frame);

/**
 * 检测定位球
 * */
CvPoint identify_ball(IplImage* frame);

#endif
