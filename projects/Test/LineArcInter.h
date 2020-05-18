#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//运行时坐标结构体
typedef struct CdtNow
{
	float xCdt;
	float yCdt;
}CdtNow_Struct;

//直线的逐点比较法的直线插补的坐标的结构体
typedef struct Line_Inter
{
	float xCdt_Sat;             //起点坐标
	float yCdt_Sat;
	float xCdt_Tgt;             //终点坐标
	float yCdt_Tgt;
	float pulseEqu;      //脉冲当量
}Line_Inter_Struct;

//圆弧的逐点比较法的直线插补的坐标的结构体
typedef struct Arc_Inter
{
	float xCdt_Sat;             //起点坐标
	float yCdt_Sat;
	float xCdt_Tgt;             //终点坐标
	float yCdt_Tgt;
	float radius;               //半径
	float pulseEqu;      //脉冲当量
}Arc_Inter_Struct;

//函数函数名：Line_point_by_point_comparison_Interpolation的测试的数据
//Line_Inter_Struct lineStructTest = { 0, 0, -6, -4, 1 };
Line_Inter_Struct lineStructTest = { 1, 2, 7, 9, 1 };
Arc_Inter_Struct  arcStructTest = { 10, 0, 6, 8, 10, 1 };

void Line_point_by_point_comparison_Interpolation(Line_Inter_Struct lineInterStr);
void Arc_point_by_point_comparison_Interpolation(Arc_Inter_Struct arcInterStr);

int TestLineInter()
{
	Line_point_by_point_comparison_Interpolation(lineStructTest);
	//    Arc_point_by_point_comparison_Interpolation(arcStructTest);
	return 0;
}

/*
* 参考书目：《机床数控技术》 主编：梅雪松 P96
* 日期：2019/04/07
* 作者：Twsa Liu
* 函数名：Line_point_by_point_comparison_Interpolation
* 功能描述：直线的逐点比较法的插补算法
* 参数：lineInterStruct，直线插补结构体数据
* 返回值：运行时的坐标的结构体
*/
void Line_point_by_point_comparison_Interpolation(Line_Inter_Struct lineInterStr)
{
	//定义和初始化运行时坐标
	CdtNow_Struct cdtNow = { lineInterStr.xCdt_Sat, lineInterStr.yCdt_Sat };
	float devt = cdtNow.xCdt * lineInterStr.yCdt_Tgt - cdtNow.yCdt * lineInterStr.xCdt_Tgt; //直线插补的偏差函数
	//float devt = 0;
	float endJudge = fabs(lineInterStr.xCdt_Tgt - cdtNow.xCdt) + fabs(lineInterStr.yCdt_Tgt - cdtNow.yCdt);

	printf("起点坐标：%f  %f\n", cdtNow.xCdt, cdtNow.yCdt);
	printf("终点坐标：%f  %f\n", lineInterStr.xCdt_Tgt, lineInterStr.yCdt_Tgt);

	int iNum = 1;
	while (endJudge > 0) //z终点判断
	{
		//得到的新的坐标值
		if (devt >= 0)
		{
			//沿Xe方向走一步
			if (lineInterStr.xCdt_Tgt >= 0)
				cdtNow.xCdt += lineInterStr.pulseEqu;
			else
				cdtNow.xCdt -= lineInterStr.pulseEqu;
			//yCdtnow = yCdtnow;
			devt -= fabs(lineInterStr.yCdt_Tgt);
		}
		else
		{
			//沿Ye方向走一步
			//xCdtnow=xCdtnow;
			if (lineInterStr.yCdt_Tgt >= 0)
				cdtNow.yCdt += lineInterStr.pulseEqu;
			else
				cdtNow.yCdt -= lineInterStr.pulseEqu;
			devt += fabs(lineInterStr.xCdt_Tgt);
		}
		endJudge -= lineInterStr.pulseEqu;

		printf("%d:	%f  %f\n", iNum++, cdtNow.xCdt, cdtNow.yCdt);
	}
}

/*
* 参考书目：《机床数控技术》 主编：梅雪松 P94
* 日期：2019/04/07
* 作者：Twsa Liu
* 函数名：Arc_point_by_point_comparison_Interpolation
* 功能描述：圆弧的逐点比较法的插补算法
* 参数：arcInterStr，圆弧擦布的结构体结构体数据
* 返回值：运行时的坐标的结构体
*/
void  Arc_point_by_point_comparison_Interpolation(Arc_Inter_Struct arcInterStr)
{
	//定义和初始化运行时坐标
	CdtNow_Struct cdtNow = { arcInterStr.xCdt_Sat, arcInterStr.yCdt_Sat };
	//终点判断计数设置
	float endJudge = fabs(arcInterStr.xCdt_Tgt - arcInterStr.xCdt_Sat) + fabs(arcInterStr.yCdt_Tgt - arcInterStr.yCdt_Sat);
	//圆弧插补的偏差函数
	float devt = cdtNow.xCdt*cdtNow.xCdt + cdtNow.yCdt*cdtNow.yCdt\
		- arcInterStr.radius*arcInterStr.radius;   //Fi=Xi^2+Yi^2-R^2
	while (endJudge > 0)   //终点判断
	{
		//        //顺圆插补,测试了第一个象限，其他象限还未测试
		//        if(devt>=0){
		//            devt+=arcInterStr.pulseEqu*arcInterStr.pulseEqu-2*cdtNow.yCdt*arcInterStr.pulseEqu;  //新的偏差计算
		//            //cdtNow.xCdt=cdtNow.xCdt;
		//            cdtNow.yCdt-=arcInterStr.pulseEqu;
		//        }else{
		//            devt+=arcInterStr.pulseEqu*arcInterStr.pulseEqu+2*cdtNow.xCdt*arcInterStr.pulseEqu;
		//            cdtNow.xCdt+=arcInterStr.pulseEqu;
		//            //cdtNow.yCdt=cdtNow.yCdt;
		//        }
		//逆圆插补
		if (devt >= 0){
			devt += arcInterStr.pulseEqu*arcInterStr.pulseEqu - 2 * cdtNow.xCdt*arcInterStr.pulseEqu;  //新的偏差计算
			//沿-Xe方向走一步
			if (arcInterStr.xCdt_Tgt >= 0)
				cdtNow.xCdt -= arcInterStr.pulseEqu;
			else
				cdtNow.xCdt += arcInterStr.pulseEqu;
			//cdtNow.yCdt=cdtNow.yCdt;
		}
		else{
			devt += arcInterStr.pulseEqu*arcInterStr.pulseEqu + 2 * cdtNow.yCdt*arcInterStr.pulseEqu;  //计算新的偏差
			//沿Ye方向走一步
			//xCdtnow=xCdtnow;
			if (arcInterStr.yCdt_Tgt >= 0)
				cdtNow.yCdt += arcInterStr.pulseEqu;
			else
				cdtNow.yCdt -= arcInterStr.pulseEqu;
			//cdtNow.xCdt=cdtNow.xCdt;
		}
		endJudge -= arcInterStr.pulseEqu;
	} //最后应该是生成文档
}


