#pragma once

#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include "Job.h"

#include <chrono>
#include <map>
#include <mutex>

class Scheduler
{
public:

	Scheduler();

	~Scheduler();

	void SetTimeLock(const float & time_lock)
	{
		auto milliseconds = std::chrono::duration<float, std::milli>(time_lock);
		this->time_lock = std::chrono::duration_cast<nanoseconds>(milliseconds);
	}

	void CheckForJob(Job * job);

	void CalculateJobTime(Job * job);

private:

	std::map<job::JOB_ID, nanoseconds> function_map;
	std::mutex map_access;
	nanoseconds time_lock;
};

#endif // !_SCHEDULER_H
