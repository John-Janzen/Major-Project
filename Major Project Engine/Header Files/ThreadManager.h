#pragma once
#include "Thread.h"

#include <queue>

class ThreadManager
{
public:

	static ThreadManager& Instance()
	{
		static ThreadManager inst;
		return inst;
	}

	void Init(const uint16_t & size)
	{
		num_of_threads = size;
		threads = new Thread*[size];
		thread_list = std::queue<Thread*>();
		std::string name;
		for (std::size_t i = 0; i < size; i++)
		{
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
			thread_list.emplace(&*threads[i]);
		}
		job_list = std::queue<std::unique_ptr<Job>>();
	}

	void Close()
	{
		this->~ThreadManager();
	}

	~ThreadManager() 
	{
		for (std::size_t i = 0; i < num_of_threads; i++)
		{
			delete(threads[i]);
		}
		std::queue<Thread*>().swap(thread_list);
		std::queue<std::unique_ptr<Job>>().swap(job_list);
	}
private:
	ThreadManager() {};
	Thread ** threads;

	std::size_t num_of_threads;

	std::queue<Thread*> thread_list;
	std::queue<std::unique_ptr<Job>> job_list;
};
