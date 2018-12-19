#include "ThreadManager.h"

ThreadManager::~ThreadManager()
{
	for (std::size_t i = 0; i < num_of_threads; i++)
	{
		delete(threads[i]);
	}

	for (int i = 0; i < task_queue.size(); i++)
	{
		delete task_queue.top();
		task_queue.pop();
	}
	task_queue = std::priority_queue<Job*, std::vector<Job*>, Job>();
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

		if (type == Thread::RENDER_THREAD)
			render_thread = threads[i];

		thread_queue.emplace(threads[i]);
	}
	task_queue = std::priority_queue<Job*, std::vector<Job*>, Job>();
}

void ThreadManager::Close()
{
	StopThreads();
	PrintJobs();
}

bool ThreadManager::HasJobs() { return !task_queue.empty(); }

void ThreadManager::AllocateJobs()
{
	Thread * thread_ptr;
	while (!JobsEmpty())
	{
		thread_ptr = thread_queue.front();
		switch (task_queue.top()->j_type / 0x100)
		{
		case 3:
			if (render_thread->CheckAvailable())
			{
				render_thread->GetLocation() = task_queue.top();
				task_queue.pop();
				render_thread->Notify();
			}
			else
			{
				Job * temp = task_queue.top();
				task_queue.pop();
				task_queue.push(temp);
			}
			break;
		default:
			if (thread_ptr->CheckAvailable())
			{
				thread_ptr->GetLocation() = task_queue.top();
				task_queue.pop();
				thread_ptr->Notify();
			}
			thread_queue.pop();
			thread_queue.push(thread_ptr);
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

std::priority_queue<Job*, std::vector<Job*>, Job> & ThreadManager::GetQueue()
{
	return task_queue;
}
