#pragma once
#include <thread>

class Thread
{
public:
	Thread();
	~Thread();

private:
	std::unique_ptr<std::thread> thread;
};

