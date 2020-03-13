#pragma once

#ifndef _THREADDEBUGGER_H
#define _THREADDEBUGGER_H

#include <SDL.h>
#include <map>
#include <list>
#include <SDL_ttf.h>

#include "Thread.h"

/*
The Thread Debugger displays all data pertaining to the threads and thread manager.
It uses a seperate SDL window to do this.
*/
class ThreadDebugger
{
public:
	ThreadDebugger();
	~ThreadDebugger();

	/*
	Load the debugger with the refresh rate and the thread count
	*/
	void LoadDebug(const float & rate, const std::size_t & count);

	/*
	Render the debugger
	*/
	void RenderDebug();

	/*
	Show the window
	*/
	void ShowDebug()
	{
		SDL_ShowWindow(debug_window);
	}

	/*
	Hide the window and clear the logs
	*/
	void HideDebug()
	{
		SDL_HideWindow(debug_window);
		current_frame.clear();
		loaded = false;
	}

	/*
	Get the window ID
	*/
	Uint32 GetWindowID() 
	{ 
		return SDL_GetWindowID(debug_window); 
	}

	/*
	Check to see if mouse is within a DataPoint
	*/
	void CheckMouseLocation(const SDL_MouseMotionEvent & event);

	/*
	Load debugger with all data for all threads
	*/
	void LoadDebugData(std::array<Thread*, Thread::MAX_THREADS> threads, const hr_tp & object_time);

	bool loaded = false;

private:
	
	/*
	Storage of the threads data saved close by so it is easy to look up
	*/
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

	/*
	From the data I can calculate the rectangle size that gives an accurate representation 
	on a graph.
	*/
	void CalculateRect(const Thread::ThreadData & data, const hr_tp & object_time, SDL_Rect & rect);

	/*
	From the data I can generate a color based on the Job id that is passed through
	*/
	void ColorByID(const job::JOB_ID & id, DataPoints::Color & color);

	/*
	This loads the label onto the sdl surface to draw on the screen
	*/
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
