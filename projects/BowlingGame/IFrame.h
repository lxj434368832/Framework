#pragma once

#define PIN_COUNT 10

__interface IFrame
{
	//当前frame以及之后的总分
	virtual unsigned TotalScore();

	//当前帧得分
	virtual unsigned FrameScore();

	//当前帧可贡献的spare奖励
	virtual unsigned GetSpareBonus();

	//当前帧可贡献的strike奖励
	virtual unsigned GetStrikeBonus();

	IFrame*	NextFrame();

	void SetNextFrame(IFrame *fm);
};
