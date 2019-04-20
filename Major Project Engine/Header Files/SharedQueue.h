#pragma once

#ifndef _SHAREDQUEUE_H
#define _SHAREDQUEUE_H

#include "Job.h"

#include <queue>
#include <mutex>

template <class T>
class SharedQueue
{
public:
	SharedQueue() {}
	~SharedQueue() {}

	void Pop()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		this->_queue.pop();
	}

	T & Front()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		return _queue.front();
	}

	void Emplace(T && item)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_queue.emplace(item);
	}

	void Emplace(T & item)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_queue.emplace(item);
	}

	bool Empty()
	{
		return _queue.empty();
	}

	const std::size_t Size()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		return _queue.size();
	}

private:
	std::queue<Job*> _queue;
	std::mutex _mutex;
};

#endif // !_SHAREDQUEUE_H
