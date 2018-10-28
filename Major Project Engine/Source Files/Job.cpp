#include "Job.h"

/*
* Constructor that initializes type and function.
*/
Job::Job(Job_Type type, std::function<void()> function)
: _type(type), _func(function){}

/*
* Destructor that eliminates data on a job. 
*/
Job::~Job() 
{
	_type = NULL_TYPE;
	_func = NULL;
}
