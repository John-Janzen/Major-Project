#pragma once

#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include "Job.h"

#include <chrono>
#include <map>
#include <mutex>

/*
The Scheduler Class contains all the times of the jobs that are done.
Each jobs is started with the max amount of time end then reduced to 
the correct amount.
*/
class Scheduler
{
public:

	Scheduler();

	~Scheduler();

	/*
	Sets the time lock of the program (default: 16.6667)
	*/
	void SetTimeLock(const float & time_lock)
	{
		auto milliseconds = std::chrono::duration<float, std::milli>(time_lock);
		this->time_lock = std::chrono::duration_cast<nanoseconds>(milliseconds);
	}

	/*
	Get a job in and check if it is in the map
	*/
	void CheckForJob(Job * job);

	/*
	Once the job is finished com back and recalculate the new
	job time.
	*/
	void CalculateJobTime(Job * job);

private:

	std::map<job::JOB_ID, nanoseconds> function_map;
	std::mutex map_access;
	nanoseconds time_lock;
};

#endif // !_SCHEDULER_H
