#include "Scheduler.h"

Scheduler::Scheduler() {}

Scheduler::~Scheduler() {}

void Scheduler::CheckForJob(Job * job)
{
	if (function_map.find(job->j_type) != function_map.end())
	{
		job->s_data.time_units = function_map[job->j_type];
	}
	else
	{
		auto temp = function_map.emplace(job->j_type, MAX_UNITS);
		job->s_data.time_units = temp.first->second;
	}
}

void Scheduler::CalculateJobTime(Job * job)
{
	milliseconds time_taken;
	if ((time_taken = milliseconds(job->s_data.end_time - job->s_data.start_time)).count() > 0.f)
	{
		if (function_map[job->j_type] != MAX_UNITS)
		{
			auto & current_units = function_map[job->j_type];
			auto new_units = ((time_taken.count() * unit_lock_ratio) + current_units) / 2.f;
			current_units = new_units;
		}
		else
		{
          	function_map[job->j_type] = time_taken.count() * unit_lock_ratio;
		}
	}
}
