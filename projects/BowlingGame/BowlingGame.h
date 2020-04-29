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
	bool SplitString(std::string strInput, char c);
	bool SplitTheTurns(std::string strInput);
	bool ValidateInput(std::string strInput);
	bool ValidateTurn(std::string strTurn);


private:				
	std::vector<std::string> m_vctTurns;
};

