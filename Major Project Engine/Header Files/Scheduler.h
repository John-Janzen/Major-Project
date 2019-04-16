#pragma once

#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include "Job.h"

#include <chrono>
#include <list>
#include <vector>
#include <queue>
#include <map>

class Scheduler
{
public:

	using hr_clock = std::chrono::high_resolution_clock;
	using ms = std::chrono::duration<float, std::milli>;
	using FuncTimes = std::map<Job::JOB_ID, float>;

	Scheduler(const std::size_t & size);
	~Scheduler();

	void SetTimeLock(const float & lock);

	void StartFrame();

	void SetScale(Job *& job);

	bool CheckForJob(Job * job);

	void SortJobs (std::list<Job*> & job_list, std::priority_queue<Job*, std::vector<Job*>, Job> & job_queue);

private:

	hr_clock::time_point t_start;
	float time_limit;
	float frame_time = 0;
	std::size_t  size_of_bickets = 1;
	FuncTimes m_functions;
};

#endif // !_SCHEDULER_H
