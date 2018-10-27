#pragma once
#include "Job.h"

#include <thread>
#include <chrono>

class Thread
{
public:
	Thread();
	~Thread();

	void Execution();

private:
	bool running = true;

	std::thread * _thread;
	std::unique_ptr<Job> current_job;
};


