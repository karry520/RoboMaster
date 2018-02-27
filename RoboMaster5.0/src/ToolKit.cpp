#include "ToolKit.hpp"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringSerial.h>

int intemp = 0;
char str[10] = {0};

/**
 * 打开串口
 * */
int openserial(int speed)
{
	int fd;
	if ((fd = serialOpen ("/dev/ttyAMA0", speed)) < 0)
	{
		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		return -1 ;
	}
	return fd;
}

//浮点型转字符串
char* floattostring(float a,char *s)
{
	float i;
	char zs[20] = {0};
	char xs[12] = {0};
	int j,k=0,f=0;
	j =(int)a;
	i = a;
	if(a==0.0)
	{
		return 0;
	}
	zs[0] = j/100+'0';
	zs[1] = j%100/10+'0';
	zs[2] = j%10+'0';
	zs[3] = '.';
	while(k<5)
	{
		if(i-j==0)
		{
			xs[k] = '0';
		}
		else
		{
			i = i*10;
			j =(int)i;
			xs[k] = j%10+'0';
		}
		k++;
	}

	strcat(zs,xs);
	strcpy(s,zs);
	return s;
}

// 整型转字符串
void inttostring(int a)
{
	if(a==0)
	{
		return;
	}
	else
	{
		inttostring(a/10);
		str[intemp] = a%10+'0';
		intemp++;
	}
}
//location 转字符串
void locationtostring(Location l,char *s)
{
	char s1[10] = {0};
	char s2[10] = {0};
	floattostring(l.pitch_angle,s1);
	strcat(s,s1);
	strcat(s,",");
	floattostring(l.horizontal_angle,s2);
	strcat(s,s2);
	strcat(s,",");
	inttostring(l.flag);
	str[intemp] = 0;
	intemp = 0;
	strcat(s,str);
	strcat(s,",");
	memset(str,0,10);
	printf("turn ok = %s\n",s);
}

/**
 * 接收信息
 * */
int receive_message(int fd)
{
	char i=0;
	char a[3];
	while(i<3)
	{
		a[i++] = serialGetchar(fd);
	}
	if(a[0]=='y')
		return BEGIN;
	else if(a[0]='n')
		return END;
}

int Send_Location(int fd,char *a)
{
	int i = 0;
	while(a[i]!=0)//(i=serialGetchar(fd))!='n')
	{
		serialPutchar(fd,a[i]);
		i++;
	}
}

/**
 * 发送信息
 * */
int send_message(Location  l,int fd)
{
	char ldata[100] = {0}; 	
	locationtostring(l,ldata);
	Send_Location(fd,ldata);
}


/**
 * 修正明度
 * */
void bright_adjust(IplImage* src,IplImage* dst,double dconstrast,double dbright)
{
	int nVal;

	unsigned char* SrcData = (unsigned char*)src->imageData;
	unsigned char* DstData = (unsigned char*)dst->imageData;

	int step = src->widthStep / sizeof(unsigned char) / 3;

	for (int nI = 0; nI<src->height; nI++)
	{
		for (int nJ = 0; nJ <src->width; nJ++)
		{
			for (int nK = 0; nK < 3; nK++)
			{
				nVal = (int)(dconstrast * SrcData[(nI*step + nJ) * 3 + nK] + dbright);
				if (nVal < 0)
					nVal = 0;
				if (nVal > 255)
					nVal = 255;
				DstData[(nI*step + nJ) * 3 + nK] = nVal;
			}
		}
	}
}

/**
 * 两张图片作差
 * */
void getdiff_image(IplImage* src1,IplImage* src2,IplImage* dst,int threshold)
{
	unsigned char* SrcData1 = (unsigned char*)src1->imageData;
	unsigned char* SrcData2 = (unsigned char*)src2->imageData;
	unsigned char* DstData  = (unsigned char*)dst ->imageData;

	int step = src1->widthStep / sizeof(unsigned char);
	for (int nI = 0; nI<src1->height; nI++)
	{
		for (int nJ = 0; nJ <src1->width; nJ++)
		{
			if (SrcData1[nI*step + nJ] - SrcData2[nI*step + nJ]> threshold)
			{
				DstData[nI*step + nJ] = 255;
			}
			else
			{
				DstData[nI*step + nJ] = 0;
			}
		}
	}

}
