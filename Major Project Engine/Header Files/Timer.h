#pragma once

#ifndef _TIMER_H
#define _TIMER_H

#include <chrono>
#include <iostream>

/*
The Timer class is a singleton that controls the delta time,
wait time for the end of frame and a general stop watch.
*/
class Timer
{
public:

	/*
	Singleton Instaniate
	*/
	static Timer& Instance()
	{
		static Timer inst;
		return inst;
	}

	using hr_clock = std::chrono::high_resolution_clock;
	using ms = std::chrono::duration<float, std::milli>;

	~Timer();

	/*
	Initialize the timer
	*/
	void Init();

	/*
	Start a new timer
	*/
	void Start();

	/*
	Stop the started timer
	*/
	void Stop();

	/*
	Wait till frame has ended
	*/
	void WaitTime();

	/*
	Check the time against the time limit
	*/
	bool CheckTimeLimit();

	/*
	Get the delta time of the frame
	*/
	const float & GetDeltaTime() { return delta_time; }

	/*
	Set the time lock (default 16.6667)
	*/
	void SetTimeLock(const float time) 
	{
		current_time_lock = ms(time);
	}

private:

	Timer() {}

	/*
	Restart the Timer class
	*/
	void Restart();
	
	/*
	Calculate the Delta Time
	*/
	void SetDeltaTime();

	/*
	Print the timer
	*/
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
