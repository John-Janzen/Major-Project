#pragma once

#ifndef _JOB_H
#define _JOB_H

#include <functional>
#include <atomic>
#include <array>
#include <chrono>

enum JOB_RETURN
{
	JOB_COMPLETED,
	JOB_RETRY,
	JOB_ISSUE
};

//typedef bool(*JobFunction)(void* &);
using JobFunction = std::function<JOB_RETURN(void*)>;
typedef std::chrono::high_resolution_clock hr;
typedef hr::time_point hr_tp;
//using milliseconds = std::chrono::duration<float, std::milli>;

using nanoseconds = std::chrono::nanoseconds;

template<class T>
JobFunction bind_function(JOB_RETURN(T::* pFunc)(void*), T * const sys = nullptr)
{
	return std::bind(pFunc, sys, std::placeholders::_1);
}

const std::uint16_t JOB_STRIDE = 0x100;

namespace job
{
	/*
	* List of Job Types that the threads will
	* run. This will provide a verious number
	* of jobs and a priority later down the line.
	*/
	enum JOB_ID
	{
		JOB_NULL = 0x000,
		JOB_MAIN,
		JOB_MISC,
		JOB_PHYSICS,
		JOB_INPUT,
		JOB_RENDER,

		JOB_MAIN_DEFAULT = JOB_STRIDE * JOB_MAIN,
		JOB_APPLICATION_UPDATE,
		JOB_TILL_NEXT_FRAME,
		JOB_MAIN_COUNT,

		JOB_DEFAULT = JOB_STRIDE * JOB_MISC,
		JOB_LOAD_MODEL,
		JOB_LOAD_TEXTURE,
		JOB_MODEL_CHECKER,
		JOB_TEXTURE_CHECKER,
		JOB_SHADER_CHECKER,
		JOB_DEFAULT_COUNT,

		JOB_PHYSICS_DEFAULT = JOB_STRIDE * JOB_PHYSICS,
		JOB_PHYSICS_LOAD,
		JOB_PHYSICS_LOAD_SINGLE,
		JOB_PHYSICS_UPDATE,
		JOB_PHYSICS_COMPONENT,
		JOB_PHYSICS_COLLISION_DETECTION,
		JOB_PHYSICS_COUNT,

		JOB_INPUT_DEFAULT = JOB_STRIDE * JOB_INPUT,
		JOB_INPUT_LOAD,
		JOB_INPUT_UPDATE,
		JOB_INPUT_COUNT,

		JOB_RENDER_DEFAULT = JOB_STRIDE * JOB_RENDER,
		JOB_RENDER_THREADING_CONTEXT,
		JOB_RENDER_LOAD,
		JOB_RENDER_LOAD_SINGLE,
		JOB_RENDER_UPDATE,
		JOB_BIND_MODEL,
		JOB_BIND_TEXTURE,
		JOB_BIND_SHADER,
		JOB_LOAD_SHADER,
		JOB_SWAP_BUFFERS,
		JOB_RENDER_COUNT,

		JOB_HEAD_END
	};
}

/*
* Job class that holds the data for the threads 
* to read and work on.
*
* Job type as stated above will govern what a job will do.
*
* Function gives the thread an actual function to work on - However,
* this is extremely limiting as all jobs currently have to work
* at a JOB_RETURN ?function? (Content*) architecture.
*
* (Will need to develop further)
*/
struct Job
{
	static const int MAX_PARENTS = 32;

	Job
	(
		JobFunction function, 
		const std::string name, 
		void* data = nullptr, 
		const job::JOB_ID type = job::JOB_DEFAULT
	)
		: _func(function), 
		job_name(name), 
		_content(data), 
		j_type(type) 
	{}

	Job() : _content(nullptr), j_type(job::JOB_DEFAULT) {}

	~Job()
	{
		_func = NULL;
		if (_content != nullptr) _content = nullptr;
		job_name.clear();
		for (auto & parent : _parent_jobs)
		{
			if (parent != nullptr)
			{
				parent->_awaiting--;
				parent = nullptr;
			}
		}
	}

	JOB_RETURN operator()()
	{
		return _func(_content);
	}

	bool operator()(Job * const & lhs, Job * const & rhs)
	{
		return lhs->s_data.time_span < rhs->s_data.time_span;
	}

	void AddParent(Job * & job)
	{
		_parent_jobs[parent_count] = job;
		parent_count++;
		job->_awaiting++;
		job->reason_waiting = true;
	}

	// NAME
	std::string job_name;
	
	// SCHEDULING TOOLS
	job::JOB_ID j_type;
	struct ScheduleData
	{
		nanoseconds time_span;
		std::chrono::high_resolution_clock::time_point start_time, end_time;
	} s_data;

	// JOB WAITING TOOLS
	std::atomic_int _awaiting = 0;
	std::atomic_uint parent_count = 0;
	std::array<Job*, MAX_PARENTS> _parent_jobs = { nullptr };
	bool reason_waiting = false;
	
	// JOB FUNCTIONALITY
	JobFunction _func;
	void* _content;
};

#endif // !_JOB_H
