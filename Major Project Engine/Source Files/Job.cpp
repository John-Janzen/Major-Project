#include "Job.h"

/*
* Constructor that initializes type and function.
*/
Job::Job(Job_Type type, std::function<void(Content*)> function, Content * data)
: _type(type), _func(function), _content(data) {}

/*
* Destructor that eliminates data on a job. 
*/
Job::~Job() 
{
	_type = NULL_TYPE;
	_func = NULL;
	delete(_content);
}
