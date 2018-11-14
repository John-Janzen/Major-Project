#pragma once

#ifndef _TESTSYSTEM_H
#define _TESTSYSTEM_H

#include "System.h"
#include "Content.h"
#include "ThreadManager.h"

#include <vector>
#include <memory>
#include <time.h>

class TestSystem :
	public System
{
public:
	TestSystem(const std::shared_ptr<ThreadManager> & thread_manager);
	~TestSystem();


	bool Load();
	void Close();

	void process(const std::shared_ptr<Content> & content);
private:
	std::vector<int> testing_field;
	std::shared_ptr<ThreadManager> t_manager;
};

#endif // !_TESTSYSTEM_H
