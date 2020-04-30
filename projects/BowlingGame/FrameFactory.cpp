#include "FrameFactory.h"
#include "Frame.h"
#include "FinalFrame.h"
#include <iostream>

IFrame* FrameFactory::BuildFrame(std::vector<unsigned> hits)
{
	if (AbstractFrame::s_uPinCount == hits[0])
		return new StrikeFrame();
	else if (AbstractFrame::s_uPinCount == hits[0] + hits[1])
		return new SpareFrame(hits);
	else
		return new MissFrame(hits);
}

IFrame* FrameFactory::BuildFinalFrame(std::vector<unsigned> hits)
{
	if (AbstractFrame::s_uPinCount == hits[0])
		return new FinalStrikeFrame(hits);
	else if (AbstractFrame::s_uPinCount == hits[0] + hits[1])
		return new SpareFrame(hits);
	else
		return new MissFrame(hits);
}

IFrame* FrameFactory::BuildFrames(std::vector<std::vector<unsigned>> hits)
{
	if (hits.empty())
	{
		std::cerr << "输入数据为空。";
	}

	int iCount = hits.size();
	IFrame *last = BuildFinalFrame(hits[iCount - 1]);
	for (int i = iCount - 2; i >= 0; i--)
	{
		IFrame *fm = BuildFrame(hits[1]);
		fm->SetNextFrame(last);
		last = fm;
	}
	return last;
}
