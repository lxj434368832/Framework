#pragma once

#include <string>
#include <vector>

class BowlingGame
{
public:
	BowlingGame();
	~BowlingGame();

	bool CalculateScore(std::string strInput, unsigned&	uScore);

private:
	std::vector<std::string> SplitString(std::string strInput, char c);

	bool ParseNormalFrame(std::string strFrame, std::vector<unsigned> &knocks);

	bool ParseFinalFrame(std::vector<std::string> vctFrame, std::vector<unsigned>& knocks);

	bool ParseFinalSpareFrame(std::string strFrame, std::vector<unsigned> &knocks);

	bool ParseFinalMissFrame(std::string strFrame, std::vector<unsigned> &knocks);

	bool ParseFinalStrikeFrame(std::string strFrame, std::vector<unsigned> &knocks);

};

