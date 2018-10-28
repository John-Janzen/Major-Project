#pragma once
#include "Job.h"

#include <atomic>
#include <thread>
#include <chrono>
#include <string>

/*
* The thread wrapper class that encapsulate the standard
* c++ library thread class. This will provide with additional
* functionality that will be useful later on in the project.
*/
class Thread
{
public:
	Thread(const std::string & name);
	~Thread();

	void Execution();
	void Stop();
	void recieve_Job(std::unique_ptr<Job> & job);
	std::atomic_bool & get_state();

	/*
	* Prints available stats.
	* Number of jobs completed.
	*/
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

	// Only one instance needed for these
	// No need for copying
	std::unique_ptr<std::thread> _thread;
	std::unique_ptr<Job> current_job;
};


