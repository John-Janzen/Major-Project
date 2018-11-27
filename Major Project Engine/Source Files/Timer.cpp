#include "Timer.h"

Timer::Timer()
{
	start_time = SteadyClock::now();
	frame_rate_control = SteadyClock::now();
	end_time = SteadyClock::now();
}

Timer::~Timer() {}

void Timer::Start()
{
	start_time = SteadyClock::now();
}

void Timer::End()
{
	end_time = SteadyClock::now();
	this->set_delta_time();
	//this->Print(end_time - start_time);
}

const MilliDuration Timer::elapsed_work()
{
	return MilliDuration(start_time - end_time);
}

void Timer::wait_time()
{
	this->Start();
	auto work_time = this->elapsed_work();
	if (work_time < current_time_lock)
	{
		if (MilliDuration(start_time - frame_rate_control) >= MilliDuration(1000))
		{
			frame_rate_control = SteadyClock::now();
			printf("Frames %u\n", frame_count);
			frame_count = 0u;
		}
		frame_count++;
		std::this_thread::sleep_for(current_time_lock - work_time);
		//printf("%d FrameSleep elapsed: %f\n", frame_count, MilliDuration(SteadyClock::now() - start_time).count());
	}
	this->End();
}

void Timer::Print(const MilliDuration & time)
{
	printf("Time ended: %f\n", time.count());
}

void Timer::set_delta_time()
{
	delta_time = MilliDuration(end_time - start_time).count() / 1000.0f;
}
