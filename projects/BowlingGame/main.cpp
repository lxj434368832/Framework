// BowlingGame.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "BowlingGame.h"

#include <iostream>

int _tmain(int argc, char *argv[])
{
	BowlingGame bg;

	while (true)
	{
		printf("������һ�б�����������У� -q�˳�����");
		std::string strInput;
		getline(std::cin, strInput);
		unsigned uScore;
		if (bg.CalculateScore(strInput, uScore))
		{
			std::cout << "��ǰ�÷�Ϊ:" << uScore << std::endl;
		}
	}

	return 0;
}

