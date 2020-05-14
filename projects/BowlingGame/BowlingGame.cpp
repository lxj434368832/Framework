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
	//1进行帧次拆分
	std::vector<std::string> vctFrame = SplitString(strInput, ' ');
	int iCount = vctFrame.size();
	if (iCount < 10)
	{
		std::cerr << "帧次不够，请确定输入。";
		return false;
	}

	//2将普通帧转化为数学模型
	std::vector<std::vector<unsigned>> vctKnocks;
	for (int i = 0; i < 9; i++)
	{
		std::vector<unsigned> vctKnock;
		if (ParseNormalFrame(vctFrame[i], vctKnock))
			vctKnocks.push_back(vctKnock);
		else
			return false;
	}

	//3.将最后帧转化为数学模型
	std::vector<unsigned> vctKnock;
	vctFrame.erase(vctFrame.begin(), vctFrame.begin() + 9);
	if (ParseFinalFrame(vctFrame, vctKnock))
	{
		vctKnocks.push_back(vctKnock);
	}
	else
		return false;

	//4.构建没帧对象
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
		if (c == ch)		//如果为分隔字符则处理添加
		{
			if (!strFrame.empty())
			{
				vctFrames.push_back(strFrame);
				strFrame.clear();
			}
		}
		else if ('X' == ch)	//如果为strike字符，则直接添加
		{
			if (!strFrame.empty())
			{
				vctFrames.push_back(strFrame);
				strFrame.clear();
			}
			
			vctFrames.push_back("X");
		}
		else if (2 == strFrame.size()) //如果字符数为2,则强制添加
		{
			vctFrames.push_back(strFrame);
			strFrame = ch;
		}
		else		//组合每帧的字符
		{
			strFrame += ch;
		}

	}

	if (!strFrame.empty())	//处理最后一帧的字符
	{
		vctFrames.push_back(strFrame);
	}
	return vctFrames;
}

bool BowlingGame::ParseNormalFrame(std::string strFrame, std::vector<unsigned> &knocks)
{
	int iSize = strFrame.size();
	if (1 == iSize && 0 == strFrame.compare("X")) //如果当前帧长度为1，且为strike字符
	{
		knocks.push_back(10);
		return true;
	}
	else if (2 == iSize)		//如果当前帧长度为2
	{
		//处理第0个字符
		unsigned uKnock1 = 0, uKnock2 = 0;
		if ('0' < strFrame[0] && strFrame[0] <= '9')
			uKnock1 = strFrame[0] - '0';
		else if ('-' != strFrame[0])
		{
			std::cerr << "输入的字符：" << strFrame << "无效！" << std::endl;
			return false;
		}

		//处理第1个字符
		if ('/' == strFrame[1])
			uKnock2 = PIN_COUNT - uKnock1;
		else if ('0' < strFrame[1] && strFrame[1] <= '9')
			uKnock2 = strFrame[1] - '0';
		else if ('-' != strFrame[1])
		{
			uKnock2 = 2 * PIN_COUNT;
		}

		//如果相加大于总插脚个数，说明存在非法输入
		if (uKnock1 + uKnock2 > PIN_COUNT)
		{
			std::cerr << "输入的字符：" << strFrame << "无效！" << std::endl;
			return false;
		}
		else
		{
			knocks.push_back(uKnock1);
			knocks.push_back(uKnock2);
			return true;
		}
	}
	else		//当前帧长度错误
	{
		std::cerr << "输入字符：" << strFrame << "无效！" << std::endl;
		return false;
	}
}

bool BowlingGame::ParseFinalFrame(std::vector<std::string> vctFrame, std::vector<unsigned>& knocks)
{
	//1.将剩下的所有字符都归集到最后一帧中
	std::string strFrame;
	for (int i = 0; i < vctFrame.size(); i++)
	{
		strFrame += vctFrame[i];
	}

	//2.分类处理三种状态
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
		std::cerr << "最后帧输入错误，请检查！" << std::endl;
		return false;
	}
}

bool BowlingGame::ParseFinalMissFrame(std::string strFrame, std::vector<unsigned> &knocks)
{
	//miss状态只有2个字符，并且只能是-和数字是合法的字符
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
			std::cerr << "最后帧输入错误，请检查！" << std::endl;
			return false;
		}
	}
	return true;
}

bool BowlingGame::ParseFinalSpareFrame(std::string strFrame, std::vector<unsigned> &knocks)
{
	//1.spare状态的第一个字符只能是-或数字，其余字符非法
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
		std::cerr << "最后帧输入错误，请检查！" << std::endl;
		return false;
	}

	//2.spare状态的第二个字符只能为/,否则非法
	if ('/' == strFrame[1])
	{
		knocks.push_back(PIN_COUNT - knocks[0]);
	}
	else
	{
		std::cerr << "最后帧输入错误，请检查！" << std::endl;
		return false;
	}

	//3.spare状态的第三个字符只能为-、X或数字，否则非法
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
		std::cerr << "最后帧输入错误，请检查！" << std::endl;
		return false;
	}
	return true;
}

bool BowlingGame::ParseFinalStrikeFrame(std::string strFrame, std::vector<unsigned> &knocks)
{
	//1.Strike状态的第一个字符必须为X，否则非法
	if ('X' == strFrame[0])
	{
		knocks.push_back(PIN_COUNT);
	}
	else
	{
		std::cerr << "最后帧输入错误，请检查！" << std::endl;
		return false;
	}

	//2.strike状态的第二和第三个字符只能为-、X、数字，否则非法
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
			std::cerr << "最后帧输入错误，请检查！" << std::endl;
			return false;
		}
	}
	return true;
}
