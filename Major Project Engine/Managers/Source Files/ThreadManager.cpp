#include "ThreadManager.h"

ThreadManager::~ThreadManager()
{
	for (std::size_t i = 0; i < num_of_threads; i++)
	{
		delete(threads[i]);
	}

	for (int i = 0; i < task_queue.Size(); i++)
	{
		delete task_queue.Front();
		task_queue.Pop();
	}
}

ThreadManager::ThreadManager(SharedQueue<Job*> & queue, const std::size_t & size)
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
			t_queues[i] = new BlockingQueue<Job*>(any_thread);
			break;
		}
		threads[i] = new Thread(this, *t_queues[i], name, type);
	}
}

void ThreadManager::Close()
{
	StopThreads();
	//PrintJobs();
}

bool ThreadManager::HasJobs() 
{ 
	bool check = false;

	for (const auto queue : t_queues)
	{
		if (queue == nullptr) continue;

		check |= !queue->Empty();
		if (!queue->Empty())
		{
			queue->Alert();
		}
	}
	check |= !task_queue.Empty();

	check |= (jobs_to_finish != 0);

	return check; 
}

void ThreadManager::AllocateJobs(const int num_new_jobs)
{
	{
		std::lock_guard<std::mutex> lock(finished_job);
		jobs_to_finish += num_new_jobs;
	}

	while (!task_queue.Empty())
	{
		switch (task_queue.Front()->j_type / JOB_STRIDE)
		{
		case Job::JOB_RENDER:
			{
				Job * temp = task_queue.Front();
				t_queues[rthread_num]->Emplace(temp);
				task_queue.Pop();
				render_thread.notify_one();
				break;
			}
		default:
			{
				Job * temp = task_queue.Front();
				t_queues[count]->Emplace(temp);
				task_queue.Pop();

				//if (count == 0) render_thread.notify_one();

				if (count >= num_of_threads - 1) count = 1;
				else count++;

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
		if (t != nullptr)
			total += t->PrintLogger(t_framestart);

	printf("Total count is: %u\n", total);
}

void ThreadManager::NewFrame()
{
	for (const auto & t : threads)
		if (t != nullptr)
			t->ClearLogger();
		
	t_framestart = std::chrono::high_resolution_clock::now();
}

void ThreadManager::StopThreads()
{
	for (std::size_t i = 0; i < num_of_threads; i++)
	{
		t_queues[i]->Close();
		threads[i]->Stop();
	}
}
