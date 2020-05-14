#pragma once

#include <string>
#include <vector>

class BowlingGame
{
public:
	BowlingGame();
	~BowlingGame();

	/*************************************************************************
	* function: 计算总得分
	* param strInput:输入的一行保龄球滚动序列
	* param uScore:得分结果
	* return:		成功返回true, 失败返回false
	*************************************************************************/
	bool CalculateScore(std::string strInput, unsigned&	uScore);

private:
	/*************************************************************************
	* function:		拆分出每帧的字符
	* param strInput:输入的一行保龄球滚动序列
	* param c:		分隔字符
	* return:		每帧的字符串
	*************************************************************************/
	std::vector<std::string> SplitString(std::string strInput, char c);

	/*************************************************************************
	* function:		解析普通帧
	* param strFrame:当前帧字符串
	* param knocks:	 解析出的得分值
	* return:		成功返回true, 失败返回false
	*************************************************************************/
	bool ParseNormalFrame(std::string strFrame, std::vector<unsigned> &knocks);

	/*************************************************************************
	* function:		解析最后帧
	* param vctFrame:最后帧字符信息
	* param knocks:	 解析出的得分值
	* return:		成功返回true, 失败返回false
	*************************************************************************/
	bool ParseFinalFrame(std::vector<std::string> vctFrame, std::vector<unsigned>& knocks);

	/*************************************************************************
	* function:		解析最后帧的spare状态
	* param strFrame:当前帧的字符串
	* param knocks:	 解析出的得分值
	* return:		成功返回true, 失败返回false
	*************************************************************************/
	bool ParseFinalSpareFrame(std::string strFrame, std::vector<unsigned> &knocks);

	//接口与上类型
	bool ParseFinalMissFrame(std::string strFrame, std::vector<unsigned> &knocks);

	//接口与上类型
	bool ParseFinalStrikeFrame(std::string strFrame, std::vector<unsigned> &knocks);

};

