#include "Thread.h"

Thread::Thread(const std::string & name)
	:_name(name)
{
	_thread = std::make_unique<std::thread>(&Thread::Execution, this);
}

Thread::~Thread() 
{
	_thread->join();
	current_job.release();
	current_job = nullptr;
}

void Thread::Execution()
{
	while (_running)
	{
		std::this_thread::sleep_for(std::chrono::seconds(2));
		std::printf("%s Running...", _name.c_str());
	}
}
