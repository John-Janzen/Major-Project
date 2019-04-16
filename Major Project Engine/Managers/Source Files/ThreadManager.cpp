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

ThreadManager::ThreadManager(std::priority_queue<Job*, std::vector<Job*>, Job> & queue, const std::size_t & size)
	: task_queue(queue), num_of_threads(size)
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
			rthread_num = i;
			t_queues[i] = new BlockingQueue<Job*>(render_thread);
			break;
		case 1:
			name = "Curie";
			t_queues[i] = new BlockingQueue<Job*>(any_thread);
			break;
		case 2:
			name = "Newton";
			t_queues[i] = new BlockingQueue<Job*>(any_thread);
			break;
		case 3:
			name = "Dennis";
			t_queues[i] = new BlockingQueue<Job*>(any_thread);
			break;
		default:
			break;
		}
		threads[i] = new Thread(*t_queues[i], name, type);
	}
}

void ThreadManager::Close()
{
	StopThreads();
	PrintJobs();
}

bool ThreadManager::HasJobs() { return !task_queue.empty(); }

void ThreadManager::AllocateJobs()
{
	while (!task_queue.empty())
	{
		switch (task_queue.top()->j_type / JOB_STRIDE)
		{
		case Job::JOB_RENDER:
			{
				Job * temp = task_queue.top();
				t_queues[rthread_num]->Emplace(temp);
				task_queue.pop();
				render_thread.notify_one();
				break;
			}
		default:
			{
				Job * temp = task_queue.top();
				t_queues[count]->Emplace(temp);
				task_queue.pop();

				if (count >= num_of_threads - 1) count = 0;
				else count++;

				if (count == 0) render_thread.notify_one();

				any_thread.notify_one();
				break;
			}
		}
	}
}

void ThreadManager::PrintJobs()
{
	int total = 0;
	for (const auto & t : threads)
		total += t->PrintStats();
	printf("Total count is: %u\n", total);
}

void ThreadManager::StopThreads()
{
	for (std::size_t i = 0; i < num_of_threads; i++)
	{
		t_queues[i]->Close();
		threads[i]->Stop();
	}
}
