#pragma once

#ifndef _TIMER_H
#define _TIMER_H

#include <chrono>
#include <iostream>
#include <thread>

using SteadyClock = std::chrono::high_resolution_clock;
using MilliDuration = std::chrono::duration<double, std::milli>;

class Timer
{
public:
	Timer();
	~Timer();

	void Start();
	void End();

	const MilliDuration elapsed_work();
	void wait_time();

	void Print(const MilliDuration & time);

	void set_delta_time();
	const float & get_delta_time() { return delta_time; }

	void set_time_lock(const MilliDuration & time) { current_time_lock = time - MilliDuration(0.5); }

private:
	SteadyClock::time_point start_time;
	SteadyClock::time_point end_time;
	SteadyClock::time_point frame_rate_control;

	float delta_time;
	MilliDuration current_time_lock;

	int frame_count = 0;
};

#endif // !_TIMER_H
