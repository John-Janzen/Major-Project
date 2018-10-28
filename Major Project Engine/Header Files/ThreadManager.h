#pragma once
#include "Thread.h"

#include <queue>

class ThreadManager
{
public:

	static ThreadManager& Instance()
	{
		static ThreadManager inst;
		return inst;
	}

	void Init(const uint16_t & size)
	{
		num_of_threads = size;
		threads = new Thread*[size];
		std::string name;
		for (std::size_t i = 0; i < size; i++)
		{
			switch (i)
			{
			case 0:
				name = "Albert";
				break;
			case 1:
				name = "Curie";
				break;
			case 2:
				name = "Newton";
				break;
			case 3:
				name = "Dennis";
				break;
			}
			threads[i] = new Thread(name);
		}
		job_list = std::queue<std::unique_ptr<Job>>();
	}

	~ThreadManager() 
	{
		for (std::size_t i = 0; i < num_of_threads; i++)
		{
			delete(threads[i]);
		}
		std::queue<std::unique_ptr<Job>>().swap(job_list);
	}

	void allocate_jobs()
	{
		for (std::size_t i = 0; i < num_of_threads && !jobs_empty(); i++)
		{
			if (threads[i]->get_state() == false)
			{
				threads[i]->recieve_Job(job_list.front());
				job_list.pop();
			}
		}
	}

	void register_job(Job_Type type, std::function<void()> function)
	{
		job_list.emplace(std::make_unique<Job>(type, function));
	}

	void register_job(std::unique_ptr<Job> job)
	{
		job_list.emplace(std::move(job));
	}

	bool jobs_empty()
	{
		return job_list.empty();
	}

	bool jobs_full()
	{
		return job_list.size() > 20 ? true : false;
	}

	void print_total_jobs()
	{
		uint16_t total = 0;
		for (std::size_t i = 0; i < num_of_threads; i++)
			total += threads[i]->print_stats();
		printf("Total count is: %u", total);
	}

private:
	ThreadManager() {};
	Thread ** threads;

	std::size_t num_of_threads;

	std::queue<std::unique_ptr<Job>> job_list;
};
