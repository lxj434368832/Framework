#include "BowlingGame.h"
#include "FrameFactory.h"
#include "Frame.h"

#include <iostream>

BowlingGame::BowlingGame()
{
}


BowlingGame::~BowlingGame()
{
}

bool BowlingGame::CalculateScore(std::string strInput, unsigned& uScore)
{
	//1����֡�β��
	std::vector<std::string> vctFrame = SplitString(strInput, ' ');
	int iCount = vctFrame.size();
	if (iCount < 10)
	{
		std::cerr << "֡�β�������ȷ�����롣";
		return false;
	}

	//2����ͨ֡ת��Ϊ��ѧģ��
	std::vector<std::vector<unsigned>> vctKnocks;
	for (int i = 0; i < 9; i++)
	{
		std::vector<unsigned> vctKnock;
		if (ParseNormalFrame(vctFrame[i], vctKnock))
			vctKnocks.push_back(vctKnock);
		else
			return false;
	}

	//3.�����֡ת��Ϊ��ѧģ��
	std::vector<unsigned> vctKnock;
	vctFrame.erase(vctFrame.begin(), vctFrame.begin() + 9);
	if (ParseFinalFrame(vctFrame, vctKnock))
	{
		vctKnocks.push_back(vctKnock);
	}
	else
		return false;

	//4.����û֡����
	IFrame *fm = FrameFactory::BuildFrames(vctKnocks);
	uScore = fm->TotalScore();
	return true;
}

std::vector<std::string> BowlingGame::SplitString(std::string strInput, char c)
{
	std::vector<std::string> vctFrames;
	std::string strFrame;
	int iCount = strInput.length();
	for (int i = 0; i < iCount; i++)
	{
		char ch = strInput[i];
		if (c == ch)		//���Ϊ�ָ��ַ��������
		{
			if (!strFrame.empty())
			{
				vctFrames.push_back(strFrame);
				strFrame.clear();
			}
		}
		else if ('X' == ch)	//���Ϊstrike�ַ�����ֱ�����
		{
			if (!strFrame.empty())
			{
				vctFrames.push_back(strFrame);
				strFrame.clear();
			}
			
			vctFrames.push_back("X");
		}
		else if (2 == strFrame.size()) //����ַ���Ϊ2,��ǿ�����
		{
			vctFrames.push_back(strFrame);
			strFrame = ch;
		}
		else		//���ÿ֡���ַ�
		{
			strFrame += ch;
		}

	}

	if (!strFrame.empty())	//�������һ֡���ַ�
	{
		vctFrames.push_back(strFrame);
	}
	return vctFrames;
}

bool BowlingGame::ParseNormalFrame(std::string strFrame, std::vector<unsigned> &knocks)
{
	int iSize = strFrame.size();
	if (1 == iSize && 0 == strFrame.compare("X")) //�����ǰ֡����Ϊ1����Ϊstrike�ַ�
	{
		knocks.push_back(10);
		return true;
	}
	else if (2 == iSize)		//�����ǰ֡����Ϊ2
	{
		//�����0���ַ�
		unsigned uKnock1 = 0, uKnock2 = 0;
		if ('0' < strFrame[0] && strFrame[0] <= '9')
			uKnock1 = strFrame[0] - '0';
		else if ('-' != strFrame[0])
		{
			std::cerr << "������ַ���" << strFrame << "��Ч��" << std::endl;
			return false;
		}

		//�����1���ַ�
		if ('/' == strFrame[1])
			uKnock2 = PIN_COUNT - uKnock1;
		else if ('0' < strFrame[1] && strFrame[1] <= '9')
			uKnock2 = strFrame[1] - '0';
		else if ('-' != strFrame[1])
		{
			uKnock2 = 2 * PIN_COUNT;
		}

		//�����Ӵ����ܲ�Ÿ�����˵�����ڷǷ�����
		if (uKnock1 + uKnock2 > PIN_COUNT)
		{
			std::cerr << "������ַ���" << strFrame << "��Ч��" << std::endl;
			return false;
		}
		else
		{
			knocks.push_back(uKnock1);
			knocks.push_back(uKnock2);
			return true;
		}
	}
	else		//��ǰ֡���ȴ���
	{
		std::cerr << "�����ַ���" << strFrame << "��Ч��" << std::endl;
		return false;
	}
}

