#pragma once

#ifndef _JOB_H
#define _JOB_H

#include "Content.h"
#include "System.h"

#include <functional>
#include <memory>
#include <atomic>

/*
* List of Job Types that the threads will
* run. This will provide a verious number
* of jobs and a priority later down the line.
*/
enum Job_Type
{
	NULL_TYPE,
};

constexpr auto MAX_OBSERVERS = 2;

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

	Job(Job_Type type, std::function<void(const std::shared_ptr<Content> &)> function, std::shared_ptr<Content> data = nullptr)
		: _type(type), _func(function), _content(data) 
	{
		
	}

	~Job()
	{
		_type = NULL_TYPE;
		_func = NULL;
		_content.reset();
		_content = nullptr;
		for (int i = 0; i < num_observers; i++)
		{
			_observers[i] = nullptr;
		}
		num_observers = 0;
		_awaiting = 0;
	}

	/* Gets the type of the job */
	Job_Type get_type()
	{
		return _type;
	}

	/* Gets the function of the job */
	std::function<void(const std::shared_ptr<Content> &)> get_function()
	{
		return _func;
	}

	std::shared_ptr<Content> & get_content()
	{
		return _content;
	}

	void add_observer(std::unique_ptr<Job> & awaiting_job)
	{
		_observers[num_observers] = awaiting_job.get();
		num_observers++;
	}

	void notify()
	{
		for (int i = 0; i < num_observers; i++)
		{
			if (_observers[i] != nullptr)
			{
				_observers[i]->OnNotify(*this);
			}
		}
		num_observers = 0;
	}

	void OnNotify(const Job & job)
	{
		
	}
	System * _system;
private:

	Job * _observers[MAX_OBSERVERS] = { nullptr };
	std::atomic_int8_t num_observers = 0, _awaiting = 0;

	
	Job_Type _type;
	std::function<void(const std::shared_ptr<Content> &)> _func;
	std::shared_ptr<Content> _content;

};

#endif // !_JOB_H
