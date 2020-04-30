#pragma once

#include <vector>
#include "IFrame.h"

class AbstractFrame : public IFrame
{
public:
	unsigned TotalScore() override;

	IFrame* NextFrame() override;

	void SetNextFrame(IFrame *fm) override;

	static const unsigned	s_uPinCount = 10;

protected:
	IFrame			*m_pNext = nullptr;
};

class StrikeFrame : public AbstractFrame
{
public:
	StrikeFrame();
	~StrikeFrame();

	unsigned FrameScore() override;

	unsigned GetSpareBonus() override;

	unsigned GetStrikeBonus() override;
};



class SpareFrame : public AbstractFrame
{
public:
	SpareFrame(std::vector<unsigned> hits);
	~SpareFrame();

	unsigned FrameScore() override;

	unsigned GetSpareBonus() override;

	unsigned GetStrikeBonus() override;

private:
	std::vector<unsigned> m_vctHits;
};



class MissFrame : public AbstractFrame
{
public:
	MissFrame(std::vector<unsigned> hits);
	~MissFrame();

	unsigned FrameScore() override;

	unsigned GetSpareBonus() override;

	unsigned GetStrikeBonus() override;

private:
	std::vector<unsigned> m_vctHits;
};
