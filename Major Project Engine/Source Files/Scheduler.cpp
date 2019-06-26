#include "Scheduler.h"

Scheduler::Scheduler() {}

Scheduler::~Scheduler() {}

void Scheduler::CheckForJob(Job * job)
{
	if (function_map.find(job->j_type) != function_map.end())		// go through the whole map and find job
	{
		std::lock_guard<std::mutex> lock(map_access);
		job->s_data.time_span = function_map[job->j_type];			//  set the jobs time span to what the map has
	}
	else
	{
		std::lock_guard<std::mutex> lock(map_access);
		auto temp = function_map.emplace(job->j_type, time_lock);	// emplace a new job id and maxed time lock
		job->s_data.time_span = temp.first->second;
	}
}

void Scheduler::CalculateJobTime(Job * job)
{
	nanoseconds time_taken;
	if ((time_taken = job->s_data.end_time - job->s_data.start_time) > nanoseconds(0))		// if the job is greater than 0 *Just a precaution*
	{
		if (function_map[job->j_type] != time_lock)				// if Job isn't deafult time
		{
			std::lock_guard<std::mutex> lock(map_access);
			auto & current_units = function_map[job->j_type];
			auto new_units = (time_taken + current_units) / 2;		// then take the last job and this jobs time and average it
			current_units = new_units;
		}
		else
		{
			std::lock_guard<std::mutex> lock(map_access);
          	function_map[job->j_type] = time_taken;					// just set the time to the map
		}
	}
}
