#pragma once

#ifndef _THREAD_H
#define _THREAD_H

#include "Job.h"
#include "EventHandler.h"

#include <atomic>
#include <thread>
#include <chrono>
#include <string>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <iostream>

/*
* The thread wrapper class that encapsulate the standard
* c++ library thread class. This will provide with additional
* functionality that will be useful later on in the project.
*/
class Thread
{
public:

	static const std::size_t MAX_THREADS = 8;
	
	/*
	Designates the thread type
	*/
	enum THREAD_TYPE
	{
		NULL_THREAD,
		ANY_THREAD,
		MAIN_THREAD,
		RENDER_THREAD
	};
	
	/*
	This is for the thread logger and thread debugger
	Job by job id with start, end and span time
	*/
	struct ThreadData
	{
		job::JOB_ID t_id;
		std::string t_name;
		hr_tp t_start, t_end;
	};

	Thread(const std::string & name, const THREAD_TYPE type = ANY_THREAD);

	~Thread();

	/*
	Main function the threads will be using to execute the jobs
	*/
	void Execution();

	/*
	Stop the threads from processing anymore jobs
	*/
	void Stop();

	/*
	Alert the thread through the condition variable
	*/
	void Alert() { _cv.notify_one(); }

	/*
	Get thread type
	*/
	const THREAD_TYPE GetType() { return t_type; }

	/*
	Check to see if thread has any jobs available
	*/
	const bool HasJob() 
	{ 
		bool test = !this->_queue.empty();
		bool test2 = current_job != nullptr;

		if (test)
			this->Alert();

		if (current_job != nullptr)
			auto asdar = "asdasr";

		return (test2 || test);
	}

	/*
	Emplace a job on this threads queue as well as adjust the 
	thread end time on the queue
	*/
	void EmplaceNewJob(Job * & job, const hr_tp & objective_time);
	
	/*
	Get the total alloted time for the threads jobs
	*/
	const nanoseconds GetAllotedTime() 
	{
		std::lock_guard<std::mutex> lock(time_mutex);
		return queue_time;
	}

	/*
	Get the time point at which the thread will finish its job.
	*/
	const hr_tp GetEndTime() { std::lock_guard<std::mutex> lock(time_mutex); return current_end; }

	/*
	Reset the alloted time.
	*/
	void ResetAllotedTime(const hr_tp & obj_time) { queue_time = nanoseconds(0); current_end = obj_time; }

	/*
	* Prints available stats.
	* Number of jobs completed.
	*/
	int PrintLogger(const std::chrono::high_resolution_clock::time_point & obj_time)
	{
		printf("%s\tcompleted %u Jobs\n", _name.c_str(), count);

		Logger.PrintData(_name, obj_time);

		return count;
	}

	/*
	Clear the logger
	*/
	void ClearLogger()
	{
		Logger.ClearData();
	}

	/*
	Get all of the data available for this thread
	*/
	std::vector<ThreadData> & AquireData()
	{
		return Logger.GetData();
	}

	/*
	Toggle debug mode for this thread
	*/
	void ToggleDebug() { debug_mode = !debug_mode; }

private:

	/*
	The Thread logger is an inner class that keeps track of 
	the jobs it has completed in the debug mode
	*/
	class ThreadLogger
	{
	public:

		ThreadLogger() {};
		~ThreadLogger() {};

		/*
		Create a new Thread data and return it
		*/
		ThreadData * Instantiate(const job::JOB_ID & id, const std::string & name)
		{
			return &(*t_data.emplace(t_data.end(), ThreadData{ id, name, hr::now() }));
		}

		/*
		Print the stats of all the jobs in a clear table
		*/
		void PrintData(const std::string name, const hr_tp & obj_time)
		{
			std::cout << "Logged Data for Thread: " << name.c_str() << "\n";
			std::cout << "-------------------------" << "\n";

			if (t_data.empty())
				std::cout << "No Data To Log!!!" << "\n";

			for (auto & time : t_data)
			{
				std::cout << "ID:\t" << time.t_id  << "\tName:\t" << time.t_name << "\n";
				std::cout << "Start Time:\t" << std::chrono::duration_cast<std::chrono::milliseconds>(time.t_start - obj_time).count() << "\n";
				std::cout << "Duration:\t" << std::chrono::duration_cast<std::chrono::milliseconds> (time.t_end - time.t_start).count() << "\n";
				std::cout << "End Time:\t" << std::chrono::duration_cast<std::chrono::milliseconds>(time.t_end - obj_time).count() << "\n";
			}
			std::cout << "Logged Data End" << std::endl;
			std::cout << "-------------------------" << std::endl;
		}

		/*
		Clear the data in the thread logger
		*/
		void ClearData()
		{
			t_data.clear();
		}

		/*
		Get all available data in logger
		*/
		std::vector<ThreadData> & GetData() { return t_data; }

	private:
		std::vector<ThreadData> t_data;
	} Logger;

	//ThreadLogger Logger;
	THREAD_TYPE t_type;

	bool _running = true, debug_mode = false;
	std::string _name;
	int count = 0;

	std::unique_ptr<std::thread> _thread;
	Job * current_job = nullptr;

	std::mutex queue_mutex, time_mutex;
	std::condition_variable _cv;
	std::queue<Job*> _queue;
	nanoseconds queue_time = nanoseconds(0);
	hr_tp current_end;

};

#endif // !_THREAD_H
