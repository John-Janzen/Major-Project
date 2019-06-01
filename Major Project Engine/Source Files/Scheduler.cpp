#include "Scheduler.h"

Scheduler::Scheduler() {}

Scheduler::~Scheduler() {}

void Scheduler::CheckForJob(Job * job)
{
	if (function_map.find(job->j_type) != function_map.end())
	{
		job->time_units = &function_map[job->j_type].Get();
	}
	else
	{
		auto temp = function_map.emplace(job->j_type, JobData());
		job->time_units = &temp.first->second.Get();
	}
}
