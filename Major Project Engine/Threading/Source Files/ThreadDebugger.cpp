#include "ThreadDebugger.h"

const std::size_t ThreadDebugger::default_height = 600;
const std::size_t ThreadDebugger::default_width = 900;

ThreadDebugger::ThreadDebugger()
{
	

	m_borders[0].w = default_width;
	m_borders[0].h = 50;
	m_borders[0].x = 0;
	m_borders[0].y = 0;

	m_borders[1].w = 50;
	m_borders[1].h = default_height;
	m_borders[1].x = 0;
	m_borders[1].y = 0;
}

ThreadDebugger::~ThreadDebugger() 
{
	SDL_DestroyWindow(debug_window);
	SDL_DestroyRenderer(debug_renderer);

	debug_window = nullptr;
	debug_renderer = nullptr;
}

void ThreadDebugger::LoadDebug(const float & rate, const std::size_t & count)
{
	if (SDL_CreateWindowAndRenderer(default_width, default_height, SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_HIDDEN, &debug_window, &debug_renderer) == -1)
		printf("Error creating debug window");

	n_threads = count;
	refresh_rate = rate;
	widthOfLines = (int)(default_width / refresh_rate);
	heightOfLines = default_height / n_threads;
}

void ThreadDebugger::RenderDebug(std::array<Thread*, Thread::MAX_THREADS> threads, const Thread::ctp & object_time)
{
	SDL_SetRenderDrawColor(debug_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(debug_renderer);
	SDL_Rect rect;
	Color color;
	if (current_frame.empty())
	{
		int count = 0;
		for (auto & thread : threads)
		{
			if (thread == nullptr) continue;
			rect = SDL_Rect();
			for (auto & data : thread->AquireData())
			{
				ColorByID(data.t_id, color);
				CalculateRect(data, object_time, rect);

				rect.y = count * heightOfLines;
				rect.h = heightOfLines;
				current_frame.emplace_back(std::make_pair(rect, color));
			}
			count++;
		}
	}

	for (auto & pair : current_frame)
	{
		rect = pair.first;
		color = pair.second;
		SDL_SetRenderDrawColor(debug_renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(debug_renderer, &rect);

		if (rect.w > 2)
		{
			SDL_SetRenderDrawColor(debug_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
			SDL_RenderDrawRect(debug_renderer, &rect);
		}
	}

	SDL_SetRenderDrawColor(debug_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

	for (int i = 0; i < refresh_rate; i++)
	{
		SDL_RenderDrawLine(debug_renderer, i * widthOfLines, 0, i * widthOfLines, default_height);
	}

	for (int i = 0; i < n_threads + 1; i++)
	{
		SDL_RenderDrawLine(debug_renderer, 0, i * heightOfLines, default_width, i * heightOfLines);
	}

	SDL_RenderPresent(debug_renderer);
}

void ThreadDebugger::ColorByID(const Job::JOB_ID & id, Color & color)
{
	switch (id / JOB_STRIDE)
	{
	case Job::JOB_RENDER:
		color.r = 255;
		color.g = color.b = ((id % JOB_STRIDE) / (float)(Job::JOB_RENDER_COUNT - Job::JOB_RENDER_DEFAULT)) * 255;
		break;
	case Job::JOB_INPUT:
		color.r = color.b = ((id % JOB_STRIDE) / (float)(Job::JOB_INPUT_COUNT - Job::JOB_INPUT_DEFAULT)) * 255;
		color.g = 255;
		break;
	case Job::JOB_PHYSICS:
		color.r = color.g = ((id % JOB_STRIDE) / (float)(Job::JOB_PHYSICS_COUNT - Job::JOB_PHYSICS_DEFAULT)) * 255;
		color.b = 255;
		break;
	case Job::JOB_MISC:
		color.r = color.b = 255;
		color.g = ((id % JOB_STRIDE) / (Job::JOB_DEFAULT_COUNT - Job::JOB_DEFAULT)) * 255;
		break;
	default:
		break;
	}
	color.a = SDL_ALPHA_OPAQUE;
}

void ThreadDebugger::CalculateRect(const Thread::ThreadData & data, const Thread::ctp & object_time,  SDL_Rect & rect)
{
	Thread::ctp start = data.t_start;
	Thread::ctp end = data.t_end;

	auto t = std::chrono::duration_cast<std::chrono::microseconds>(start - object_time);
	auto t2 = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

	auto locX = t.count() / (refresh_rate * 1000);
	auto sizeX = t2.count() / (refresh_rate * 1000);

	auto num1 = locX * default_width;
	auto num2 = sizeX * default_width;

	rect.x = (int)std::ceilf(num1);
	rect.w = (int)std::ceilf(num2);

	//auto dakfnaegn = "asdasdawd";

}
