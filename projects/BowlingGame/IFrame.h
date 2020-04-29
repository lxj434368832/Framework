#pragma once

class IFrame
{
public:
	//当前frame以及之后的总分
	virtual unsigned TotalScore() = 0;

	//当前帧得分
	virtual unsigned FrameScore() = 0;

	//当前帧可贡献的spare奖励
	virtual unsigned GetSpareBonus() = 0;

	//当前帧可贡献的strike奖励
	virtual unsigned GetStrikeBonus() = 0;

	IFrame*	NextFrame();

	void SetNextFrame(IFrame *frame);

protected:
	const unsigned c_uBalls = 10;
};
