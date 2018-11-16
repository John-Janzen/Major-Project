#include "ThreadManager.h"

ThreadManager::~ThreadManager()
{
	for (std::size_t i = 0; i < num_of_threads; i++)
	{
		delete(threads[i]);
	}
	delete(job_list);
}

void ThreadManager::Init(const std::size_t & size)
{
	num_of_threads = size;
	threads = new Thread*[size];
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
	}
	job_list = new BlockingQueue<std::unique_ptr<Job>>();
}

void ThreadManager::Close()
{
	stop_threads();
}

void ThreadManager::allocate_jobs()
{
	for (std::size_t i = 0; i < num_of_threads && !jobs_empty(); i++)
	{
		if (threads[i]->check_availability())
		{
			job_list->pop(*threads[i]->get_location());
		}
	}
}

void ThreadManager::register_job(Job_Type type, std::function<void(const std::shared_ptr<Content> &)> function)
{
	job_list->emplace(std::make_unique<Job>(type, function));
}


void ThreadManager::register_job(std::unique_ptr<Job> job)
{
	job_list->emplace(std::move(job));
}

bool ThreadManager::jobs_empty()
{
	return job_list->empty();
}

bool ThreadManager::jobs_full()
{
	return job_list->size() > 20 ? true : false;
}

void ThreadManager::print_total_jobs()
{
	int total = 0;
	for (std::size_t i = 0; i < num_of_threads; i++)
		total += threads[i]->print_stats();
	printf("Total count is: %u\n", total);
}

void ThreadManager::stop_threads()
{
	for (std::size_t i = 0; i < num_of_threads; i++)
		threads[i]->Stop();
}