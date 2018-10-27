#include "Thread.h"

Thread::Thread() 
{
	_thread = new std::thread();
}

Thread::~Thread() 
{
	_thread->join();
	current_job.release();
	current_job = nullptr;
}

void Thread::Execution()
{
	while (running)
	{
		std::this_thread::sleep_for(std::chrono::seconds(5));
		std::printf("Running...");
	}
}
