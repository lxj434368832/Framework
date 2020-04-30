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

	bool ParseFinalFrame(std::vector<std::string> vctInput, std::vector<unsigned>& knocks);

};

