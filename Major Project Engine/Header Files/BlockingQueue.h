#pragma once

#ifndef _BLOCKINGQUEUE_H
#define _BLOCKINGQUEUE_H

#include <queue>
#include <mutex>

template<typename T>
class BlockingQueue
{
public:
	BlockingQueue()
	{
		_queue = std::queue<T>();
	}
	~BlockingQueue()
	{
		std::queue<T>().swap(_queue);
	}

	bool pop(T & location)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (!_queue.empty())
		{
			if (_queue.front()->get_waiting() <= 0)
			{
				location = std::move(_queue.front());
				_queue.front() = nullptr;
				_queue.pop();
			}
			else
			{
				_queue.emplace(std::move(_queue.front()));
				_queue.front() = nullptr;
				_queue.pop();
			}
			return true;
		}
		return false;
	}

	void emplace(T item)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_queue.emplace(item);
	}

	bool empty()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		return _queue.empty();
	}

	std::size_t size()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		return _queue.size();
	}

private:
	std::mutex _mutex;
	std::queue<T> _queue;
};

#endif // !_BLOCKINGQUEUE_H
