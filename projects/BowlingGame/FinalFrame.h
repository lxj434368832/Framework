#pragma once

#include <vector>
#include "IFrame.h"

class AbstractFinalFrame : public AbstractFrame
{
public:
	AbstractFinalFrame(std::vector<unsigned> hits);

	unsigned TotalScore() override;

	IFrame* NextFrame() override;

protected:
	std::vector<unsigned>	m_vctHits;
};


class FinalStrikeFrame : public AbstractFinalFrame
{
public:
	FinalStrikeFrame(std::vector<unsigned> hits);
	~FinalStrikeFrame();

	unsigned FrameScore() override;

	unsigned GetSpareBonus() override;

	unsigned GetStrikeBonus() override;
};


class FinalSpareFrame : public AbstractFinalFrame
{
public:
	FinalSpareFrame(std::vector<unsigned> hits);

	unsigned FrameScore() override;
	
	unsigned GetSpareBonus() override;

	unsigned GetStrikeBonus() override;
};


class FinalMissFrame : public AbstractFinalFrame
{
public:
	FinalMissFrame(std::vector<unsigned> hits);

	unsigned FrameScore() override;

	unsigned GetSpareBonus() override;

	unsigned GetStrikeBonus() override;
};