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

	enum THREAD_TYPE
	{
		NULL_THREAD,
		ANY_THREAD,
		MAIN_THREAD,
		RENDER_THREAD
	};

	struct ThreadData
	{
		job::JOB_ID t_id;
		std::string t_name;
		hr_tp t_start, t_end;
	};

	Thread(const std::string & name, const THREAD_TYPE type = ANY_THREAD);

	~Thread();

	void Execution();

	void Stop();

	void Alert() { _cv.notify_one(); }

	const THREAD_TYPE GetType() { return t_type; }

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

	void EmplaceNewJob(Job * & job, const hr_tp & objective_time) 
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

	const nanoseconds GetAllotedTime() 
	{
		std::lock_guard<std::mutex> lock(time_mutex);
		return queue_time;
	}

	const hr_tp GetEndTime() { std::lock_guard<std::mutex> lock(time_mutex); return current_end; }
	void ResetAllotedTime() { queue_time = nanoseconds(0); current_end = hr::now(); }

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

	void ClearLogger()
	{
		Logger.ClearData();
	}

	std::vector<ThreadData> & AquireData()
	{
		return Logger.GetData();
	}

	void ToggleDebug() { debug_mode = !debug_mode; }

private:

	class ThreadLogger
	{
	public:

		ThreadLogger() {};
		~ThreadLogger() {};

		ThreadData * Instantiate(const job::JOB_ID & id, const std::string & name)
		{
			return &(*t_data.emplace(t_data.end(), ThreadData{ id, name, hr::now() }));
		}

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

		void ClearData()
		{
			t_data.clear();
		}

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
