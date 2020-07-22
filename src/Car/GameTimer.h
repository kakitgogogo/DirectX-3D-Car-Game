#pragma once

// 游戏计时器
class GameTimer 
{
public:
	GameTimer();

	// 获取总运行时间
	float TotalTime() const;
	// 获取距离上次的Tick的时间差距
	float DeltaTime() const;

	// 重置计时器，重新开始计时
	void Reset();
	// 从暂停恢复，开始计时
	void Start();
	// 暂停计时
	void Stop();

	// 每一帧调用
	void Tick();

private:
	float mSecondsPerCount;
	float mDeltaTime;

	__int64 mBaseTime;   // 游戏开始的时刻
	__int64 mStopTime;   // 游戏暂停的时刻
	__int64 mPrevTime;   // 之前Tick的时刻
	__int64 mCurrTime;   // 现在的时刻

	__int64 mPausedTime; // 暂停的总时间

	bool mStopped;       // 游戏是否暂停
};