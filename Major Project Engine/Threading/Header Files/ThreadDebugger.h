#pragma once

#ifndef _THREADDEBUGGER_H
#define _THREADDEBUGGER_H

#include <SDL.h>
#include <map>
#include <list>
#include <SDL_ttf.h>

#include "Thread.h"

class ThreadDebugger
{
public:
	ThreadDebugger();
	~ThreadDebugger();

	void LoadDebug(const float & rate, const std::size_t & count);

	void RenderDebug();

	void ShowDebug()
	{
		SDL_ShowWindow(debug_window);
	}

	void HideDebug()
	{
		SDL_HideWindow(debug_window);
		current_frame.clear();
		loaded = false;
	}

	Uint32 GetWindowID() 
	{ 
		return SDL_GetWindowID(debug_window); 
	}

	void CheckMouseLocation(const SDL_MouseMotionEvent & event);

	void LoadDebugData(std::array<Thread*, Thread::MAX_THREADS> threads, const hr_tp & object_time);

	bool loaded = false;

private:
	
	struct DataPoints
	{
		using milli = std::chrono::duration<float, std::milli>;

		struct Color
		{
			Uint8 r, g, b, a;
		} c;
		std::uint16_t id;
		std::string name;
		std::chrono::duration<float, std::milli> start, end, total;
	};

	void CalculateRect(const Thread::ThreadData & data, const hr_tp & object_time, SDL_Rect & rect);

	void ColorByID(const job::JOB_ID & id, DataPoints::Color & color);

	void LoadLabel(SDL_Rect * rect, const std::string & label);

	SDL_Window * debug_window = nullptr;
	SDL_Renderer * debug_renderer = nullptr;

	const static int default_width;
	const static int default_height;

	const int border_width = 100;

	std::size_t windowX = default_width - border_width;
	std::size_t windowY = default_height - border_width;

	TTF_Font * sans_font;

	std::size_t n_threads;
	float refresh_rate;

	int widthOfLines, heightOfLines;

	std::string job_label = "No Job Selected";
	std::string start_label = "Start: 0.000ms";
	std::string end_label = "End: 0.000ms";
	std::string total_label = "Total: 0.000ms";
	const std::string thread_label = "Thread ";

	SDL_Rect m_borders[2];
	SDL_Rect m_thread_labels[10];
	SDL_Rect m_text[4];

	std::vector<std::pair<SDL_Rect, DataPoints>> current_frame;
};

#endif // !_THREADDEBUGGER_H
