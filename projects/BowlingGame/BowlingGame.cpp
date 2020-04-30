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
	std::vector<std::string> vctFrame = SplitString(strInput, ' ');
	int iCount = vctFrame.size();
	if (iCount < 10)
	{
		std::cerr << "帧次不够，请确定输入。";
		return false;
	}

	std::vector<std::vector<unsigned>> vctKnocks;

	for (int i = 0; i < 9; i++)
	{
		std::vector<unsigned> vctKnock;
		if (ParseNormalFrame(vctFrame[i], vctKnock))
			vctKnocks.push_back(vctKnock);
		else
			return false;
	}

	std::vector<unsigned> vctKnock;
	vctFrame.erase(vctFrame.begin(), vctFrame.begin() + 8);
	if (ParseFinalFrame(vctFrame, vctKnock))
	{
		vctKnocks.push_back(vctKnock);
		return true;
	}
	else
		return false;

}

std::vector<std::string> BowlingGame::SplitString(std::string strInput, char c)
{
	std::vector<std::string> vctFrames;
	int iCount = strInput.length();
	for (int i = 0; i < iCount; i++)
	{
		std::string strFrame;
		char ch = strInput[i];
		if (c == ch && !strFrame.empty())
		{
			vctFrames.push_back(strFrame);
		}
		else
		{
			strFrame += ch;
		}
	}
}

bool BowlingGame::ParseNormalFrame(std::string strFrame, std::vector<unsigned> &knocks)
{
	int iSize = strFrame.size();
	if (1 == iSize && 0 == strFrame.compare("X"))
	{
		knocks.push_back(10);
	}
	else if (2 == iSize)
	{
		unsigned uKnock1 = 0, uKnock2 = 0;
		if ('0' < strFrame[0] && strFrame[0] <= '9')
			uKnock1 = strFrame[0] - '0';
		else if ('-' != strFrame[0])
		{
			std::cerr << "输入的字符：" << strFrame << "无效！" << std::endl;
			return false;
		}

		if ('/' == strFrame[1])
			uKnock2 = AbstractFrame::s_uPinCount - uKnock1;
		else if ('0' < strFrame[1] && strFrame[1] <= '9')
			uKnock2 = strFrame[1] - '0';
		else if ('-' != strFrame[1])
		{
			uKnock2 = 2 * AbstractFrame::s_uPinCount;
		}

		if (uKnock1 + uKnock2 > AbstractFrame::s_uPinCount)
		{
			std::cerr << "输入的字符：" << strFrame << "无效！" << std::endl;
			return false;
		}
		else
		{
			knocks.push_back(uKnock1);
			knocks.push_back(uKnock2);
		}
	}
	else
	{
		std::cerr << "输入字符：" << strFrame << "无效！" << std::endl;
		return false;
	}
}

bool BowlingGame::ParseFinalFrame(std::vector<std::string> vctInput, std::vector<unsigned>& knocks)
{
	int iSize = vctInput.size();
	if (1 == iSize)
	{
		std::string strFrame = vctInput[0];
		if (2 == strFrame.size())
		{
		}
		else if (3 == strFrame.size())
		{
		}
	}
	else if (3 == iSize)
	{
		for ()
		{
		}
	}
	else
	{
		std::cerr << "帧次错误，请确定输入。" << std::endl;
		return false;
	}
}
