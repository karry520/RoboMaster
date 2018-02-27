#include "Auto_Attack_Mode.hpp"
#include "Standby_Mode.hpp"
#include "Rune_Mode.hpp"
#include "ToolKit.hpp"

/**
 * 程序的入口
 * １．首先对程序参数进行初始化
 * ２．对手写体数字图像进行训练　
 * ３．采集摄像头的数据
 * ４．对每帧图像内容进行分析
 * ５．不断的监听主控芯片的命令
 * */
int main(int argc,char** argv)
{

	/******程序初始化*******/

	//打开raspberry串口
	//int fd = openserial(9600);
	
	//程序模式的设定
	int mode = 2;
	//图像帧数据结构
	IplImage* frame;

	//从摄像头采集数据　
	//CvCapture* capture = cvCreateCameraCapture(0);		
	CvCapture* capture = cvCreateFileCapture("../img/6.avi");		

	//训练图库
	Train();

	while(true)
	{
		//从帧缓冲中取出一帧图片
		frame = cvQueryFrame(capture);

		switch(mode)
		{
			case 1://待命模式
				standby_mode();
				//mode = receive_message(fd);
				break;
			case 2://自动打击模式
				auto_attack_mode(frame,fd);
				//mode = receive_message(fd);
				break;
			case 3://打击神符模式
				rune_mode(frame,fd);
				//mode = receive_message(fd);
				break;
			default://默认模式
				//mode = receive_message(fd);
				break;
		}
	}

	//释放资源
	cvReleaseCapture(&capture);
	return 0;
}
