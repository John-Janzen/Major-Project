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

	void Restart();

	void wait_time();

	void Print(const ms_duration & time);

	void set_delta_time();
	const float & get_delta_time() { return delta_time; }

	void set_time_lock(const float time) 
	{
		current_time_lock = ms_duration(time); 
	}

private:
	hr_clock::time_point current_time_frame;
	hr_clock::time_point frame_rate_control;

	float delta_time;
	ms_duration current_time_lock;

	int frame_count = 0;
};

#endif // !_TIMER_H
