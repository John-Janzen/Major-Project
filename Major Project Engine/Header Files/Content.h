#pragma once

#ifndef _CONTENT_H
#define _CONTENT_H

#include <vector>

struct Content
{
	virtual ~Content() {};
};

class InitialContent : public Content
{
public:
	std::vector<int> copy_section;

	InitialContent(const std::vector<int> & vec) : copy_section(vec) {}
	~InitialContent() {};
private:
};

#endif // !_CONTENT_H
