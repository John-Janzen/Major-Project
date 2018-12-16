#include "Thread.h"
#include "TaskManager.h"
/*
* Constructor that registers a name for the
* thread class.
*/
Thread::Thread(const std::string & name, const THREAD_TYPE type)
	:_name(name), t_type(type)
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
		if (current_job != nullptr)
		{
			//printf("%s Starting Job: %s\n", this->_name.c_str(), current_job->GetName().c_str());
			auto result = current_job->GetFunction()(current_job->GetContent());
			if (result == JOB_COMPLETED)
			{
				count++;
				TaskManager::Instance().NotifyDone();
				delete(current_job);
				current_job = nullptr;
			}
			else if (result == JOB_RETRY)
			{
				TaskManager::Instance().RetryJob(current_job);
				current_job = nullptr;
			}
			else
			{
				printf("ISSUE FOUND WITH JOB: %s", current_job->GetName().c_str());
				throw std::invalid_argument("FOUND ISSUE");
			}
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::nanoseconds(500));
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

bool Thread::CheckAvailable()
{
	return (current_job == nullptr);
}

Job * & Thread::GetLocation()
{
	return current_job;
}
