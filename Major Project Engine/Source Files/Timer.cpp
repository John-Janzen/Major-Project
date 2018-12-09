#include "Timer.h"

Timer::Timer()
{
	current_time_frame = frame_rate_control = hr_clock::now();
}

Timer::~Timer() {}

void Timer::Restart()
{
	current_time_frame = hr_clock::now();
}

void Timer::wait_time()
{
	// COMES HAPPENS BEFORE (10ms HAVE PASSED)
	if (ms_duration(hr_clock::now() - current_time_frame) < current_time_lock)		// AS LONG AS THE WORK_TIME IS LESS THAN THE FRAMETIME NEEDED (60FPS LOCK) -> (16.66ms)
	{
		frame_count++;																// INCREMENT FRAME
		if (ms_duration(current_time_frame - frame_rate_control) >= ms_duration(std::chrono::seconds(1)))		// CHECK IF DURATION OF FRAME IS GREATER THAN A SECOND
		{
			//printf("Frame Count: %d", frame_count);
			frame_rate_control = hr_clock::now();									// RESET THE FRAME TIME TO NOW
			frame_count = 0u;														// RESET FRAME COUNT
		}
		
		while (true)		// SPINLOCK FROM THE CURRENT FRAMETIME BELOW TO THE BEGINNING OF THE NEXT FRAME
		{
			auto time = ms_duration(hr_clock::now() - current_time_frame);
			if (time > current_time_lock)
				break;
			//std::this_thread::yield();
		}
		//printf("%d FrameSleep elapsed: %f\n", frame_count, std::chrono::duration<double, std::milli>(hr_clock::now() - current_time_frame).count());		// PRINT RESULTS
		this->set_delta_time();
		current_time_frame = hr_clock::now();
	}
	else
	{
		this->Restart();																// OR WE RESTART THE FRAMETIME
	}
}

bool Timer::checkTimeLimit()
{
	return (ms_duration(hr_clock::now() - current_time_frame) < (current_time_lock - std::chrono::milliseconds(1))) ? true : false;
}

void Timer::Print(const ms_duration & time)
{
	printf("Time ended: %f\n", time.count());
}

void Timer::set_delta_time()
{
	delta_time = std::chrono::duration<float, std::milli>(hr_clock::now() - current_time_frame).count() / 1000.0f;
}
