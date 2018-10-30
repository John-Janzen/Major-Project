#pragma once

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

	T pop()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		T value;
		if (!_queue.empty())
		{
			value = std::move(_queue.front());
			_queue.pop();
		}
		return value;
	}

	void emplace(T item)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_queue.emplace(std::move(item));
	}

	bool empty()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		return _queue.empty();
	}

	int size()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		return _queue.size();
	}

private:
	std::mutex _mutex;
	std::queue<T> _queue;
};

