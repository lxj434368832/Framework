#pragma once

#define PIN_COUNT 10

__interface IFrame
{
	//��ǰframe�Լ�֮����ܷ�
	virtual unsigned TotalScore();

	//��ǰ֡�÷�
	virtual unsigned FrameScore();

	//��ǰ֡�ɹ��׵�spare����
	virtual unsigned GetSpareBonus();

	//��ǰ֡�ɹ��׵�strike����
	virtual unsigned GetStrikeBonus();

	IFrame*	NextFrame();

	void SetNextFrame(IFrame *fm);
};
