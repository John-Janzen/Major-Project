#pragma once

#ifndef _JOB_H
#define _JOB_H

#include <functional>
#include <memory>
#include <atomic>
#include <vector>

enum JOB_RETURN
{
	JOB_COMPLETED,
	JOB_RETRY,
	JOB_ISSUE
};

//typedef bool(*JobFunction)(void* &);
using JobFunction = std::function<JOB_RETURN(void*)>;

template<class T>
JobFunction bind_function(JOB_RETURN(T::* pFunc)(void*), T * const sys = nullptr)
{
	return std::bind(pFunc, sys, std::placeholders::_1);
}

const std::uint16_t JOB_STRIDE = 0x100;

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
public:

	/*
	* List of Job Types that the threads will
	* run. This will provide a verious number
	* of jobs and a priority later down the line.
	*/
	enum JOB_ID
	{
		JOB_NULL = 0x000,
		JOB_MISC,
		JOB_PHYSICS,
		JOB_INPUT,
		JOB_RENDER,

		JOB_DEFAULT = JOB_STRIDE * JOB_MISC,
		JOB_LOAD_MODEL,
		JOB_LOAD_TEXTURE,
		JOB_MODEL_CHECKER,
		JOB_TEXTURE_CHECKER,
		JOB_SHADER_CHECKER,

		JOB_PHYSICS_DEFAULT = JOB_STRIDE * JOB_PHYSICS,
		JOB_PHYSICS_LOAD,
		JOB_PHYSICS_UPDATE,
		JOB_PHYSICS_COMPONENT,

		JOB_INPUT_DEFAULT = JOB_STRIDE * JOB_PHYSICS,
		JOB_INPUT_LOAD,
		JOB_INPUT_UPDATE,

		JOB_RENDER_DEFAULT = JOB_STRIDE * JOB_RENDER,
		JOB_RENDER_LOAD,
		JOB_RENDER_UPDATE,
		JOB_BIND_MODEL,
		JOB_BIND_TEXTURE,
		JOB_BIND_SHADER,
		JOB_LOAD_SHADER,

		JOB_HEAD_END
	};

	Job(JobFunction function, const std::string name, void* data = nullptr, const Job::JOB_ID type = Job::JOB_DEFAULT, Job * parent = nullptr)
		: _func(function), job_name(name), _content(data), j_type(type), _parent_job(parent) {}

	Job() : _content(nullptr), j_type(JOB_DEFAULT), _parent_job(nullptr) {}

	~Job()
	{
		_func = NULL;
		if (_content != nullptr) _content = nullptr;
		if (_parent_job != nullptr)
		{
			_parent_job->_awaiting--;
			_parent_job = nullptr;
		}
		//if (f_time_data != nullptr) f_time_data = nullptr;
	}

	JOB_RETURN operator()()
	{
		return _func(_content);
	}

	bool operator()(Job * const & lhs, Job * const & rhs)
	{
		return lhs->_scale < rhs->_scale;
	}

	std::string job_name;
	
	JOB_ID j_type;
	float _scale = 0;
	int _age = 0;

	int _awaiting = 0;
	Job * _parent_job = nullptr;
	
	JobFunction _func;
	void* _content;

};

#endif // !_JOB_H
