#include "Timer.h"

Timer::Timer()
{
	current_time_frame = frame_rate_control = d_start = d_end = hr_clock::now();
}

Timer::~Timer() {}

void Timer::Restart()
{
	current_time_frame = hr_clock::now();
}

void Timer::Start()
{
	d_start = hr_clock::now();
}

void Timer::End()
{
	d_end = hr_clock::now();
	this->set_delta_time();
	//this->Print(end_time - start_time);
}

void Timer::wait_time()
{
	this->Start();
	// COMES HAPPENS BEFORE (10ms HAVE PASSED)
	if (mil_duration(hr_clock::now() - current_time_frame) < current_time_lock)		// AS LONG AS THE WORK_TIME IS LESS THAN THE FRAMETIME NEEDED (60FPS LOCK) -> (16.66ms)
	{
		frame_count++;																										// INCREMENT FRAME
		if (mil_duration(current_time_frame - frame_rate_control) >= mil_duration(std::chrono::seconds(1)))		// CHECK IF DURATION OF FRAME IS GREATER THAN A SECOND
		{
			frame_rate_control = hr_clock::now();									// RESET THE FRAME TIME TO NOW
			frame_count = 0u;														// RESET FRAME COUNT
		}
		std::this_thread::sleep_until(current_time_frame += current_time_lock);		// SLEEP FROM THE CURRENT FRAMETIME BELOW TO THE BEGINNING OF THE NEXT FRAME
		printf("%d FrameSleep elapsed: %f\n", frame_count, std::chrono::duration<double, std::milli>(hr_clock::now() - current_time_frame).count());		// PRINT RESULTS
	}
	else
	{
		this->Restart();																// OR WE RESTART THE FRAMETIME
	}
	this->End();
}

void Timer::Print(const mil_duration & time)
{
	printf("Time ended: %f\n", time.count());
}

void Timer::set_delta_time()
{
	delta_time = std::chrono::duration<float, std::milli>(d_end - d_start).count() / 1000.0f;
}
