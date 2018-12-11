#include "ThreadManager.h"

ThreadManager::~ThreadManager()
{
	for (std::size_t i = 0; i < num_of_threads; i++)
	{
		delete(threads[i]);
	}
	std::queue<Job*>().swap(task_queue);
}

ThreadManager::ThreadManager(const std::size_t & size)
	: num_of_threads(size)
{
	std::string name;
	for (std::size_t i = 0; i < size; i++)
	{
		Thread::THREAD_TYPE type = Thread::ANY_THREAD;
		switch (i)
		{
		case 0:
			name = "Albert";
			type = Thread::RENDER_THREAD;
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
		default:
			break;
		}
		threads[i] = new Thread(name, type);
	}
	task_queue = std::queue<Job*>();
}

void ThreadManager::Close()
{
	stop_threads();
}

bool ThreadManager::HasJobs() { return !task_queue.empty(); }

void ThreadManager::allocate_jobs()
{
	while (!jobs_empty())
	{
		if (task_queue.front()->get_waiting() != 0)
		{
			Job * temp = task_queue.front();
			task_queue.pop();
			task_queue.push(temp);
			continue;
		}
		for (std::size_t i = 0; i < num_of_threads && !jobs_empty(); i++)
		{
			switch (task_queue.front()->get_type())
			{
			case Job::RENDER_TYPE:
				if (threads[0]->check_availability())
				{
					threads[0]->get_location() = task_queue.front();
					task_queue.front() = nullptr;
					task_queue.pop();
				}
				else
				{
					Job * temp = task_queue.front();
					task_queue.pop();
					task_queue.push(temp);
					temp = nullptr;
					continue;
				}
				break;
			default:
				if (io_thread == threads[i] && io_thread->check_availability())
					io_thread = nullptr;

				if (threads[i]->check_availability())
				{
					threads[i]->get_location() = task_queue.front();
					task_queue.front() = nullptr;
					task_queue.pop();
				}
				break;
			}
		}
	}
}

bool ThreadManager::jobs_empty()
{
	return task_queue.empty();
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

void ThreadManager::get_jobs(std::list<Job*> * job_list)
{
	while (!job_list->empty())
	{
		task_queue.emplace(job_list->front());
		job_list->pop_front();
	}
}
