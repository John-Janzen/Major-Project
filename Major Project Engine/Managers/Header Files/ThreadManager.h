#pragma once

#ifndef _THREADMANAGER_H
#define _THREADMANAGER_H

#include "ThreadDebugger.h"
#include "SharedQueue.h"
#include "EventHandler.h"
#include "GameStates.h"

#include <queue>
#include <array>

/*
This Manager class is created as a singleton because
we don't need multiple instances of the thread manager.
A single class will do as most programs will only engage
with this class by calling RegisterJob.

Furthermore, main thread is going to be the only one to
use most of these functions.
*/
class ThreadManager : public EventListener
{
public:

	ThreadManager(const std::size_t & n_threads, SharedQueue<Job*> & queue);

	/*
	Deletes the threads and job list.
	*/
	~ThreadManager();

	void HandleEvent(const EventType & e, void * data);

	/*
	Stops the threads.
	*/
	void Close();

	bool HasJobs();

	/*
	This function allocates jobs from the threads that are readily available.
	As long as there are jobs to allocate to the threads it will check
	all of the threads available to make sure they are not busy
	before handing a job off the queue.
	*/
	void AllocateJobs(const int num_new_jobs);

	/*
	Prints the stats for the jobs that the threads have
	completed. Along with the number of jobs in total
	that have been completed.
	*/
	void PrintJobs();

	void NewFrame();

	/*
	Stops the threads by calling there stop
	function. Had to make this because a there
	isn't an easy way to destruct a singleton.
	*/
	void StopThreads();

	void RetryJob(Job * job)
	{
		task_queue.Emplace(job);
	}

	void MainThreadJob(Job * && job)
	{
		task_queue.Emplace(job);
	}

	const std::size_t GetNumThreads() const { return n_threads; }

	std::atomic<int> jobs_to_finish = 0;

	/// DEBUGGING SECTION
	void LoadDebugger(const float & rate, const std::size_t & count) { t_debug.LoadDebug(rate, count); }
	void ShowDebugger() { t_debug.ShowDebug(); }
	void RenderDebugger() { t_debug.RenderDebug(); }
	void HideDebugger() { t_debug.HideDebug(); }
	Uint32 GetDebugWindowID() { return t_debug.GetWindowID(); }
	void CheckDebugMouseLoc(const SDL_MouseMotionEvent & mme) { t_debug.CheckMouseLocation(mme); }

private:

	ThreadDebugger t_debug;
	std::uint8_t debug_mode = 0;

	std::array<Thread*, Thread::MAX_THREADS> threads = { nullptr };
	std::array<BlockingQueue<Job*>*, Thread::MAX_THREADS> t_queues = { nullptr };
	std::size_t rt_loc, mt_loc;

	std::condition_variable cv_any, cv_render, cv_main;

	SharedQueue<Job*> & task_queue;
	std::size_t n_threads;
	std::mutex finished_job;

	std::chrono::high_resolution_clock::time_point t_framestart;
	
	int count = 1;
};

#endif // !_THREADMANAGER_H
