#pragma once

#ifndef _TIMER_H
#define _TIMER_H

#include <chrono>
#include <iostream>
#include <thread>

using hr_clock = std::chrono::high_resolution_clock;
using ms_duration = std::chrono::duration<float, std::milli>;

class Timer
{
public:
	Timer();
	~Timer();

	void Start();
	void Stop();

	void WaitTime();

	bool CheckTimeLimit();

	const float & GetDeltaTime() { return delta_time; }

	void SetTimeLock(const float time) 
	{
		current_time_lock = ms_duration(time); 
	}

private:

	void Restart();
	
	void SetDeltaTime();

	void Print(const ms_duration & time);

	hr_clock::time_point current_time_frame;
	hr_clock::time_point frame_rate_control;

	float delta_time;
	ms_duration current_time_lock;

	hr_clock::time_point timer_start;
	hr_clock::time_point timer_end;

	int frame_count = 0;
};

#endif // !_TIMER_H
