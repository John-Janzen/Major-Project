#include "Thread.h"

/*
* Constructor that registers a name for the
* thread class.
*/
Thread::Thread(const std::string & name, const THREAD_TYPE type)
	: _name(name), t_type(type)
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
	while (true)
	{
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			_cv.wait(lock, [this] {								// wait at condition variable until there is a job or the game closes
				if (!this->_queue.empty())
				{
					this->current_job = std::move(this->_queue.front());
					this->_queue.pop();
					return true;
				}
				if (!this->_running)
				{
					return true;
				}
				return false;
			});
		}
		
		if (!_running) break;									// check if thread is still running here

		if (debug_mode)											// if debug mode is on then create new thread data
			data = Logger.Instantiate(current_job->j_type, current_job->job_name);

		if (current_job != nullptr)
		{
			current_job->s_data.start_time = hr::now();			// get job start time
			switch ((*current_job)())							// start job
			{
			case JOB_COMPLETED:									// if JOB COMPLETED then
				current_job->s_data.end_time = hr::now();		// get job end time
				{
					std::lock_guard<std::mutex> lock(time_mutex);
					this->queue_time -= this->current_job->s_data.time_span;	// adjust queued time for all jobs
				}

				count++;
				EventHandler::Instance().SendEvent(EventType::JOB_FINISHED, current_job);	// Post that I have finished a job

				delete(current_job);							// delete the job
				current_job = nullptr;							// reset to nullptr

				if (debug_mode && data != nullptr)
					data->t_end = std::chrono::high_resolution_clock::now();	// if debug mode then set end time

				break;
			case JOB_RETRY:										// if JOB RETRY
				{
					std::lock_guard<std::mutex> lock(time_mutex);
					this->queue_time -= this->current_job->s_data.time_span;	// adjust queued time for all jobs
				}
				EventHandler::Instance().SendEvent(EventType::JOB_REENTER, current_job);	// Post that I have to retry a job

				current_job = nullptr;							// reset to nullptr
				if (debug_mode && data != nullptr)
					data->t_end = std::chrono::high_resolution_clock::now();	// if debug mode then set end time

				break;
			case JOB_ISSUE:
				printf("ISSUE FOUND WITH JOB: %s", current_job->job_name.c_str());
				throw std::invalid_argument("FOUND ISSUE");		// if JOB ISSUE then throw exception * WILL TERMINATE PROGRAM *
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
	this->Alert();
	_thread->join();
	_thread.reset();
}

void Thread::EmplaceNewJob(Job *& job, const hr_tp & objective_time)
{
	{
		std::lock_guard<std::mutex> lock(time_mutex);
		if (current_end < hr::now())
		{
			auto now = nanoseconds(hr::now() - objective_time);
			current_end += now + job->s_data.time_span;
		}
		else
		{
			current_end += job->s_data.time_span;
		}
		queue_time += job->s_data.time_span;
	}
	{
		std::lock_guard<std::mutex> lock(queue_mutex);
		this->_queue.emplace(job);
	}
	this->Alert();
}
