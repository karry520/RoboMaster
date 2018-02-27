#include "Identify.hpp"
#include "ToolKit.hpp"

/*******************本地函数申明********************/
double angle(CvPoint* pt1,CvPoint* pt2,CvPoint* pt0);

vector<CvBox2D> armordetect(vector<CvBox2D> vellipse);

void drawbox(CvBox2D box,IplImage* img);

/*******************外部函数定义********************/

/**
 * 识别十字
 * */
CvPoint identify_cross(IplImage* frame)
{

	CvPoint cross = cvPoint(-1,-1);

	CvSeq *pContour = NULL;
	CvMemStorage *pStorage = cvCreateMemStorage(0);

	CvSize pImgSize = cvGetSize(frame);

	IplImage* pRawImg = cvCreateImage(pImgSize, IPL_DEPTH_8U, 3);
	IplImage* pGrayImage = cvCreateImage(pImgSize, IPL_DEPTH_8U, 1);
	IplImage* pRImage = cvCreateImage(pImgSize, IPL_DEPTH_8U, 1);
	IplImage* pGImage = cvCreateImage(pImgSize, IPL_DEPTH_8U, 1);
	iplimage* pbimage = cvcreateimage(pimgsize, ipl_depth_8u, 1);
	iplimage* pbinary = cvcreateimage(pimgsize, ipl_depth_8u, 1);
	iplimage* prlt = cvcreateimage(pimgsize, ipl_depth_8u, 1);


	//亮度修正
	bright_adjust(frame,prawimg,1,-120);

	//通道分离
	cvsplit(prawimg,pbimage,pgimage,primage,0);

	//颜色作差
	getdiff_image(pgimage,primage,pbinary,50);

	//鼓胀,腐蚀处理
	cvdilate(pbinary,pgrayimage,null,3);
	cverode (pgrayimage,prlt,null,1);

	//寻找序列　
	cvfindcontours(prlt, pstorage, &pcontour, sizeof(cvcontour), cv_retr_list, cv_chain_approx_simple);

	cvseq *pcurseq = pcontour;  
	cvseq *poldseq = null;  

	//遍历轮廓序列
	while( pcurseq )  
	{  

		double tmparea = fabs(cvcontourarea(pcurseq));
		//删除掉面积比较小的轮廓
		if( tmparea < 200 )  
		{  
			;	
		}  
		else 
		{  
			//计算点集的最外面（up-right）矩形边界
			CvRect rect1 = cvBoundingRect(pCurSeq,0);  

			//找出完整包含轮廓的最小矩形
			CvBox2D rect = cvMinAreaRect2(pCurSeq);

			//检测轮廓是否为凸包，1为凸包，0为非凸
			int checkcxt = cvCheckContourConvexity( pCurSeq );
			CvSeq* hull = cvConvexHull2( pCurSeq, 0, CV_CLOCKWISE, 0 );//二维凸包
			CvMemStorage* hullStorage = cvCreateMemStorage(0);
			CvSeq* defect = cvConvexityDefects( pCurSeq, hull, hullStorage);//凸包中的缺陷
			cvDrawContours(frame, defect, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), 0, 2, CV_FILLED, cvPoint(0, 0));

			//用cvBoxPoints找出矩形的4个顶点
			CvPoint2D32f rect_pts0[4];
			cvBoxPoints(rect, rect_pts0);
			int npts = 4;
			CvPoint rect_pts[4];
			for (int rp=0; rp<4; rp++)
				rect_pts[rp]= cvPointFrom32f(rect_pts0[rp]);
			CvPoint lpt = cvPoint(frame->width,frame->height);
			CvPoint rpt = cvPoint(0,0);
			CvPoint tpt = cvPoint(frame->width,frame->height);
			CvPoint bpt = cvPoint(0,0);
			//求最左点
			for (int i=0; i<4; i++)
			{
				if (rect_pts[i].x<lpt.x)
					lpt = rect_pts[i];
			}
			for (int i=0; i<4; i++)
			{
				if (rect_pts[i].x>rpt.x)
					rpt = rect_pts[i];
			}
			for (int i=0; i<4; i++)
			{
				if (rect_pts[i].y<tpt.y)
					tpt = rect_pts[i];
			}
			for (int i=0; i<4; i++)
			{
				if (rect_pts[i].y>bpt.y)
					bpt = rect_pts[i];
			}
			//画出对角线
			//cvLine(frame, lpt, rpt, CV_RGB(0,0,255));
			//cvLine(frame, tpt, bpt, CV_RGB(0,0,255));
			//画出中心点
			CvPoint *pt = rect_pts;

			//cvPolyLine(frame, &pt, &npts, 1, 1, CV_RGB(255,0,0), 2);

			CvPoint center = cvPoint(rect.center.x,rect.center.y);
			double s = angle( &lpt, &bpt,&center);
			if (s < 0.05 && abs(rect.size.height - rect.size.width) < 5)
			{
				//cvCircle(frame, cvPoint(rect.center.x, rect.center.y), 3, CV_RGB(255, 255, 0), 3, 8, 0);
				cvPolyLine(frame, &pt, &npts, 1, 1, CV_RGB(255, 0, 0), 2);
				cross.x = rect.center.x;
				cross.y = rect.center.y;
			}

		}  
		pCurSeq=pCurSeq->h_next;
	}

	//释放资源　
	cvReleaseImage(&pRawImg);
	cvReleaseImage(&pGrayImage);
	cvReleaseImage(&pRImage);
	cvReleaseImage(&pGImage);
	cvReleaseImage(&pBImage);
	cvReleaseImage(&pBinary);
	cvReleaseImage(&pRlt);

	cvReleaseMemStorage(&pStorage);

	return cross;
}

