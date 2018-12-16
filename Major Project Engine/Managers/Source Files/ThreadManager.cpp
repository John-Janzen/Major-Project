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
		thread_queue.emplace(threads[i]);
	}
	task_queue = std::queue<Job*>();
}

void ThreadManager::Close()
{
	StopThreads();
}

bool ThreadManager::HasJobs() { return !task_queue.empty(); }

void ThreadManager::AllocateJobs()
{
	Thread * thread_ptr;
	while (!JobsEmpty())
	{
		thread_ptr = thread_queue.front();
		switch (task_queue.front()->GetType())
		{
		case Job::RENDER_TYPE:
			if (thread_ptr->GetType() == Thread::RENDER_THREAD)
			{
				if (thread_ptr->CheckAvailable())
				{
					thread_ptr->GetLocation() = task_queue.front();
					task_queue.pop();
				}
				else
				{
					Job * temp = task_queue.front();
					task_queue.pop();
					task_queue.push(temp);
					thread_queue.pop();
					thread_queue.push(thread_ptr);
				}
			}
			else
			{
				thread_queue.pop();
				thread_queue.push(thread_ptr);
			}
			break;
		default:
			if (thread_ptr->CheckAvailable())
			{
				thread_ptr->GetLocation() = task_queue.front();
				task_queue.pop();
			}
			else
			{
				thread_queue.pop();
				thread_queue.push(thread_ptr);
			}
			break;
		}
	}
}

bool ThreadManager::JobsEmpty()
{
	return task_queue.empty();
}

void ThreadManager::PrintJobs()
{
	int total = 0;
	for (std::size_t i = 0; i < num_of_threads; i++)
		total += threads[i]->PrintStats();
	printf("Total count is: %u\n", total);
}

void ThreadManager::StopThreads()
{
	for (std::size_t i = 0; i < num_of_threads; i++)
		threads[i]->Stop();
}

void ThreadManager::GetJobs(std::list<Job*> * job_list)
{
	while (!job_list->empty())
	{
		task_queue.emplace(job_list->front());
		job_list->pop_front();
	}
}
