#include "Job.h"

Job::Job(Job_Type type, std::function<void()> function)
: _type(type), _func(function){}

Job::~Job() {}
