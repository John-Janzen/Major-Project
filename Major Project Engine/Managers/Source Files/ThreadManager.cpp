#include "ThreadManager.h"

ThreadManager::ThreadManager(const std::size_t & size, SharedQueue<Job*> & queue)
	: task_queue(queue)
{
	EventHandler::Instance().SubscribeEvent(EventType::JOB_FINISHED, this);
	EventHandler::Instance().SubscribeEvent(EventType::JOB_REENTER, this);
	EventHandler::Instance().SubscribeEvent(EventType::NEW_FRAME, this);
	EventHandler::Instance().SubscribeEvent(EventType::STATE_CHANGE, this);

	n_threads = size;
	std::string name;
	frame_start = std::chrono::high_resolution_clock::now();
	for (std::size_t i = 0; i < size; i++)
	{
		Thread::THREAD_TYPE type = Thread::ANY_THREAD;
		switch (i)
		{
		case 0:
			name = "Albert";
			type = Thread::MAIN_THREAD;
			mt_loc = i;
			break;
		case 1:
			name = "Curie";
			type = Thread::RENDER_THREAD;
			rt_loc = i;
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
}

ThreadManager::~ThreadManager()
{
	for (std::size_t i = 0; i < n_threads; i++)
	{
		delete(threads[i]);
	}

	for (int i = 0; i < task_queue.Size(); i++)
	{
		delete task_queue.Front();
		task_queue.Pop();
	}
}

void ThreadManager::HandleEvent(const EventType & e, void * data)
{
	switch (e)
	{
	case EventType::JOB_FINISHED:
	{
		{
			std::lock_guard<std::mutex> lock(finished_job);
			jobs_to_finish--;
		}
		break;
	}
	case EventType::JOB_REENTER:
	{
		this->RetryJob(static_cast<Job*>(data));
		break;
	}
	case EventType::NEW_FRAME:
	{
		this->NewFrame();
		break;
	}
	case EventType::STATE_CHANGE:
	{
		GAME_STATE gs = *static_cast<GAME_STATE*>(data);

		if (gs == DEBUG_LOAD)
		{
			for (auto thread : threads)
				if (thread != nullptr)
					thread->ToggleDebug();
			debug_mode = 1;
		}
		else if (gs == DEBUG_RUN)
		{
			for (auto thread : threads)
				if (thread != nullptr)
					thread->ToggleDebug();
			debug_mode = 0;
		}
		break;
	}
	default:
		break;
	}
}

void ThreadManager::Close()
{
	this->StopThreads();
}

bool ThreadManager::HasJobs() 
{ 
	bool check = false;

	for (int i = 1; i < n_threads; i++)
	{
		check |= threads[i]->HasJob();
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
		case job::JOB_MAIN:
			threads[mt_loc]->EmplaceNewJob(temp, frame_start);
			task_queue.Pop();
			break;

		case job::JOB_RENDER:
			threads[rt_loc]->EmplaceNewJob(temp, frame_start);
			task_queue.Pop();
			break;

		default:
		{
			int selection = n_threads - 1;
			for (int i = 1; i < n_threads; i++)
			{
				if (threads[i]->GetEndTime() < threads[selection]->GetEndTime())
					selection = i;
			}

			threads[selection]->EmplaceNewJob(temp, frame_start);
			task_queue.Pop();
		}
			break;
		}
	}
}

void ThreadManager::PrintJobs()
{
	int total = 0;
	for (const auto & t : threads)
		if (t != nullptr)
			total += t->PrintLogger(frame_start);

	printf("Total count is: %u\n", total);
}

void ThreadManager::NewFrame()
{
	if (debug_mode == 1)
	{
		for (int i = 0; i < n_threads; i++)
		{
			threads[i]->ClearLogger();
		}
		debug_start = std::chrono::high_resolution_clock::now();
		debug_mode = 2;
	}
	else if (debug_mode == 2)
	{
		t_debug.LoadDebugData(threads, debug_start);
		EventHandler::Instance().SendEvent(EventType::DEBUG_FINISHED_LOAD);
		//this->PrintJobs();
	}

	frame_start = std::chrono::high_resolution_clock::now();

	for (int i = 1; i < n_threads; i++)
	{
		if (threads[i]->GetAllotedTime() != nanoseconds(0) || threads[i]->GetEndTime() < hr::now() && !threads[i]->HasJob())
		{
			threads[i]->ResetAllotedTime();
		}
	}
}

void ThreadManager::StopThreads()
{
	for (std::size_t i = 0; i < n_threads; i++)
	{
		threads[i]->Stop();
	}
}
