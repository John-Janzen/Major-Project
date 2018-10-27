#pragma once
#include <functional>

enum Job_Type
{
	NULL_TYPE,
};

class Job
{
public:
	Job();
	~Job();

	Job_Type get_type()
	{
		return _type;
	}

	std::function<void()> get_function()
	{
		return _func;
	}
private:
	Job_Type _type;
	std::function<void()> _func;
};

