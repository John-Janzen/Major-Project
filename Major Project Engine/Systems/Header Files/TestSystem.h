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
	TestSystem(TaskManager & tm);
	~TestSystem();

	bool Load(SceneManager * & sm);
	void Close(void* content);

	void Update(SceneManager * & sm) {}

	bool Process(void* content);
	bool ReturnProcess(void* content);
private:
	std::vector<int> testing_field;
	int results[4];
};

#endif // !_TESTSYSTEM_H
