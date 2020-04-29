#pragma once

class IFrame
{
public:
	//��ǰframe�Լ�֮����ܷ�
	virtual unsigned TotalScore() = 0;

	//��ǰ֡�÷�
	virtual unsigned FrameScore() = 0;

	//��ǰ֡�ɹ��׵�spare����
	virtual unsigned GetSpareBonus() = 0;

	//��ǰ֡�ɹ��׵�strike����
	virtual unsigned GetStrikeBonus() = 0;

	IFrame*	NextFrame();

	void SetNextFrame(IFrame *frame);

protected:
	const unsigned c_uBalls = 10;
};
