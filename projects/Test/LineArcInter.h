#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//����ʱ����ṹ��
typedef struct CdtNow
{
	float xCdt;
	float yCdt;
}CdtNow_Struct;

//ֱ�ߵ����ȽϷ���ֱ�߲岹������Ľṹ��
typedef struct Line_Inter
{
	float xCdt_Sat;             //�������
	float yCdt_Sat;
	float xCdt_Tgt;             //�յ�����
	float yCdt_Tgt;
	float pulseEqu;      //���嵱��
}Line_Inter_Struct;

//Բ�������ȽϷ���ֱ�߲岹������Ľṹ��
typedef struct Arc_Inter
{
	float xCdt_Sat;             //�������
	float yCdt_Sat;
	float xCdt_Tgt;             //�յ�����
	float yCdt_Tgt;
	float radius;               //�뾶
	float pulseEqu;      //���嵱��
}Arc_Inter_Struct;

//������������Line_point_by_point_comparison_Interpolation�Ĳ��Ե�����
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
* �ο���Ŀ�����������ؼ����� ���ࣺ÷ѩ�� P96
* ���ڣ�2019/04/07
* ���ߣ�Twsa Liu
* ��������Line_point_by_point_comparison_Interpolation
* ����������ֱ�ߵ����ȽϷ��Ĳ岹�㷨
* ������lineInterStruct��ֱ�߲岹�ṹ������
* ����ֵ������ʱ������Ľṹ��
*/
void Line_point_by_point_comparison_Interpolation(Line_Inter_Struct lineInterStr)
{
	//����ͳ�ʼ������ʱ����
	CdtNow_Struct cdtNow = { lineInterStr.xCdt_Sat, lineInterStr.yCdt_Sat };
	float devt = cdtNow.xCdt * lineInterStr.yCdt_Tgt - cdtNow.yCdt * lineInterStr.xCdt_Tgt; //ֱ�߲岹��ƫ���
	//float devt = 0;
	float endJudge = fabs(lineInterStr.xCdt_Tgt - cdtNow.xCdt) + fabs(lineInterStr.yCdt_Tgt - cdtNow.yCdt);

	printf("������꣺%f  %f\n", cdtNow.xCdt, cdtNow.yCdt);
	printf("�յ����꣺%f  %f\n", lineInterStr.xCdt_Tgt, lineInterStr.yCdt_Tgt);

	int iNum = 1;
	while (endJudge > 0) //z�յ��ж�
	{
		//�õ����µ�����ֵ
		if (devt >= 0)
		{
			//��Xe������һ��
			if (lineInterStr.xCdt_Tgt >= 0)
				cdtNow.xCdt += lineInterStr.pulseEqu;
			else
				cdtNow.xCdt -= lineInterStr.pulseEqu;
			//yCdtnow = yCdtnow;
			devt -= fabs(lineInterStr.yCdt_Tgt);
		}
		else
		{
			//��Ye������һ��
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
* �ο���Ŀ�����������ؼ����� ���ࣺ÷ѩ�� P94
* ���ڣ�2019/04/07
* ���ߣ�Twsa Liu
* ��������Arc_point_by_point_comparison_Interpolation
* ����������Բ�������ȽϷ��Ĳ岹�㷨
* ������arcInterStr��Բ�������Ľṹ��ṹ������
* ����ֵ������ʱ������Ľṹ��
*/
void  Arc_point_by_point_comparison_Interpolation(Arc_Inter_Struct arcInterStr)
{
	//����ͳ�ʼ������ʱ����
	CdtNow_Struct cdtNow = { arcInterStr.xCdt_Sat, arcInterStr.yCdt_Sat };
	//�յ��жϼ�������
	float endJudge = fabs(arcInterStr.xCdt_Tgt - arcInterStr.xCdt_Sat) + fabs(arcInterStr.yCdt_Tgt - arcInterStr.yCdt_Sat);
	//Բ���岹��ƫ���
	float devt = cdtNow.xCdt*cdtNow.xCdt + cdtNow.yCdt*cdtNow.yCdt\
		- arcInterStr.radius*arcInterStr.radius;   //Fi=Xi^2+Yi^2-R^2
	while (endJudge > 0)   //�յ��ж�
	{
		//        //˳Բ�岹,�����˵�һ�����ޣ��������޻�δ����
		//        if(devt>=0){
		//            devt+=arcInterStr.pulseEqu*arcInterStr.pulseEqu-2*cdtNow.yCdt*arcInterStr.pulseEqu;  //�µ�ƫ�����
		//            //cdtNow.xCdt=cdtNow.xCdt;
		//            cdtNow.yCdt-=arcInterStr.pulseEqu;
		//        }else{
		//            devt+=arcInterStr.pulseEqu*arcInterStr.pulseEqu+2*cdtNow.xCdt*arcInterStr.pulseEqu;
		//            cdtNow.xCdt+=arcInterStr.pulseEqu;
		//            //cdtNow.yCdt=cdtNow.yCdt;
		//        }
		//��Բ�岹
		if (devt >= 0){
			devt += arcInterStr.pulseEqu*arcInterStr.pulseEqu - 2 * cdtNow.xCdt*arcInterStr.pulseEqu;  //�µ�ƫ�����
			//��-Xe������һ��
			if (arcInterStr.xCdt_Tgt >= 0)
				cdtNow.xCdt -= arcInterStr.pulseEqu;
			else
				cdtNow.xCdt += arcInterStr.pulseEqu;
			//cdtNow.yCdt=cdtNow.yCdt;
		}
		else{
			devt += arcInterStr.pulseEqu*arcInterStr.pulseEqu + 2 * cdtNow.yCdt*arcInterStr.pulseEqu;  //�����µ�ƫ��
			//��Ye������һ��
			//xCdtnow=xCdtnow;
			if (arcInterStr.yCdt_Tgt >= 0)
				cdtNow.yCdt += arcInterStr.pulseEqu;
			else
				cdtNow.yCdt -= arcInterStr.pulseEqu;
			//cdtNow.xCdt=cdtNow.xCdt;
		}
		endJudge -= arcInterStr.pulseEqu;
	} //���Ӧ���������ĵ�
}


#include <cmath>
#include <iostream>
using namespace std;

int TestLineInter1()
{
	int xs = 0, ys = 0;
	int xe = 11, ye = -4;
	//xs��ysΪ������꣬xe��yeΪ�յ�����

	//cout << "������������꣺" << endl;
	//cin >> xs >> ys;
	//cout << "�������յ����꣺" << endl;
	//cin >> xe >> ye;
	//cout << "------TestLineInter1------" << endl;

	int iCounter, xi, yi, iSlope;
	//iCounterΪ�岹�ܴ�����xi��yiΪ�岹���̵㣬XieLvChaZhiΪб�ʡ�

	iCounter = abs((xe - xs)) + abs((ye - ys));

	xi = xs;
	yi = ys;
	//�������д���ʵ�ֽ����̵��������������档

	for (int i = 0; i < iCounter; i++)
	{
		iSlope = (xe - xs)*(yi - ys) - (xi - xs)*(ye - ys);

		//Y��������������
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

		//X��������������
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

		//��һ��������
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

		//�ڶ���������
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

		//������������
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

		//������������
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


#include <math.h>//���þ���ֵ����
#include <iostream>
using namespace std;

void TestLineInter2()
{
	int Counter, xe, ye, x0, y0, xi = 0, yi = 0, FM, F;
	int i = 1;
	//cout << "######################" << endl;
	//cout << "#��ӭ����ֱ�߲岹����#" << endl;
	//cout << "######################" << endl << endl;
	//cout << "������������꣺" << endl;
	//cin >> x0 >> y0;
	//cout << "�������յ����꣺" << endl;
	//cin >> xe >> ye;

	x0 = -1908, y0 = -3138;
	xe = -1895, ye = -3139;

	cout << endl << "*****************************************************************************" << endl;
	Counter = abs((xe - x0)) + abs((ye - y0));

	xi = x0;//x,y�ֱ���ʵʱ������
	yi = y0;//
	//FM =xe*yi-xi*ye;

	while (Counter > 0)
	{
		FM = (xe - x0)*(yi - y0) - (xi - x0)*(ye - y0);
		if (xe - x0 == 0)
		{
		}
		else if ((ye - y0) / (xe - x0) < 0)//����б��
			FM = FM*(-1);

		if (FM >= 0)
		{
			//F=FM-ye;
			if (xe - x0 == 0)
			{
				if (ye - y0 > 0)
					yi = yi + 1; //Y�����߲�����
				else if (ye - y0 < 0)
					yi--;//Y�����߲�����
			}
			else if (xe - x0 > 0)
				xi = xi + 1; //X�����߲�����
			else if (xe - x0 < 0)
				xi--;//X�����߲�����
		}
		else
		{
			//F=FM+xe;
			if (ye - y0 > 0)
				yi = yi + 1; //Y�����߲�����
			else if (ye - y0 < 0)
				yi--;//Y�����߲�����
		}
		Counter--;
		i++;

		cout << i << ":  " << xi << "," << yi << "  \tFM=" << FM << "  " << endl;
	}
	cout << endl << "*****************************************************************************" << endl << endl;
}

