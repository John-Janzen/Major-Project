#include "Scheduler.h"

Scheduler::Scheduler(const std::size_t & limit) : size_of_bickets(limit) {}

Scheduler::~Scheduler() {}

void Scheduler::SetTimeLock(const float & lock)
{
	time_limit = lock;
}

void Scheduler::StartFrame()
{
	t_start = hr_clock::now();
	frame_time = 0.f;
}

void Scheduler::SetScale(Job * & job)
{
	//job->_scale = ((*job->f_time_data / time_limit) * 10) + (job->j_type / Job::JOB_HEAD_END) + (0x100 % job->j_type) + (job->_age * 10);
}

bool Scheduler::CheckForJob(Job * job)
{
	/*auto job_found = m_functions.find(job->j_type);
	if (job_found != m_functions.end())
	{
		job->_scale = ((job_found->second / time_limit) * 10) + (job->j_type / Job::JOB_HEAD_END) + (job->j_type % 0x100);
		job->f_time_data = &job_found->second;
		return true;
	}
	else
	{
		auto temp = m_functions.emplace(job->j_type, 0.01f);
		job->f_time_data = &temp.first->second;
	}*/
	return false;
}

void Scheduler::SortJobs(std::list<Job*> & job_list, std::priority_queue<Job*, std::vector<Job*>, Job> & job_queue)
{
	/*Job * job_ptr = nullptr;
	while (!job_list.empty())
	{
		if (frame_time < time_limit)
		{
			job_ptr = (*job_list.begin());
			for (auto tl_it : job_list)
			{
				if (time_limit > (*tl_it->f_time_data + frame_time))
				{
					if (job_ptr->_scale < tl_it->_scale || *job_ptr->f_time_data < *tl_it->f_time_data)
					{
						job_ptr = tl_it;
					}
				}
			}
			job_queue.emplace(job_ptr);
			job_list.remove(job_ptr);
			frame_time += *job_ptr->f_time_data;
			job_ptr = nullptr;
		}
		else
		{
			break;
		}
	}*/
	//printf("Time: %f\n", frame_time);
}
