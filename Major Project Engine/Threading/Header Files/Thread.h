#pragma once

#ifndef _THREAD_H
#define _THREAD_H

#include "Job.h"
#include "BlockingQueue.h"

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
	typedef std::chrono::high_resolution_clock hr;
	typedef hr::time_point ctp;

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
		ctp t_start, t_end;
	};

	Thread(BlockingQueue<Job*> & queue, const std::string & name, const THREAD_TYPE type = ANY_THREAD);

	~Thread();

	void Execution();
	void Stop();

	const THREAD_TYPE GetType() { return t_type; }

	const bool HasJob() { return (current_job != nullptr); }

	const int GetAllotedTime() const { return this->queue_time; }

	void AddAllotedTime(const Job::UNIT_TIME & time) { this->queue_time += time; }
	void SubAllotedTime(const Job::UNIT_TIME & time) { this->queue_time -= time; }

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

		ThreadData * Instatiate(const job::JOB_ID & id, const std::string & name)
		{
			return &(*t_data.emplace(t_data.end(), ThreadData{ id, name, hr::now() }));
		}

		void PrintData(const std::string name, const ctp & obj_time)
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

	Job::UNIT_TIME queue_time = 0;

	std::unique_ptr<std::thread> _thread;
	Job * current_job;

	BlockingQueue<Job*> & job_list;
};

#endif // !_THREAD_H
