#include "Frame.h"

unsigned AbstractFrame::TotalScore()
{
	return FrameScore() + NextFrame()->TotalScore();
}

IFrame* AbstractFrame::NextFrame()
{
	return m_pNext;
}

void AbstractFrame::SetNextFrame(IFrame *fm)
{
	m_pNext = fm;
}



StrikeFrame::StrikeFrame()
{
}

StrikeFrame::~StrikeFrame()
{
}

unsigned StrikeFrame::FrameScore()
{
	return PIN_COUNT + NextFrame()->GetStrikeBonus();
}

unsigned StrikeFrame::GetSpareBonus()
{
	return PIN_COUNT;
}

unsigned StrikeFrame::GetStrikeBonus()
{
	return PIN_COUNT + NextFrame()->GetSpareBonus();
}



SpareFrame::SpareFrame(std::vector<unsigned> hits)
	:m_vctHits(hits)
{
}

SpareFrame::~SpareFrame()
{
}

unsigned SpareFrame::FrameScore()
{
	return PIN_COUNT + NextFrame()->GetSpareBonus();
}

unsigned SpareFrame::GetSpareBonus()
{
	return m_vctHits[0];
}

unsigned SpareFrame::GetStrikeBonus()
{
	return PIN_COUNT;
}


MissFrame::MissFrame(std::vector<unsigned> hits)
	:m_vctHits(hits)
{
}

MissFrame::~MissFrame()
{
}

unsigned MissFrame::FrameScore()
{
	return m_vctHits[0] + m_vctHits[1];
}

unsigned MissFrame::GetSpareBonus()
{
	return m_vctHits[0];
}

unsigned MissFrame::GetStrikeBonus()
{
	return m_vctHits[0] + m_vctHits[1];
}
