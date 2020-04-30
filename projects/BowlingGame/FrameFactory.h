#pragma once

#include <vector>

__interface IFrame;

class FrameFactory
{
public:
	static IFrame* BuildFrame(std::vector<unsigned> hits);

	static IFrame* BuildFinalFrame(std::vector<unsigned> hits);

	static IFrame* BuildFrames(std::vector<std::vector<unsigned>> hits);
};

