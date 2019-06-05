#include "Thread.h"
#include "ThreadManager.h"

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
Thread::~Thread() {}

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
	ThreadData * data = nullptr;
	while (_running)
	{
		job_list.Aquire(current_job);

		if (!_running) break;

		if (debug_mode)
			data = Logger.Instatiate(current_job->j_type, current_job->job_name);

		if (current_job != nullptr)
		{
			current_job->s_data.start_time = hr::now();
			switch ((*current_job)())
			{
			case JOB_COMPLETED:
				current_job->s_data.end_time = hr::now();
				this->SubAllotedTime(current_job->s_data.time_units);

				count++;
				EventHandler::Instance().SendEvent(EventType::JOB_FINISHED, current_job);

				delete(current_job);
				current_job = nullptr;

				if (debug_mode && data != nullptr)
					data->t_end = std::chrono::high_resolution_clock::now();
				break;
			case JOB_RETRY:
				this->SubAllotedTime(current_job->s_data.time_units);
				EventHandler::Instance().SendEvent(EventType::JOB_REENTER, current_job);

				current_job = nullptr;
				if (debug_mode && data != nullptr)
					data->t_end = std::chrono::high_resolution_clock::now();
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
}

/*
* This function stops the threads from continously running and
* joins the main thread until there is only one thread left.
*/
void Thread::Stop()
{
	_running = false;
	_thread->join();
	_thread.reset();
}
