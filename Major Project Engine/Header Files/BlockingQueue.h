#pragma once

#ifndef _BLOCKINGQUEUE_H
#define _BLOCKINGQUEUE_H

#include <list>
#include <mutex>

template<typename T>
class BlockingQueue
{
public:
	BlockingQueue()
	{
		_queue = std::list<T>();
	}

	~BlockingQueue()
	{
		std::list<T>().swap(_queue);
	}

	T & front()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		return _queue.front();
	}

	void front_to_back()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		T temp = _queue.front();
		_queue.pop_front();
		_queue.push_back(temp);
	}

	bool pop(T & location)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (!_queue.empty())
		{
			if (_queue.front()->get_waiting() <= 0)
			{
				location = _queue.front();
				_queue.pop_front();
			}
			else
			{
				_queue.emplace_back(_queue.front());
				_queue.pop_front();
			}
			return true;
		}
		return false;
	}

	void emplace(T item)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_queue.emplace_back(item);
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
	std::list<T> _queue;
};

#endif // !_BLOCKINGQUEUE_H
