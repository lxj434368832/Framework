#include "FinalFrame.h"
#include <iostream>


AbstractFinalFrame::AbstractFinalFrame(std::vector<unsigned> hits)
	:m_vctHits(hits)
{

}

unsigned AbstractFinalFrame::TotalScore()
{
	FrameScore();
}

IFrame* AbstractFinalFrame::NextFrame()
{
	std::cerr << "当前帧为最后一个。";
}



FinalStrikeFrame::FinalStrikeFrame(std::vector<unsigned> hits)
	:AbstractFinalFrame(hits)
{

}

FinalStrikeFrame ::~FinalStrikeFrame()
{
}

unsigned FinalStrikeFrame::FrameScore()
{
	return s_uPinCount + m_vctHits[1] + m_vctHits[2];
}

unsigned FinalStrikeFrame::GetSpareBonus()
{
	return s_uPinCount;
}

unsigned FinalStrikeFrame::GetStrikeBonus()
{
	return s_uPinCount + m_vctHits[1];
}



FinalSpareFrame::FinalSpareFrame(std::vector<unsigned> hits)
	: AbstractFinalFrame(hits)
{

}

unsigned FinalSpareFrame::FrameScore()
{
	return s_uPinCount + m_vctHits[2];
}

unsigned FinalSpareFrame::GetSpareBonus()
{
	return m_vctHits[0];
}

unsigned FinalSpareFrame::GetStrikeBonus()
{
	return s_uPinCount;
}



FinalMissFrame::FinalMissFrame(std::vector<unsigned> hits)
	:AbstractFinalFrame(hits)
{

}

unsigned FinalMissFrame::FrameScore()
{
	return m_vctHits[0] + m_vctHits[1];
}

unsigned FinalMissFrame::GetSpareBonus()
{
	return m_vctHits[0];
}

unsigned FinalMissFrame::GetStrikeBonus()
{
	return m_vctHits[0] + m_vctHits[1];
}
