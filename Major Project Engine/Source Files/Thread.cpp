#include "Thread.h"

Thread::Thread(const std::string & name)
	:_name(name)
{
	_thread = std::make_unique<std::thread>(&Thread::Execution, this);
}

Thread::~Thread() 
{
	current_job.release();
	current_job = nullptr;
}

void Thread::Execution()
{
	while (_running)
	{
		if (current_job != nullptr)
		{
			current_job.get()->get_function()();
			current_job.reset();
			current_job = nullptr;
			std::printf("%s Finished Job!\n", _name.c_str());
			count++;
			_busy = false;
		}
		//else
		//{
		//	std::this_thread::sleep_for(std::chrono::milliseconds(100));
		//}
	}
}

void Thread::recieve_Job(std::unique_ptr<Job> & job)
{
	_busy = true;
	current_job = std::move(job);
}

void Thread::Stop()
{
	_running = false;
	_thread->join();
}

std::atomic_bool& Thread::get_state()
{
	return _busy;
}
