#include "Auto_Attack_Mode.hpp"
#include "ToolKit.hpp"
#include "Identify.hpp"

/**
 * 自主打击模式
 * １．将十字激光找到，并将其在图像中的坐标保存下来
 * ２．将机器人的装甲板找到，并将其在图像中的坐标保存下来
 * ３．将计算后的云台旋转参数发送给主控芯片
 * */
void auto_attack_mode(IplImage* frame,int fd)
{

	//在帧中找十字激光
	CvPoint cross = identify_cross(frame);
	//在帧中找装甲板
	CvPoint rectangle = identify_rectangle(frame);

	Location location;

	//判断是否检测到目标 
	if((cross.x + cross.y ) > 0 && (rectangle.x + rectangle.y) > 0)
	{

		int horizontal_angle = -1;
		int pitch_angle = -1;

		//计算云台旋转的水平角度
		horizontal_angle =(int)((cross.x - rectangle.x) * 50 / 480) ;
		//计算云台旋转的俯仰角度
		pitch_angle = (int)((cross.y - rectangle.y) * 72 / 640);

		location.horizontal_angle = horizontal_angle;
		location.pitch_angle = pitch_angle;
		location.flag = 0;

		//发送数据 
		send_message(location,fd);
		
	}

}
