#include "Thread.h"

/*
* Constructor that registers a name for the
* thread class.
*/
Thread::Thread(const std::string & name)
	:_name(name)
{
	_thread = std::make_unique<std::thread>(&Thread::Execution, this);
}

/*
* Destructor that elimates all data pertaining
* to the thread class.
*/
Thread::~Thread() 
{
	current_job.reset();
	current_job = nullptr;
}

/*
* Each thread will run this function continously for the duration of
* the project. As pictured in the proposal Figure on this is the
* crux of the multi-threading portion.
*
* As long as there is no job for the thread to do then it will
* go back to sleep. Else, the job will hopefully be completed
* and the thread continues the cycle.
*/
void Thread::Execution()
{
	while (_running)
	{
		if (current_job != nullptr)
		{
			current_job.get()->get_function()(current_job->get_content());
			current_job.reset();
			current_job = nullptr;
			std::printf("%s Finished Job!\n", _name.c_str());
			count++;
			_busy = false;
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
}

/*
* This is where the threads recieve jobs from the managers.
* The flag _busy is to indicate to the manager that it is currently
* working on a job.
*/
void Thread::recieve_Job(std::unique_ptr<Job> & job)
{
	_busy = true;
	current_job = std::move(job);
}

/*
* This function stops the threads from continously running and 
* joins the main thread until there is only one thread left.
*/
void Thread::Stop()
{
	_running = false;
	_thread->join();
}

/*
* Gets the current state of the thread.
* This is a flag for busy or not busy for the manager.
*/
std::atomic_bool& Thread::get_state()
{
	return _busy;
}
