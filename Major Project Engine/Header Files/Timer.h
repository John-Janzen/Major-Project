#pragma once

#ifndef _TIMER_H
#define _TIMER_H

#include <chrono>
#include <iostream>
#include <thread>

class Timer
{
public:
	Timer();
	~Timer();

	void Start();
	void End();

	const std::chrono::steady_clock::rep & elapsed_work();
	void wait_time();

	void Print(const std::chrono::duration<double, std::milli> & time);

	void set_delta_time();
	const float & get_delta_time() { return delta_time; }

	void set_time_lock(const float & time) { current_time_lock = time; }

private:
	std::chrono::steady_clock::time_point start_time;
	std::chrono::steady_clock::time_point end_time;

	float delta_time;
	float current_time_lock;
};

#endif // !_TIMER_H
