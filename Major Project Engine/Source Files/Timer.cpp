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

	// COMES HAPPENS BEFORE (10ms HAVE PASSED)
	if (ms(hr_clock::now() - current_time_frame) < current_time_lock)		// AS LONG AS THE WORK_TIME IS LESS THAN THE FRAMETIME NEEDED (60FPS LOCK) -> (16.66ms)
	{
		//printf("Timer: %f\n", ms(hr_clock::now() - current_time_frame).count());
		while (true)		// SPINLOCK FROM THE CURRENT FRAMETIME BELOW TO THE BEGINNING OF THE NEXT FRAME
		{
			auto time = ms(hr_clock::now() - current_time_frame);
			if (time > current_time_lock)
				break;
			//std::this_thread::yield();
		}
		//printf("Timer: %f\n", ms(hr_clock::now() - current_time_frame).count());
		//printf("%d FrameSleep elapsed: %f\n", frame_count, std::chrono::duration<double, std::milli>(hr_clock::now() - current_time_frame).count());		// PRINT RESULTS
	}
	this->SetDeltaTime();
	this->Restart();
}

bool Timer::CheckTimeLimit()
{
	//Print(ms(hr_clock::now() - current_time_frame));
	return ms(hr_clock::now() - current_time_frame) < current_time_lock;
}

void Timer::Print(const ms & time)
{
	printf("%f\n", time.count());
}

void Timer::SetDeltaTime()
{
	this->delta_time = ms(hr_clock::now() - current_time_frame).count() / 1000.f;
}