#include <cmath>
#include <iostream>
using namespace std;

int TestLineInter1()
{
	int xs = 0, ys = 0;
	int xe = 11, ye = -4;
	//xs与ys为起点坐标，xe与ye为终点坐标

	//cout << "请输入起点坐标：" << endl;
	//cin >> xs >> ys;
	//cout << "请输入终点坐标：" << endl;
	//cin >> xe >> ye;
	//cout << "------TestLineInter1------" << endl;

	int iCounter, xi, yi, iSlope;
	//iCounter为插补总次数，xi与yi为插补过程点，XieLvChaZhi为斜率。

	iCounter = abs((xe - xs)) + abs((ye - ys));

	xi = xs;
	yi = ys;
	//以上两行代码实现将过程点放在起点坐标上面。

	for (int i = 0; i < iCounter; i++)
	{
		iSlope = (xe - xs)*(yi - ys) - (xi - xs)*(ye - ys);

		//Y轴正负方向走向
		if (xe - xs == 0)
		{
			if (ye - ys > 0)
			{
				yi++;
			}
			else
			{
				yi--;
			}
		}

		//X轴正负方向走向
		if (ye - ys == 0)
		{
			if (xe - xs > 0)
			{
				xi++;
			}
			else
			{
				xi--;
			}
		}

		//第一象限走向
		if ((xe - xs > 0) && (ye - ys > 0))
		{
			if (iSlope >= 0)
			{
				xi++;
			}
			else
			{
				yi++;
			}
		}

		//第二象限走向
		if ((xe - xs < 0) && (ye - ys>0))
		{
			if (iSlope <= 0)
			{
				xi--;
			}
			else
			{
				yi++;
			}
		}

		//第三象限走向
		if ((xe - xs < 0) && (ye - ys < 0))
		{
			if (iSlope <= 0)
			{
				yi--;
			}
			else
			{
				xi--;
			}
		}

		//第四象限走向
		if ((xe - xs > 0) && (ye - ys < 0))
		{
			if (iSlope >= 0)
			{
				yi--;
			}
			else
			{
				xi++;
			}
		}

		cout << xi << "," << yi << endl;
	}
	return 0;
}


#include <math.h>//调用绝对值函数
#include <iostream>
using namespace std;

void TestLineInter2()
{
	int Counter, xe, ye, x0, y0, xi = 0, yi = 0, FM, F;
	int i = 1;
	//cout << "######################" << endl;
	//cout << "#欢迎来到直线插补程序#" << endl;
	//cout << "######################" << endl << endl;
	//cout << "请输入起点坐标：" << endl;
	//cin >> x0 >> y0;
	//cout << "请输入终点坐标：" << endl;
	//cin >> xe >> ye;

	x0 = -1908, y0 = -3138;
	xe = -1895, ye = -3139;

	cout << endl << "*****************************************************************************" << endl;
	Counter = abs((xe - x0)) + abs((ye - y0));

	xi = x0;//x,y分别存放实时的坐标
	yi = y0;//
	//FM =xe*yi-xi*ye;

	while (Counter > 0)
	{
		FM = (xe - x0)*(yi - y0) - (xi - x0)*(ye - y0);
		if (xe - x0 == 0)
		{
		}
		else if ((ye - y0) / (xe - x0) < 0)//调整斜率
			FM = FM*(-1);

		if (FM >= 0)
		{
			//F=FM-ye;
			if (xe - x0 == 0)
			{
				if (ye - y0 > 0)
					yi = yi + 1; //Y正向走步程序
				else if (ye - y0 < 0)
					yi--;//Y负向走步程序
			}
			else if (xe - x0 > 0)
				xi = xi + 1; //X正向走步程序
			else if (xe - x0 < 0)
				xi--;//X负向走步程序
		}
		else
		{
			//F=FM+xe;
			if (ye - y0 > 0)
				yi = yi + 1; //Y正向走步程序
			else if (ye - y0 < 0)
				yi--;//Y负向走步程序
		}
		Counter--;
		i++;

		cout << i << ":  " << xi << "," << yi << "  \tFM=" << FM << "  " << endl;
	}
	cout << endl << "*****************************************************************************" << endl << endl;
}

