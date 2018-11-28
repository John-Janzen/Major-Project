#pragma once

#ifndef _TIMER_H
#define _TIMER_H

#include <chrono>
#include <iostream>
#include <thread>

using hr_clock = std::chrono::high_resolution_clock;
using mil_duration = std::chrono::duration<long long, std::nano>;

class Timer
{
public:
	Timer();
	~Timer();

	void Restart();
	void Start();
	void End();

	void wait_time();

	void Print(const mil_duration & time);

	void set_delta_time();
	const float & get_delta_time() { return delta_time; }

	void set_time_lock(const float time) 
	{
		current_time_lock = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<double, std::milli>(time)); 
	}

private:
	hr_clock::time_point current_time_frame;
	hr_clock::time_point frame_rate_control;

	hr_clock::time_point d_start, d_end;

	float delta_time;
	mil_duration current_time_lock;

	int frame_count = 0;
};

#endif // !_TIMER_H
