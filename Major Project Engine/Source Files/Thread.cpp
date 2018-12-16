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
	_cv.notify_all();
	_thread->join();
	_thread.reset();

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
	while (true)
	{
		std::unique_lock<std::mutex> lk(_mutex);
		_cv.wait(lk, [this] {return (this->current_job != nullptr); });

		if (!_running)
			break;

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
		lk.unlock();
	}
}

void Thread::Notify()
{
	_cv.notify_all();
}

/*
* This function stops the threads from continously running and
* joins the main thread until there is only one thread left.
*/
void Thread::Stop()
{
	_running = false;
	while (current_job != nullptr);
	current_job = new Job();
	this->Notify();
}

bool Thread::CheckAvailable()
{
	std::lock_guard<std::mutex> lk(_mutex);
	return (current_job == nullptr);
}

Job * & Thread::GetLocation()
{
	return current_job;
}
