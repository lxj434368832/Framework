#include <iostream>
#include "CommonDefine.h"
#include "..\..\include\filehandle\FileHandle.h"

int main(int argc, char *argv[])
{
	std::string strPath = "D:/mqw\\MyProgram\\mqwprogwe\\fewf";
	if (0 == FileHandle::CreateDir(strPath.c_str()))
		printf("����Ŀ¼�ɹ���\n");
	else
		printf("����Ŀ¼ʧ�ܣ�\n");

	system("pause");
	return 0;
}