/**
 * 识别裁判系统矩形
 * */
CvPoint identify_rectangle(IplImage* frame)
{
	CvPoint rectangle = cvPoint(-1,-1);

	vector<CvBox2D> vEllipse;
	CvBox2D s;
	vector<CvBox2D> vRlt;
	vector<CvBox2D> vArmor;
	CvScalar sl;

	bool bFlag = false;
	CvSeq *pContour = NULL;
	CvMemStorage *pStorage = cvCreateMemStorage(0);

	CvSize pImgSize = cvGetSize(frame);

	IplImage* pRawImg = cvCreateImage(pImgSize, IPL_DEPTH_8U, 3);
	IplImage* pGrayImage = cvCreateImage(pImgSize, IPL_DEPTH_8U, 1);
	IplImage* pRImage = cvCreateImage(pImgSize, IPL_DEPTH_8U, 1);
	IplImage* pGImage = cvCreateImage(pImgSize, IPL_DEPTH_8U, 1);
	IplImage* pBImage = cvCreateImage(pImgSize, IPL_DEPTH_8U, 1);
	IplImage* pBinary = cvCreateImage(pImgSize, IPL_DEPTH_8U, 1);
	IplImage* pRlt = cvCreateImage(pImgSize, IPL_DEPTH_8U, 1);

	bright_adjust(frame, pRawImg, 1, -120);
	cvSplit(pRawImg, pBImage, pGImage, pRImage, 0);
	getdiff_image(pRImage, pGImage, pBinary, 50);

	cvDilate(pBinary, pGrayImage, NULL, 3);
	cvErode(pGrayImage, pRlt, NULL, 1);

	cvFindContours(pRlt, pStorage, &pContour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	for (; pContour != NULL; pContour = pContour->h_next)
	{
		if (pContour->total > 10)
		{
			bFlag = true;
			s = cvFitEllipse2(pContour);
			for (int nI = 0; nI < 5; nI++)
			{
				for (int nJ = 0; nJ < 5; nJ++)
				{
					if (s.center.y - 2 + nJ > 0 && s.center.y - 2 + nJ < 480 && s.center.x - 2 + nI > 0 && s.center.x - 2 + nI <  640)
					{
						sl = cvGet2D(frame, (int)(s.center.y - 2 + nJ), (int)(s.center.x - 2 + nI));
						if (sl.val[0] < 200 || sl.val[1] < 200 || sl.val[2] < 200)
							bFlag = false;
					}
				}
			}
			if (bFlag)
			{
				vEllipse.push_back(s);
				//cvEllipseBox(frame, s, CV_RGB(255, 0, 0), 2, 8, 0);
			}
		}

	}
	vRlt = armordetect(vEllipse);

	for (unsigned int nI = 0; nI < vRlt.size(); nI++)
		drawbox(vRlt[nI], frame);

	vEllipse.clear();
	vRlt.clear();
	vArmor.clear();

	cvReleaseImage(&pRawImg);
	cvReleaseImage(&pGrayImage);
	cvReleaseImage(&pRImage);
	cvReleaseImage(&pGImage);
	cvReleaseImage(&pBImage);
	cvReleaseImage(&pBinary);
	cvReleaseImage(&pRlt);

	cvReleaseMemStorage(&pStorage);

	return rectangle;

}

/**
 * 检测定位球
 * */
CvPoint identify_ball(IplImage* frame)
{
	CvPoint ball = cvPoint(-1,-1);

	return ball;

}

/*******************本地函数定义********************/

double angle(CvPoint* pt1,CvPoint* pt2,CvPoint* pt0)
{
	double dx1 = pt1->x - pt0->x;
	double dy1 = pt1->y - pt0->y;
	double dx2 = pt2->x - pt2->x;
	double dy2 = pt2->y - pt2->y;

	return (dx1 * dx2 + dy1*dy2) / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2*dy2) + 1e-10);
}


