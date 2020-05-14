#pragma once

#include <string>
#include <vector>

class BowlingGame
{
public:
	BowlingGame();
	~BowlingGame();

	/*************************************************************************
	* function: �����ܵ÷�
	* param strInput:�����һ�б������������
	* param uScore:�÷ֽ��
	* return:		�ɹ�����true, ʧ�ܷ���false
	*************************************************************************/
	bool CalculateScore(std::string strInput, unsigned&	uScore);

private:
	/*************************************************************************
	* function:		��ֳ�ÿ֡���ַ�
	* param strInput:�����һ�б������������
	* param c:		�ָ��ַ�
	* return:		ÿ֡���ַ���
	*************************************************************************/
	std::vector<std::string> SplitString(std::string strInput, char c);

	/*************************************************************************
	* function:		������ͨ֡
	* param strFrame:��ǰ֡�ַ���
	* param knocks:	 �������ĵ÷�ֵ
	* return:		�ɹ�����true, ʧ�ܷ���false
	*************************************************************************/
	bool ParseNormalFrame(std::string strFrame, std::vector<unsigned> &knocks);

	/*************************************************************************
	* function:		�������֡
	* param vctFrame:���֡�ַ���Ϣ
	* param knocks:	 �������ĵ÷�ֵ
	* return:		�ɹ�����true, ʧ�ܷ���false
	*************************************************************************/
	bool ParseFinalFrame(std::vector<std::string> vctFrame, std::vector<unsigned>& knocks);

	/*************************************************************************
	* function:		�������֡��spare״̬
	* param strFrame:��ǰ֡���ַ���
	* param knocks:	 �������ĵ÷�ֵ
	* return:		�ɹ�����true, ʧ�ܷ���false
	*************************************************************************/
	bool ParseFinalSpareFrame(std::string strFrame, std::vector<unsigned> &knocks);

	//�ӿ���������
	bool ParseFinalMissFrame(std::string strFrame, std::vector<unsigned> &knocks);

	//�ӿ���������
	bool ParseFinalStrikeFrame(std::string strFrame, std::vector<unsigned> &knocks);

};

