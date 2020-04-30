#include "FinalFrame.h"
#include <iostream>


AbstractFinalFrame::AbstractFinalFrame(std::vector<unsigned> hits)
	:m_vctHits(hits)
{

}

unsigned AbstractFinalFrame::TotalScore()
{
	return FrameScore();
}

IFrame* AbstractFinalFrame::NextFrame()
{
	std::cerr << "当前帧为最后一个。";
	return nullptr;
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
	return PIN_COUNT + m_vctHits[1] + m_vctHits[2];
}

unsigned FinalStrikeFrame::GetSpareBonus()
{
	return PIN_COUNT;
}

unsigned FinalStrikeFrame::GetStrikeBonus()
{
	return PIN_COUNT + m_vctHits[1];
}



FinalSpareFrame::FinalSpareFrame(std::vector<unsigned> hits)
	: AbstractFinalFrame(hits)
{

}

unsigned FinalSpareFrame::FrameScore()
{
	return PIN_COUNT + m_vctHits[2];
}

unsigned FinalSpareFrame::GetSpareBonus()
{
	return m_vctHits[0];
}

unsigned FinalSpareFrame::GetStrikeBonus()
{
	return PIN_COUNT;
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
