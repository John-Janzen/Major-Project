#pragma once
#include <functional>

/*
* List of Job Types that the threads will
* run. This will provide a verious number
* of jobs and a priority later down the line.
*/
enum Job_Type
{
	NULL_TYPE,
};

/*
* Job class that holds the data for the threads 
* to read and work on. 
*
* Job type as stated above will govern what a job will do.
*
* Function gives the thread an actual function to work on - However,
* this is extremely limiting as all jobs currently have to work
* at a void ?function? () architecture.
*
* (Will need to develop further)
*/
class Job
{
public:

	Job(Job_Type, std::function<void()>);
	~Job();

	/* Gets the type of the job */
	Job_Type get_type()
	{
		return _type;
	}

	/* Gets the function of the job */
	std::function<void()> get_function()
	{
		return _func;
	}
private:
	Job_Type _type;
	std::function<void()> _func;
};

