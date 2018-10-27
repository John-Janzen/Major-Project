#pragma once
#include "Job.h"

#include <thread>
#include <chrono>
#include <string>

class Thread
{
public:
	Thread(const std::string & name);
	~Thread();

	void Execution();

private:
	bool _running = true;
	std::string _name;

	std::unique_ptr<std::thread> _thread;
	std::unique_ptr<Job> current_job;
};


