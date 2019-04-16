#include "Thread.h"
#include "TaskManager.h"

/*
* Constructor that registers a name for the
* thread class.
*/
Thread::Thread(BlockingQueue<Job*> & queue, const std::string & name, const THREAD_TYPE type)
	: job_list(queue), _name(name), t_type(type)
{
	_thread = std::make_unique<std::thread>(&Thread::Execution, this);
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
void Thread::Execution()
{
	Job * current_job;
	while (_running)
	{
		job_list.Aquire(current_job);

		switch ((*current_job)())
		{
		case JOB_COMPLETED:
			count++;
			delete(current_job);
			current_job = nullptr;
			break;
		case JOB_RETRY:
			job_list.Emplace(current_job);
			current_job = nullptr;
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
