#include "ThreadManager.h"

ThreadManager::ThreadManager(const std::size_t & size, SharedQueue<Job*> & queue)
	: task_queue(queue)
{
	num_of_threads = size;
	std::string name;
	for (std::size_t i = 0; i < size; i++)
	{
		Thread::THREAD_TYPE type = Thread::ANY_THREAD;
		switch (i)
		{
		case 0:
			name = "Albert";
			type = Thread::MAIN_THREAD;
			mt_loc = i;
			t_queues[i] = new BlockingQueue<Job*>(cv_main);
			break;
		case 1:
			name = "Curie";
			type = Thread::RENDER_THREAD;
			rt_loc = i;
			t_queues[i] = new BlockingQueue<Job*>(cv_render);
			break;
		case 2:
			name = "Newton";
			t_queues[i] = new BlockingQueue<Job*>(cv_any);
			break;
		case 3:
			name = "Dennis";
			t_queues[i] = new BlockingQueue<Job*>(cv_any);
			break;
		default:
			t_queues[i] = new BlockingQueue<Job*>(cv_any);
			break;
		}
		threads[i] = new Thread(this, *t_queues[i], name, type);
	}
}

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

void ThreadManager::Close()
{
	this->StopThreads();
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

	check |= jobs_to_finish != 0;
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
		Job * temp = task_queue.Front();

		switch (temp->j_type / JOB_STRIDE)
		{
		case Job::JOB_MAIN:
			{
				t_queues[mt_loc]->Emplace(temp);
				task_queue.Pop();
				
				cv_main.notify_one();
				break;
			}
		case Job::JOB_RENDER:
			{
				t_queues[rt_loc]->Emplace(temp);
				task_queue.Pop();

				cv_render.notify_one();
				break;
			}
		default:
			{
				t_queues[count]->Emplace(temp);
				task_queue.Pop();
				
				if (count >= num_of_threads - 1) count = 1;
				else count++;

				t_queues[count]->Alert();
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
