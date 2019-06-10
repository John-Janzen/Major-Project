#pragma once

#ifndef _TIMER_H
#define _TIMER_H

#include <chrono>
#include <iostream>
#include <thread>

class Timer
{
public:

	static Timer& Instance()
	{
		static Timer inst;
		return inst;
	}

	using hr_clock = std::chrono::high_resolution_clock;
	using ms = std::chrono::duration<float, std::milli>;

	~Timer();

	void Init();

	void Start();

	void Stop();

	void WaitTime();

	bool CheckTimeLimit();

	const float & GetDeltaTime() { return delta_time; }

	void SetTimeLock(const float time) 
	{
		current_time_lock = ms(time);
	}

private:

	Timer() {}

	void Restart();
	
	void SetDeltaTime();

	void Print(const ms & time);

	hr_clock::time_point current_time_frame;
	hr_clock::time_point frame_rate_control;

	float delta_time;
	ms current_time_lock = ms(16.6667f);

	hr_clock::time_point timer_start;
	hr_clock::time_point timer_end;

	int frame_count = 0;
};

#endif // !_TIMER_H
