#pragma once

#ifndef _THREAD_H
#define _THREAD_H

#include "Job.h"

#include <atomic>
#include <thread>
#include <chrono>
#include <string>
#include <condition_variable>
#include <mutex>

/*
* The thread wrapper class that encapsulate the standard
* c++ library thread class. This will provide with additional
* functionality that will be useful later on in the project.
*/
class Thread
{
public:

	enum THREAD_TYPE
	{
		NULL_THREAD,
		ANY_THREAD,
		RENDER_THREAD,
		IO_THREAD
	};

	Thread(const std::string & name, const THREAD_TYPE type = ANY_THREAD);
	~Thread();

	void Execution();
	void Notify();
	void Stop();

	bool CheckAvailable();

	Job * & GetLocation();

	const THREAD_TYPE GetType() { return t_type; }

	/*
	* Prints available stats.
	* Number of jobs completed.
	*/
	int PrintStats()
	{
		printf("%s\tcompleted %u Jobs\n", _name.c_str(), count);
		return count;
	}

private:
	THREAD_TYPE t_type;

	bool _running = true;
	std::string _name;
	int count = 0;

	std::condition_variable _cv;
	std::mutex _mutex;

	// Only one instance needed for these
	// No need for copying
	std::unique_ptr<std::thread> _thread;
	Job * current_job;
};

#endif // !_THREAD_H
