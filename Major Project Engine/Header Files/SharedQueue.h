#pragma once

#ifndef _SHAREDQUEUE_H
#define _SHAREDQUEUE_H

#include <queue>
#include <mutex>

/*
This Shared queue is for both the task manager and the thread manager.
They share this queue so that there is no going back and forther 
between two different queues.
*/
template <class T>
class SharedQueue
{
public:
	SharedQueue() {}
	~SharedQueue() {}

	/*
	Take the first job off the queue
	*/
	void Pop()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		this->_queue.pop();
	}

	/*
	Look at the front job
	*/
	T & Front()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		return _queue.front();
	}

	/*
	Put a new job on the queue
	*/
	void Emplace(T && item)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_queue.emplace(item);
	}

	/*
	Put a new job on the queue
	*/
	void Emplace(T & item)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_queue.emplace(item);
	}

	/*
	Check if empty
	*/
	bool Empty()
	{
		return _queue.empty();
	}

	/*
	Get the size of the queue
	*/
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
