#pragma once

#include <vector>

class IFrame;

class FrameFactory
{
public:
	static IFrame* BuildFrame(std::vector<unsigned> hits);

	static IFrame* BuildFinalFrame(std::vector<unsigned> hits);

	static IFrame* BuildFrames(std::vector<std::vector<unsigned>> hits);
};

