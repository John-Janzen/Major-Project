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

	T & Front()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		return _queue.front();
	}

	void FrontToBack()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		T temp = _queue.front();
		_queue.pop_front();
		_queue.push_back(temp);
	}

	bool Pop(T & location)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (!_queue.empty())
		{
			if (_queue.front()->GetWaiting() <= 0)
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

	void Emplace(T item)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_queue.emplace_back(item);
	}

	bool Empty()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		return _queue.empty();
	}

	std::size_t Size()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		return _queue.size();
	}

	

private:
	std::mutex _mutex;
	std::list<T> _queue;
};

#endif // !_BLOCKINGQUEUE_H
