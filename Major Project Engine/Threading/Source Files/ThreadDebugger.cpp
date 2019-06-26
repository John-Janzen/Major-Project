#include "ThreadDebugger.h"

const int ThreadDebugger::default_width = 900;
const int ThreadDebugger::default_height = 600;

ThreadDebugger::ThreadDebugger()
{
	m_borders[0] = { 0, 0, default_width, border_width };
	m_borders[1] = { 0, 0, border_width, default_height };
	m_text[0] = { ((int)windowX / 2) - 50, 0, 250, 50 };
	m_text[1] = { border_width + 100, 50, 100, 50 };
	m_text[2] = { ((int)windowX / 2) + 50, 50, 100, 50 };
	m_text[3] = { (int)windowX - 100, 50, 100, 50 };

	if (TTF_Init() < 0)
	{
		printf("Error loading TTF library");
	}
	else
	{
		sans_font = TTF_OpenFont("Assets/Text/OpenSans.ttf", 24);
	}
}

ThreadDebugger::~ThreadDebugger() 
{
	TTF_CloseFont(sans_font);
	SDL_DestroyWindow(debug_window);
	SDL_DestroyRenderer(debug_renderer);

	debug_window = nullptr;
	debug_renderer = nullptr;
}

void ThreadDebugger::LoadDebug(const float & rate, const std::size_t & count)
{
	if (SDL_CreateWindowAndRenderer(default_width, default_height, SDL_WINDOW_HIDDEN, &debug_window, &debug_renderer) == -1)
		printf("Error creating debug window");

	SDL_SetWindowTitle(debug_window, "Thread Debugger");

	n_threads = count;
	refresh_rate = rate;
	widthOfLines = (int)(windowX / refresh_rate);
	heightOfLines = (int)(windowY / n_threads);
	current_frame.reserve(50);

	for (int i = 0; i < count; i++)
	{
		m_thread_labels[i] = { 12, (i * ((int)windowY / (int)count)) + ((((int)windowY / (int)count) / 2) - 12) + border_width, 75, 25 };
	}
}

