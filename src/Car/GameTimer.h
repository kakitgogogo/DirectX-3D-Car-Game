#pragma once

// ��Ϸ��ʱ��
class GameTimer 
{
public:
	GameTimer();

	// ��ȡ������ʱ��
	float TotalTime() const;
	// ��ȡ�����ϴε�Tick��ʱ����
	float DeltaTime() const;

	// ���ü�ʱ�������¿�ʼ��ʱ
	void Reset();
	// ����ͣ�ָ�����ʼ��ʱ
	void Start();
	// ��ͣ��ʱ
	void Stop();

	// ÿһ֡����
	void Tick();

private:
	float mSecondsPerCount;
	float mDeltaTime;

	__int64 mBaseTime;   // ��Ϸ��ʼ��ʱ��
	__int64 mStopTime;   // ��Ϸ��ͣ��ʱ��
	__int64 mPrevTime;   // ֮ǰTick��ʱ��
	__int64 mCurrTime;   // ���ڵ�ʱ��

	__int64 mPausedTime; // ��ͣ����ʱ��

	bool mStopped;       // ��Ϸ�Ƿ���ͣ
};