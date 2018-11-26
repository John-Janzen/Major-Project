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

	bool Load();
	void Close();

	bool process(Content * & content);
	bool return_process(Content * & content);
private:
	std::vector<int> testing_field;
	int * results[4];
};

#endif // !_TESTSYSTEM_H
