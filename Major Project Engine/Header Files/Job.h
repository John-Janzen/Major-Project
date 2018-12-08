#pragma once

#ifndef _JOB_H
#define _JOB_H

#include "Content.h"

#include <functional>
#include <memory>
#include <atomic>
#include <vector>

/*
* List of Job Types that the threads will
* run. This will provide a verious number
* of jobs and a priority later down the line.
*/
enum JOB_TYPE
{
	NULL_TYPE,
	ANY_TYPE,
	RENDER_TYPE,
	IO_TYPE,
};

//typedef bool(*JobFunction)(void* &);
using JobFunction = std::function<bool(void*)>;

template<class T>
JobFunction bind_function(bool(T::* pFunc)(void*), T * const sys = nullptr)
{
	return std::bind(pFunc, sys, std::placeholders::_1);
}

/*
* Job class that holds the data for the threads
* to read and work on.
*
* Job type as stated above will govern what a job will do.
*
* Function gives the thread an actual function to work on - However,
* this is extremely limiting as all jobs currently have to work
* at a void ?function? (Content*) architecture.
*
* (Will need to develop further)
*/
class Job
{
public:

	Job(JobFunction function, void* data = nullptr, const JOB_TYPE type = ANY_TYPE)
		: _func(function), _content(data), j_type(type)
	{
	}

	~Job()
	{
		_func = NULL;
		if (_content != nullptr) _content = nullptr;
		if (_parent_job != nullptr)
		{
			_parent_job->OnNotify();
			_parent_job = nullptr;
		}
	}

	/* Gets the function of the job */
	JobFunction get_function() { return _func; }

	void* get_content() { return _content; }

	const JOB_TYPE get_type() { return j_type; }

	void set_parent(Job * & parent)
	{
		_parent_job = parent;
	}

	void increment_wait()
	{
		_awaiting++;
	}

	void OnNotify()
	{
		_awaiting--;
	}

	std::atomic_int & get_waiting()
	{
		return _awaiting;
	}

private:

	JOB_TYPE j_type;
	std::atomic<int> _awaiting = 0;
	Job * _parent_job;
	
	JobFunction _func;
	void* _content;

};

#endif // !_JOB_H
