// BowlingGame.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "BowlingGame.h"

#include <iostream>

int main(int argc, char *argv[])
{
	BowlingGame bg;

	while (true)
	{
		printf("\n\n������һ�б�����������У� -q�˳�����\n");
		std::string strInput;
		getline(std::cin, strInput);

		if (0 == strInput.compare("-q"))
			break;

		unsigned uScore;
		if (bg.CalculateScore(strInput, uScore))
		{
			std::cout << "��ǰ�÷�Ϊ:" << uScore << std::endl;
		}
	}

	return 0;
}

