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
	std::string name;
	for (std::size_t i = 0; i < size; i++)
	{
		THREAD_TYPE type = ANY_THREAD;
		switch (i)
		{
		case 0:
			name = "Albert";
			type = RENDER_THREAD;
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
	job_list = new BlockingQueue<Job*>();
}

void ThreadManager::Close()
{
	stop_threads();
}

void ThreadManager::allocate_jobs()
{
	while (!jobs_empty())
	{
		for (std::size_t i = 0; i < num_of_threads && !jobs_empty(); i++)
		{
			switch (job_list->front()->get_type())
			{
			case RENDER_TYPE:
				if (threads[0]->check_availability())
				{
					job_list->pop(threads[0]->get_location());
				}
				else
				{
					job_list->front_to_back();
					continue;
				}
				break;
			case IO_TYPE:
				if (io_thread == nullptr)
				{
					if (threads[i]->check_availability())
					{
						io_thread = threads[i];
						job_list->pop(io_thread->get_location());
					}
					else 
					{

					}
				}
				else
				{
					if (io_thread->check_availability())
					{
						job_list->pop(io_thread->get_location());
					}
				}
				break;
			default:
				if (io_thread == threads[i] && io_thread->check_availability())
					io_thread = nullptr;

				if (threads[i]->check_availability())
				{
					job_list->pop(threads[i]->get_location());
				}
				break;
			}
		}
	}
}

void ThreadManager::register_job(JobFunction function, void* content, const JOB_TYPE type)
{
	job_list->emplace(new Job(function, content, type));
}


void ThreadManager::register_job(Job * job)
{
	job_list->emplace(job);
	job = nullptr;
}

void ThreadManager::register_job(Job * job, Job * parent_job)
{
	parent_job->increment_wait();
	job->set_parent(parent_job);
	job_list->emplace(job);
	job = nullptr;
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
