#pragma once

#ifndef _THREADMANAGER_H
#define _THREADMANAGER_H

#include "ThreadDebugger.h"
#include "SharedQueue.h"
#include "EventHandler.h"
#include "GameStates.h"

#include <queue>
#include <array>
#include <cassert>

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

	/*
	Deal with Event as they come in from the event handler
	*/
	void HandleEvent(const EventType & e, void * data);

	/*
	Stops the threads.
	*/
	void Close();

	/*
	Check to see if I have a job anywhere
	*/
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

	/*
	New frame has started
	reset frame time and if debug loading deal with that
	*/
	void NewFrame();

	/*
	Stops the threads by calling there stop function.
	*/
	void StopThreads();

	/*
	Replace the job onto the shared queue for a different job.
	*/
	void RetryJob(Job * job)
	{
		task_queue.Emplace(job);
	}

	/*
	Get the number of threads.
	*/
	const std::size_t GetNumThreads() const { return n_threads; }

	std::atomic<int> jobs_to_finish = 0;

	/// DEBUGGING SECTION
	/*
	Load the debugger with refresh rate and number of threads
	*/
	void LoadDebugger(const float & rate, const std::size_t & count) { t_debug.LoadDebug(rate, count); }

	/*
	Display the window for debugger
	*/
	void ShowDebugger() { t_debug.ShowDebug(); }

	/*
	Render the window for the debugger
	*/
	void RenderDebugger() { t_debug.RenderDebug(); }

	/*
	Hide the window for the debugger
	*/
	void HideDebugger() 
	{ 
		t_debug.HideDebug(); 
		for (int i = 0; i < n_threads; i++)
		{
			threads[i]->ClearLogger();
		}
	}

	/*
	Get the window ID from SDL window
	*/
	Uint32 GetDebugWindowID() { return t_debug.GetWindowID(); }

	/*
	Check to see if mouse is inside debug item
	*/
	void CheckDebugMouseLoc(const SDL_MouseMotionEvent & mme) { t_debug.CheckMouseLocation(mme); }

private:

	ThreadDebugger t_debug;			// Debugger
	std::uint8_t debug_mode = 0;	// step by step debug loader flag

	std::array<Thread*, Thread::MAX_THREADS> threads = { nullptr };
	std::size_t rt_loc, mt_loc;		// render thread location and main thread location in std::array for easy access

	SharedQueue<Job*> & task_queue;
	std::size_t n_threads;
	std::mutex finished_job;

	std::chrono::high_resolution_clock::time_point debug_start, frame_start;
	
	int count = 1;
};

#endif // !_THREADMANAGER_H
