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
	return s_uPinCount + NextFrame()->GetStrikeBonus();
}

unsigned StrikeFrame::GetSpareBonus()
{
	return s_uPinCount;
}

unsigned StrikeFrame::GetStrikeBonus()
{
	return s_uPinCount + NextFrame()->GetSpareBonus();
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
	return s_uPinCount + NextFrame()->GetSpareBonus();
}

unsigned SpareFrame::GetSpareBonus()
{
	return m_vctHits[0];
}

unsigned SpareFrame::GetStrikeBonus()
{
	return s_uPinCount;
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