bool BowlingGame::ParseFinalFrame(std::vector<std::string> vctFrame, std::vector<unsigned>& knocks)
{
	//1.��ʣ�µ������ַ����鼯�����һ֡��
	std::string strFrame;
	for (int i = 0; i < vctFrame.size(); i++)
	{
		strFrame += vctFrame[i];
	}

	//2.���ദ������״̬
	if (2 == strFrame.size())
	{
		return ParseFinalMissFrame(strFrame, knocks);
	}
	else if (3 == strFrame.size())
	{
		if ('/' == strFrame[1])
		{
			return ParseFinalSpareFrame(strFrame, knocks);
		}
		else
		{
			return ParseFinalStrikeFrame(strFrame, knocks);
		}
	}
	else
	{
		std::cerr << "���֡����������飡" << std::endl;
		return false;
	}
}

bool BowlingGame::ParseFinalMissFrame(std::string strFrame, std::vector<unsigned> &knocks)
{
	//miss״ֻ̬��2���ַ�������ֻ����-�������ǺϷ����ַ�
	for (int i = 0; i < 2; i++)
	{
		if ('-' == strFrame[i])
		{
			knocks.push_back(0);
		}
		else if ('0' < strFrame[i] && strFrame[i] <= '9')
		{
			knocks.push_back(strFrame[i] - '0');
		}
		else
		{
			std::cerr << "���֡����������飡" << std::endl;
			return false;
		}
	}
	return true;
}

bool BowlingGame::ParseFinalSpareFrame(std::string strFrame, std::vector<unsigned> &knocks)
{
	//1.spare״̬�ĵ�һ���ַ�ֻ����-�����֣������ַ��Ƿ�
	if ('-' == strFrame[0])
	{
		knocks.push_back(0);
	}
	else if ('0' < strFrame[0] && strFrame[0] <= '9')
	{
		knocks.push_back(strFrame[0] - '0');
	}
	else
	{
		std::cerr << "���֡����������飡" << std::endl;
		return false;
	}

	//2.spare״̬�ĵڶ����ַ�ֻ��Ϊ/,����Ƿ�
	if ('/' == strFrame[1])
	{
		knocks.push_back(PIN_COUNT - knocks[0]);
	}
	else
	{
		std::cerr << "���֡����������飡" << std::endl;
		return false;
	}

	//3.spare״̬�ĵ������ַ�ֻ��Ϊ-��X�����֣�����Ƿ�
	if ('-' == strFrame[2])
	{
		knocks.push_back(0);
	}
	else if ('X' == strFrame[2])
	{
		knocks.push_back(PIN_COUNT);
	}
	else if ('0' < strFrame[2] && strFrame[2] <= '9')
	{
		knocks.push_back(strFrame[2] - '0');
	}
	else
	{
		std::cerr << "���֡����������飡" << std::endl;
		return false;
	}
	return true;
}

bool BowlingGame::ParseFinalStrikeFrame(std::string strFrame, std::vector<unsigned> &knocks)
{
	//1.Strike״̬�ĵ�һ���ַ�����ΪX������Ƿ�
	if ('X' == strFrame[0])
	{
		knocks.push_back(PIN_COUNT);
	}
	else
	{
		std::cerr << "���֡����������飡" << std::endl;
		return false;
	}

	//2.strike״̬�ĵڶ��͵������ַ�ֻ��Ϊ-��X�����֣�����Ƿ�
	for (int i = 1; i < 3; i++)
	{
		if ('-' == strFrame[i])
		{
			knocks.push_back(0);
		}
		else if ('X' == strFrame[i])
		{
			knocks.push_back(PIN_COUNT);
		}
		else if ('0' < strFrame[i] && strFrame[i] <= '9')
		{
			knocks.push_back(strFrame[i] - '0');
		}
		else
		{
			std::cerr << "���֡����������飡" << std::endl;
			return false;
		}
	}
	return true;
}
