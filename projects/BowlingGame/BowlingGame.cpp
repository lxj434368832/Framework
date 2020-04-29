#include "BowlingGame.h"
#include <iostream>

BowlingGame::BowlingGame()
{
}


BowlingGame::~BowlingGame()
{
}

bool BowlingGame::CalculateScore(std::string strInput, unsigned& uScore)
{
	std::vector<std::string> vctString = SplitString(strInput, ' ');
	if (vctString.size() < 10)
	{
		std::cerr << "轮次不够，请确定输入。";
		return false;
	}
	std::string strTenth = vctString[9];
	if (strTenth.compare("X"))
	{

	}
	if (12 == vctString.size())
	{
	}
	return true;
}

bool BowlingGame::SplitString(std::string strInput, char c)
{
	m_vctTurns.clear();
	int iCount = strInput.length();
	for (int i = 0; i < iCount; i++)
	{
		std::string str1;
		char ch = strInput[i];
		if ('1' < ch && '9' > ch || 'X' == ch ||
			'/' == ch || '-' == ch)
		{
			str1 += ch;
		}
		else if (c == ch && !str1.empty())
		{
			if (2 == str1.size() && '/' == str1[0])
			{
			}
			m_vctTurns.push_back(str1);
		}
		else
		{
			std::cerr << "输入的字符：" << ch << "不合法！"<<std::endl;
		}
	}
}

bool BowlingGame::SplitTheTurns(std::string strInput)
{
	int idx = strInput.find(' ', 0);
	while (idx != std::string::npos)
	{
		std::string strTurn = strInput.substr(0, idx);

	}
}

bool BowlingGame::ValidateInput(std::string strInput)
{
}

bool BowlingGame::ValidateTurn(std::string strTurn)
{

}
