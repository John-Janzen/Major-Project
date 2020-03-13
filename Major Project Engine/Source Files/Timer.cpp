#include "Timer.h"

Timer::~Timer() {}

void Timer::Init()
{
	current_time_frame = frame_rate_control = hr_clock::now();
}

void Timer::Start()
{
	timer_start = hr_clock::now();
}

void Timer::Stop()
{
	printf("Timer Stopped at ");
	Print(ms(hr_clock::now() - timer_start));
}

void Timer::Restart()
{
	current_time_frame = hr_clock::now();
}

void Timer::WaitTime()
{
	frame_count++;																// INCREMENT FRAME
	if (ms(current_time_frame - frame_rate_control) >= ms(std::chrono::seconds(1)))		// CHECK IF DURATION OF FRAME IS GREATER THAN A SECOND
	{
		std::cout << frame_count << std::endl;
		frame_rate_control = hr_clock::now();									// RESET THE FRAME TIME TO NOW
		frame_count = 0u;														// RESET FRAME COUNT
	}

	if (ms(hr_clock::now() - current_time_frame) < current_time_lock)		// AS LONG AS THE WORK_TIME IS LESS THAN THE FRAMETIME NEEDED (60FPS LOCK) -> (16.66ms)
	{
		while (ms(hr_clock::now() - current_time_frame) < current_time_lock);		// SPINLOCK FROM THE CURRENT FRAMETIME BELOW TO THE BEGINNING OF THE NEXT FRAME 
	}
	this->SetDeltaTime();			// Set the delta time after the sleep
	this->Restart();				// reset the frame time
}

bool Timer::CheckTimeLimit()
{
	return ms(hr_clock::now() - current_time_frame) < current_time_lock;		// check the time against the current time lock
}

void Timer::Print(const ms & time)
{
	printf("%f\n", time.count());
}

void Timer::SetDeltaTime()
{
	this->delta_time = ms(hr_clock::now() - current_time_frame).count() / 1000.f;
}