void ThreadDebugger::RenderDebug()
{
	SDL_SetRenderDrawColor(debug_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(debug_renderer);

	SDL_Rect rect;
	DataPoints dp_job;

	for (int i = 0; i < n_threads; i++)
		this->LoadLabel(&m_thread_labels[i], std::string(thread_label + std::to_string(i)));

	this->LoadLabel(&m_text[0], job_label.c_str());
	this->LoadLabel(&m_text[1], start_label.c_str());
	this->LoadLabel(&m_text[2], total_label.c_str());
	this->LoadLabel(&m_text[3], end_label.c_str());

	SDL_SetRenderDrawColor(debug_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRects(debug_renderer, m_borders, 2);

	for (int i = 0; i < refresh_rate; i++)
	{
		SDL_RenderDrawLine(debug_renderer, i * widthOfLines + border_width, border_width, i * widthOfLines + border_width, default_height);
	}

	for (int i = 0; i < n_threads + 1; i++)
	{
		SDL_RenderDrawLine(debug_renderer, border_width, i * heightOfLines + border_width, default_width, i * heightOfLines + border_width);
	}

	for (auto & pair : current_frame)
	{
		rect = pair.first;
		dp_job = pair.second;
		SDL_SetRenderDrawColor(debug_renderer, dp_job.c.r, dp_job.c.g, dp_job.c.b, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(debug_renderer, &rect);

		if (rect.w > 2)
		{
			SDL_SetRenderDrawColor(debug_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
			SDL_RenderDrawRect(debug_renderer, &rect);
		}
	}

	SDL_RenderPresent(debug_renderer);
}

void ThreadDebugger::LoadLabel(SDL_Rect * rect, const std::string & label)
{
	auto text_surface = TTF_RenderText_Shaded(this->sans_font, label.c_str(), SDL_Color{ 0, 0, 0 }, SDL_Color{ 255, 255, 255 });
	auto text_texture = SDL_CreateTextureFromSurface(debug_renderer, text_surface);

	SDL_RenderCopy(debug_renderer, text_texture, NULL, rect);

	SDL_DestroyTexture(text_texture);
	SDL_FreeSurface(text_surface);

	text_surface = nullptr;
	text_texture = nullptr;
}

void ThreadDebugger::LoadDebugData(std::array<Thread*, Thread::MAX_THREADS> threads, const hr_tp & object_time)
{
	SDL_Rect rect;
	DataPoints dp_job;

	if (!loaded)
	{
		job_label = "No Job Selected";
		int count = 0;
		for (auto & thread : threads)
		{
			if (thread == nullptr) continue;
			rect = SDL_Rect();
			for (auto & data : thread->AquireData())
			{
				if ((data.t_end - data.t_start).count() < 0) data.t_end = hr::now();

				ColorByID(data.t_id, dp_job.c);
				CalculateRect(data, object_time, rect);

				rect.y = (count * heightOfLines) + border_width;
				dp_job.id = data.t_id;
				dp_job.name = data.t_name;
				dp_job.start = DataPoints::milli(data.t_start - object_time);
				dp_job.end = DataPoints::milli(data.t_end - object_time);
				dp_job.total = DataPoints::milli(data.t_end - data.t_start);
				current_frame.emplace_back(std::make_pair(rect, dp_job));
			}
			count++;
		}
		loaded = true;
	}
}

void ThreadDebugger::ColorByID(const job::JOB_ID & id, DataPoints::Color & color)
{
	switch (id / JOB_STRIDE)
	{
	case job::JOB_MAIN:
		color.r = color.g = 255;
		color.b = (Uint8)std::roundf(((id % JOB_STRIDE) / (float)(job::JOB_MAIN_COUNT - job::JOB_MAIN_DEFAULT)) * 255);
		break;
	case job::JOB_RENDER:
		color.r = 255;
		color.g = color.b = (Uint8)std::roundf(((id % JOB_STRIDE) / (float)(job::JOB_RENDER_COUNT - job::JOB_RENDER_DEFAULT)) * 255);
		break;
	case job::JOB_INPUT:
		color.r = color.b = (Uint8)std::roundf(((id % JOB_STRIDE) / (float)(job::JOB_INPUT_COUNT - job::JOB_INPUT_DEFAULT)) * 255);
		color.g = 255;
		break;
	case job::JOB_PHYSICS:
		color.r = color.g = (Uint8)std::roundf(((id % JOB_STRIDE) / (float)(job::JOB_PHYSICS_COUNT - job::JOB_PHYSICS_DEFAULT)) * 255);
		color.b = 255;
		break;
	case job::JOB_MISC:
		color.r = color.b = 255;
		color.g = (Uint8)std::roundf(((id % JOB_STRIDE) / (float)(job::JOB_DEFAULT_COUNT - job::JOB_DEFAULT)) * 255);
		break;
	default:
		break;
	}
	color.a = SDL_ALPHA_OPAQUE;
}

void ThreadDebugger::CheckMouseLocation(const SDL_MouseMotionEvent & event)
{
	if (current_frame.empty()) return;
	SDL_Rect rect;
	for (auto & data : current_frame)
	{
		rect = data.first;
		if ((event.x > rect.x && event.x < rect.x + rect.w)
			&& event.y > rect.y && event.y < rect.y + rect.h)
		{
			job_label = data.second.name;
			start_label = "Start: " + std::to_string(data.second.start.count()) + "ms";
			total_label = "Total: " + std::to_string(data.second.total.count()) + "ms";
			end_label = "End: " + std::to_string(data.second.end.count()) + "ms";
			return;
		}
	}
	job_label = "No Job Selected";
	start_label = "Start: 0.000ms";
	end_label = "End: 0.000ms";
	total_label = "Total: 0.000ms";
}

void ThreadDebugger::CalculateRect(const Thread::ThreadData & data, const hr_tp & object_time,  SDL_Rect & rect)
{
	rect.x = (int)std::ceilf((std::chrono::duration<float, std::milli>(data.t_start - object_time).count() / refresh_rate) * (windowX)) + border_width;
	rect.w = (int)std::ceilf((std::chrono::duration<float, std::milli>(data.t_end - data.t_start).count() / refresh_rate) * (windowX));
	if (rect.w <= 2)
		rect.w = 5;
	rect.h = heightOfLines;
}
