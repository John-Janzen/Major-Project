#pragma once

#ifndef _THREADDEBUGGER_H
#define _THREADDEBUGGER_H

#include <SDL.h>

#include "Thread.h"

class ThreadDebugger
{
public:
	ThreadDebugger();
	~ThreadDebugger();

	void LoadDebug(const float & rate, const std::size_t & count);

	void RenderDebug(std::array<Thread*, Thread::MAX_THREADS> threads, const Thread::ctp & object_time);

	void ColorByID(const Job::JOB_ID & id);

	void ShowDebug()
	{
		SDL_ShowWindow(debug_window);
	}

	void HideDebug()
	{
		SDL_HideWindow(debug_window);
	}

private:

	void CalculateRect(const Thread::ThreadData & data, const Thread::ctp & object_time, SDL_Rect & rect);

	SDL_Window * debug_window;
	SDL_Renderer * debug_renderer;

	const static std::size_t default_width;
	const static std::size_t default_height;

	std::size_t n_threads;
	float refresh_rate;

	int widthOfLines, heightOfLines;

	SDL_Rect m_borders[2];
};

#endif // !_THREADDEBUGGER_H
