#pragma once

#ifndef _TESTSYSTEM_H
#define _TESTSYSTEM_H

#include "System.h"
#include "ThreadManager.h"

#include <vector>
#include <memory>
#include <time.h>

class TestSystem :
	public System
{
public:
	TestSystem(TaskManager & tm, SceneManager & sm, EventHandler & eh);
	~TestSystem();

	bool Load();
	void Close(void* content);

	void HandleEvent(const EventType & e, void * data);

	JOB_RETURN Update(void * ptr) { return JOB_COMPLETED; }

	bool Process(void* content);
	bool ReturnProcess(void* content);
private:
	std::vector<int> testing_field;
	int results[4];
};

#endif // !_TESTSYSTEM_H
