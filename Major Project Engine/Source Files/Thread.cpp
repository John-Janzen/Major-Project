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
		if (current_job)
		{
			current_job.get()->get_function()(current_job->get_content());
			//printf("%s\tfinished Job %i\n", this->_name.c_str(), static_cast<int>(current_job.get()->get_type()));
			count++;
			current_job.reset();
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(2));
		}
	}
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

bool Thread::check_availability()
{
	return (current_job.get() == nullptr);
}

std::unique_ptr<Job> * Thread::get_location()
{
	return &current_job;
}
