#pragma once
#include "Job.h"

#include <atomic>
#include <thread>
#include <chrono>
#include <string>

class Thread
{
public:
	Thread(const std::string & name);
	~Thread();

	void Execution();
	void recieve_Job(std::unique_ptr<Job> & job);
	std::atomic_bool & get_state();

	int print_stats()
	{
		printf("%s completed %u Jobs\n", _name.c_str(), count);
		return count;
	}

private:
	bool _running = true;
	std::atomic_bool _busy = false;
	std::string _name;
	int count = 0;

	std::unique_ptr<std::thread> _thread;
	std::unique_ptr<Job> current_job;
};


