#pragma once

#ifndef _BLOCKINGQUEUE_H
#define _BLOCKINGQUEUE_H

#include <queue>
#include <mutex>

template<typename T>
class BlockingQueue
{
public:
	BlockingQueue(std::condition_variable & cv)
		: _cv(cv)
	{
		_queue = std::queue<T>();
	}

	~BlockingQueue()
	{
		std::queue<T>().swap(_queue);
	}

	void Close()
	{
		open = false;
		_cv.notify_all();
	}

	void Aquire(T & loc)
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_cv.wait(lock, [this, &loc] {
			if (!this->Empty())
			{
				loc = std::move(this->_queue.front());
				this->_queue.pop();
				return true;
			}
			if (!open)
			{
				return true;
			}
			return false;
		});
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

	std::size_t Size()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		return _queue.size();
	}

	void Alert()
	{
		_cv.notify_all();
	}

private:
	std::mutex _mutex;
	std::condition_variable & _cv;
	std::queue<T> _queue;

	bool open = true;
};

#endif // !_BLOCKINGQUEUE_H
