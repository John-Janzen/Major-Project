#include "Timer.h"

Timer::Timer()
{
	start_time = std::chrono::steady_clock::now();
	end_time = std::chrono::steady_clock::now();
}

Timer::~Timer() {}

void Timer::Start()
{
	start_time = std::chrono::steady_clock::now();
}

void Timer::End()
{
	end_time = std::chrono::steady_clock::now();
	this->set_delta_time();
	//this->Print(end_time - start_time);
}

const std::chrono::steady_clock::rep & Timer::elapsed_work()
{
	return std::chrono::duration<double, std::milli>(start_time - end_time).count();
}

void Timer::wait_time()
{
	this->Start();
	if (this->elapsed_work() < current_time_lock)
	{
		std::chrono::duration<double, std::milli> delta_ms(current_time_lock - this->elapsed_work());
		auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
		std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
	}
	this->End();
}

void Timer::Print(const std::chrono::duration<double, std::milli> & time)
{
	printf("Time ended: %f\n", std::chrono::duration<double, std::milli>(time.count()));
}

void Timer::set_delta_time()
{
	using ms = std::chrono::duration<float, std::milli>;
	delta_time = std::chrono::duration_cast<ms>(end_time - start_time).count() / 1000.0f;
	//delta_time = std::chrono::duration_cast<std::chrono::microseconds>(start_time - end_time).count() / 1000.0f;
}
