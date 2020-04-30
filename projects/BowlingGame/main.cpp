// BowlingGame.cpp : 定义控制台应用程序的入口点。
//

#include "BowlingGame.h"

#include <iostream>

int main(int argc, char *argv[])
{
	BowlingGame bg;

	while (true)
	{
		printf("\n\n请输入一行保龄球滚动序列， -q退出程序。\n");
		std::string strInput;
		getline(std::cin, strInput);

		if (0 == strInput.compare("-q"))
			break;

		unsigned uScore;
		if (bg.CalculateScore(strInput, uScore))
		{
			std::cout << "当前得分为:" << uScore << std::endl;
		}
	}

	return 0;
}

