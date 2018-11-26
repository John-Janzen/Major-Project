#pragma once

#ifndef _CONTENT_H
#define _CONTENT_H

#include <vector>

struct Content
{
	virtual ~Content() {};
};

struct InitialContent : public Content
{
	std::vector<int> copy_section;
	int * result;

	InitialContent(const std::vector<int> & vec, int * & end_loc) : copy_section(vec)
	{
		result = end_loc;
	};
	~InitialContent() 
	{ 
		result = nullptr; 
	};
};

#endif // !_CONTENT_H
