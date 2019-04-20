#include "Thread.h"
#include "ThreadManager.h"

/*
* Constructor that registers a name for the
* thread class.
*/
Thread::Thread(ThreadManager * const tm, BlockingQueue<Job*> & queue, const std::string & name, const THREAD_TYPE type)
	: job_list(queue), _name(name), t_type(type)
{
	_thread = std::make_unique<std::thread>(&Thread::Execution, this, tm);
}

/*
* Destructor that elimates all data pertaining
* to the thread class.
*/
Thread::~Thread() 
{
	_thread->join();
	_thread.reset();
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
void Thread::Execution(ThreadManager * const tm)
{
	Job * current_job;
	while (_running)
	{
		job_list.Aquire(current_job);

		if (!_running) break;

		switch ((*current_job)())
		{
		case JOB_COMPLETED:
			count++;

			delete(current_job);

			current_job = nullptr;
			tm->NotifyDone();
			break;
		case JOB_RETRY:
			tm->RetryJob(current_job);

			current_job = nullptr;
			//tm->NotifyDone();
			break;
		case JOB_ISSUE:
			printf("ISSUE FOUND WITH JOB: %s", current_job->job_name.c_str());
			throw std::invalid_argument("FOUND ISSUE");
			break;
		default:
			break;
		}
	}
}

/*
* This function stops the threads from continously running and
* joins the main thread until there is only one thread left.
*/
void Thread::Stop()
{
	//while (current_job != nullptr);
	_running = false;
	//current_job = new Job();
	//this->Notify();
}
