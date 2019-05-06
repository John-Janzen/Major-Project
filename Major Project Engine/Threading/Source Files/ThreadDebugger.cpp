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
	heightOfLines = default_height / (n_threads + 1);
}

void ThreadDebugger::RenderDebug(std::array<Thread*, Thread::MAX_THREADS> threads)
{
	SDL_SetRenderDrawColor(debug_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(debug_renderer);

	for (auto & thread : threads)
	{
		if (thread == nullptr) continue;
		
		

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

void ThreadDebugger::ColorByID(const Job::JOB_ID & id)
{
	int r, g, b;
	switch (id / JOB_STRIDE)
	{
	case Job::JOB_RENDER:
		r = 255;
		g = b = id % JOB_STRIDE;
		break;
	case Job::JOB_INPUT:
		r = b = id % JOB_STRIDE;
		g = 255;
		break;
	case Job::JOB_PHYSICS:
		r = g = id % JOB_STRIDE;
		b = 255;
		break;
	case Job::JOB_MISC:
		r = b = 255;
		g = id % JOB_STRIDE;
		break;
	default:
		break;
	}
	SDL_SetRenderDrawColor(debug_renderer, r, g, b, SDL_ALPHA_OPAQUE);
}
