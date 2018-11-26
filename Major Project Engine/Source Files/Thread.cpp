#include "Thread.h"
#include "ThreadManager.h"
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
	delete(current_job);
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
			if (current_job->get_function()(current_job->get_content()))
			{
				count++;
				delete(current_job);
				current_job = nullptr;
			}
			else 
			{
				ThreadManager::Instance().register_job(current_job);
				current_job = nullptr;
			}
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
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
	return (current_job == nullptr);
}

Job * & Thread::get_location()
{
	return current_job;
}
