#pragma once

#ifndef _SCHEDULER_H
#define _SCHEDULER_H

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

private:

	struct JobData
	{
		Job::UNIT_TIME time = 0u;
		int count = 0;

		Job::UNIT_TIME & Get() { count++; return time; }
	};

	using FuncTimes = std::map<job::JOB_ID, JobData>;

	float time_lock;
	const Job::UNIT_TIME MAX_UNITS = 1000;
	Job::UNIT_TIME unit_lock_ratio;

	FuncTimes function_map;
};

#endif // !_SCHEDULER_H
