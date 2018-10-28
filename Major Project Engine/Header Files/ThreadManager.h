#pragma once
#include "Thread.h"

#include <queue>

/*
* This Manager class is created as a singleton because
* we don't need multiple instances of the thread manager.
* A single class will do as most programs will only engage
* with this class by calling register_job.
* 
* Furthermore, main thread is going to be the only one to
* use most of these functions.
*/
class ThreadManager
{
public:

	/*
	* Singleton required function.
	* Gets the classes data.
	*/
	static ThreadManager& Instance()
	{
		static ThreadManager inst;
		return inst;
	}

	/*
	* Initializes the manager to create threads with a dynamic
	* amount based on the hardware limitations.
	*/
	void Init(const uint16_t & size)
	{
		num_of_threads = size;
		threads = new Thread*[size];
		std::string name;
		for (std::size_t i = 0; i < size; i++)
		{
			/*
			* I have decided to name each of the threads
			* with a number of names (should probably add more).
			* Do you know who these names are connected to?
			*/
			switch (i)
			{
			case 0:
				name = "Albert";
				break;
			case 1:
				name = "Curie";
				break;
			case 2:
				name = "Newton";
				break;
			case 3:
				name = "Dennis";
				break;
			}
			threads[i] = new Thread(name);
		}
		job_list = std::queue<std::unique_ptr<Job>>();
	}

	/*
	* Stops the threads.
	*/
	void Close()
	{
		stop_threads();
	}

	/*
	* Deletes the threads and emptys the job list by swapping
	* with an empty array. (This was the simplest way of emptying a queue)
	*/
	~ThreadManager() 
	{
		for (std::size_t i = 0; i < num_of_threads; i++)
		{
			delete(threads[i]);
		}
		std::queue<std::unique_ptr<Job>>().swap(job_list);
	}

	/*
	* This function allocates jobs from the threads that are readily available.
	* As long as there are jobs to allocate to the threads it will check
	* all of the threads available to make sure they are not busy
	* before handing a job off the queue.
	*/
	void allocate_jobs()
	{
		for (std::size_t i = 0; i < num_of_threads && !jobs_empty(); i++)
		{
			if (threads[i]->get_state() == false)
			{
				threads[i]->recieve_Job(job_list.front());
				job_list.pop();
			}
		}
	}

	/*
	* To register a job in the manager any class can call this function
	* with the job type and function.
	*/
	void register_job(Job_Type type, std::function<void(Content*)> function)
	{
		job_list.emplace(std::make_unique<Job>(type, function));
	}

	/*
	* Register a job that the someone already made with the
	* make_unique function.
	*/
	void register_job(std::unique_ptr<Job> job)
	{
		job_list.emplace(std::move(job));
	}

	/*
	* Check if there is no jobs in the queue.
	*/
	bool jobs_empty()
	{
		return job_list.empty();
	}

	/*
	* Check if the queue is full. (20 is an
	* arbitrary number for testing purposes.
	*/
	bool jobs_full()
	{
		return job_list.size() > 20 ? true : false;
	}

	/*
	* Prints the stats for the jobs that the threads have
	* completed. Along with the number of jobs in total 
	* that have been completed.
	*/
	void print_total_jobs()
	{
		int total = 0;
		for (std::size_t i = 0; i < num_of_threads; i++)
			total += threads[i]->print_stats();
		printf("Total count is: %u", total);
	}

	/*
	* Stops the threads by calling there stop
	* function. Had to make this because a there
	* isn't an easy way to destruct a singleton.
	*/
	void stop_threads()
	{
		for (std::size_t i = 0; i < num_of_threads; i++)
			threads[i]->Stop();
	}

private:
	ThreadManager() {};
	Thread ** threads;

	std::size_t num_of_threads;

	// List of jobs available
	std::queue<std::unique_ptr<Job>> job_list;
};
