#include "FrameFactory.h"
#include "Frame.h"
#include "FinalFrame.h"
#include <iostream>

IFrame* FrameFactory::BuildFrame(std::vector<unsigned> hits)
{
	if (PIN_COUNT == hits[0])
		return new StrikeFrame();
	else if (PIN_COUNT == hits[0] + hits[1])
		return new SpareFrame(hits);
	else
		return new MissFrame(hits);
}

IFrame* FrameFactory::BuildFinalFrame(std::vector<unsigned> hits)
{
	if (PIN_COUNT == hits[0])
		return new FinalStrikeFrame(hits);
	else if (PIN_COUNT == hits[0] + hits[1])
		return new FinalSpareFrame(hits);
	else
		return new FinalMissFrame(hits);
}

IFrame* FrameFactory::BuildFrames(std::vector<std::vector<unsigned>> hits)
{
	if (10 != hits.size())
	{
		std::cerr << "ÊäÈëÖ¡Êý´íÎóÎª:" << hits.size();
		return nullptr;
	}

	int iCount = hits.size();
	IFrame *last = BuildFinalFrame(hits[iCount - 1]);
	for (int i = iCount - 2; i >= 0; i--)
	{
		IFrame *fm = BuildFrame(hits[i]);
		fm->SetNextFrame(last);
		last = fm;
	}
	return last;
}
