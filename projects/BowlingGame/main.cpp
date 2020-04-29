// BowlingGame.cpp : 定义控制台应用程序的入口点。
//

#include "BowlingGame.h"

#include <iostream>

int _tmain(int argc, char *argv[])
{
	BowlingGame bg;

	while (true)
	{
		printf("请输入一行保龄球滚动序列， -q退出程序。");
		std::string strInput;
		getline(std::cin, strInput);
		unsigned uScore;
		if (bg.CalculateScore(strInput, uScore))
		{
			std::cout << "当前得分为:" << uScore << std::endl;
		}
	}

	return 0;
}

