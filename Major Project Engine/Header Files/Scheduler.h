#pragma once

#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include "EventHandler.h"
#include "Job.h"

#include <chrono>
#include <map>

class Scheduler
{
public:

	Scheduler();

	~Scheduler();

	void SetTimeLock(const float & time_lock)
	{
		this->time_lock = time_lock;
		unit_lock_ratio = (int)std::ceil(MAX_UNITS / time_lock);
	}

	void CheckForJob(Job * job);

	void CalculateJobTime(Job * job);

private:

	using FuncTimes = std::map<job::JOB_ID, Job::UNIT_TIME>;
	using hr_clock = std::chrono::high_resolution_clock;
	using milliseconds = std::chrono::duration<float, std::milli>;

	const static Job::UNIT_TIME MAX_UNITS = 1000;

	float time_lock;
	Job::UNIT_TIME unit_lock_ratio;

	FuncTimes function_map;
};

#endif // !_SCHEDULER_H
