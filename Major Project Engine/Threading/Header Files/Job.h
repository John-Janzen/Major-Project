#pragma once

#ifndef _JOB_H
#define _JOB_H

#include <functional>
#include <memory>
#include <atomic>
#include <vector>
#include <array>

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

	static const int MAX_PARENTS = 10;

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
		JOB_PHYSICS_PREUPDATE,
		JOB_PHYSICS_UPDATE,
		JOB_PHYSICS_COMPONENT,

		JOB_INPUT_DEFAULT = JOB_STRIDE * JOB_INPUT,
		JOB_INPUT_LOAD,
		JOB_INPUT_UPDATE,

		JOB_RENDER_DEFAULT = JOB_STRIDE * JOB_RENDER,
		JOB_RENDER_LOAD,
		JOB_RENDER_UPDATE,
		JOB_BIND_MODEL,
		JOB_BIND_TEXTURE,
		JOB_BIND_SHADER,
		JOB_LOAD_SHADER,
		JOB_SWAP_BUFFERS,

		JOB_HEAD_END
	};

	/*void TransferJobID(const Job::JOB_ID & id)
	{
		switch (id)
		{

		case JOB_LOAD_MODEL:
			std::cout << "Load Model\n";
			break;
		case JOB_LOAD_TEXTURE:
			std::cout << "Load Model\n";
			break;
		case JOB_MODEL_CHECKER:
			std::cout << "Load Model\n";
			break;
		case JOB_TEXTURE_CHECKER:
			std::cout << "Load Model\n";
			break;
		case JOB_SHADER_CHECKER:
			std::cout << "Load Model\n";
			break;

		case JOB_PHYSICS_LOAD:
			std::cout << "Load Model\n";
			break;
		case JOB_PHYSICS_PREUPDATE:
			std::cout << "Load Model\n";
			break;
		case JOB_PHYSICS_UPDATE:
			std::cout << "Load Model\n";
			break;
		case JOB_PHYSICS_COMPONENT:
			std::cout << "Load Model\n";
			break;

		case JOB_INPUT_LOAD:
			std::cout << "Load Model\n";
			break;
		case JOB_INPUT_UPDATE:
			std::cout << "Load Model\n";
			break;

		case JOB_RENDER_LOAD:
			std::cout << "Load Model\n";
			break;
		case JOB_RENDER_UPDATE:
			break;
		case JOB_BIND_MODEL:
			break;
		case JOB_BIND_TEXTURE:
			break;
		case JOB_BIND_SHADER:
			break;
		case JOB_LOAD_SHADER:
			break;
		case JOB_SWAP_BUFFERS:
			break;
		default:
			break;
		}
	}*/


	Job(JobFunction function, const std::string name, void* data = nullptr, const Job::JOB_ID type = Job::JOB_DEFAULT, Job * parent = nullptr)
		: _func(function), job_name(name), _content(data), j_type(type) 
	{
		if (parent != nullptr)
		{
			_parent_jobs[parent_count] = parent;
			parent_count++;
		}
	}

	Job() : _content(nullptr), j_type(JOB_DEFAULT) {}

	~Job()
	{
		_func = NULL;
		if (_content != nullptr) _content = nullptr;
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
		return lhs->_scale < rhs->_scale;
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
	JOB_ID j_type;
	float _scale = 0;
	int _age = 0;

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
