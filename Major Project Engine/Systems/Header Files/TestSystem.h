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
	TestSystem();
	~TestSystem();

	JOB_RETURN Load(void* content);
	void Close(void* content);

	bool process(void* content);
	bool return_process(void* content);
private:
	std::vector<int> testing_field;
	int results[4];
};

#endif // !_TESTSYSTEM_H
