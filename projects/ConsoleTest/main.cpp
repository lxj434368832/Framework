#include <iostream>
#include "CommonDefine.h"
#include "..\..\include\filehandle\FileHandle.h"

int main(int argc, char *argv[])
{
	std::string strPath = "D:/mqw\\MyProgram\\mqwprogwe\\fewf";
	if (0 == FileHandle::CreateDir(strPath.c_str()))
		printf("创建目录成功！\n");
	else
		printf("创建目录失败！\n");

	system("pause");
	return 0;
}