vector<CvBox2D> armordetect(vector<CvBox2D> vellipse)
{
	vector<CvBox2D> vRlt;
	CvBox2D Armor;
	int nL, nW;
	double dAngle;
	vRlt.clear();
	if (vellipse.size() < 2)
		return vRlt;
	for (unsigned int nI = 0; nI < vellipse.size() - 1; nI++)
	{
		for (unsigned int nJ = nI + 1; nJ < vellipse.size(); nJ++)
		{
			dAngle = abs(vellipse[nI].angle - vellipse[nJ].angle);
			while (dAngle > 180)
				dAngle -= 180;
			if ((dAngle < T_ANGLE_THRE || 180 - dAngle < T_ANGLE_THRE) && abs(vellipse[nI].size.height - vellipse[nJ].size.height) < (vellipse[nI].size.height + vellipse[nJ].size.height) / T_SIZE_THRE && abs(vellipse[nI].size.width - vellipse[nJ].size.width) < (vellipse[nI].size.width + vellipse[nJ].size.width) / T_SIZE_THRE)
			{
				Armor.center.x = (vellipse[nI].center.x + vellipse[nJ].center.x) / 2;
				Armor.center.y = (vellipse[nI].center.y + vellipse[nJ].center.y) / 2;
				Armor.angle = (vellipse[nI].angle + vellipse[nJ].angle) / 2;
				if (180 - dAngle < T_ANGLE_THRE)
					Armor.angle += 90;
				nL = (vellipse[nI].size.height + vellipse[nJ].size.height) / 2;
				nW = sqrt((vellipse[nI].center.x - vellipse[nJ].center.x) * (vellipse[nI].center.x - vellipse[nJ].center.x) + (vellipse[nI].center.y - vellipse[nJ].center.y) * (vellipse[nI].center.y - vellipse[nJ].center.y));
				if (nL < nW)
				{
					Armor.size.height = nL;
					Armor.size.width = nW;
				}
				else
				{
					Armor.size.height = nW;
					Armor.size.width = nL;
				}
				if( Armor.size.height < Armor.size.width && Armor.size.width < 200 && Armor.size.height > 60)
					vRlt.push_back(Armor);
			}
		}
	}
	return vRlt;
}

void drawbox(CvBox2D box,IplImage* img)
{
	CvPoint2D32f point[4];
	int i;
	for (i = 0; i<4; i++)
	{
		point[i].x = 0;
		point[i].y = 0;
	}
	cvBoxPoints(box, point); 
	CvPoint pt[4];
	for (i = 0; i<4; i++)
	{
		pt[i].x = (int)point[i].x;
		pt[i].y = (int)point[i].y;
	}
	cvLine(img, pt[0], pt[1], CV_RGB(0, 0, 255), 2, 8, 0);
	cvLine(img, pt[1], pt[2], CV_RGB(0, 0, 255), 2, 8, 0);
	cvLine(img, pt[2], pt[3], CV_RGB(0, 0, 255), 2, 8, 0);
	cvLine(img, pt[3], pt[0], CV_RGB(0, 0, 255), 2, 8, 0);
}
