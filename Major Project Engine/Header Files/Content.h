#pragma once

#ifndef _CONTENT_H
#define _CONTENT_H

struct Content
{
	virtual ~Content() {};


};

class InitialContent : public Content
{
public:
	int num1, num2;

	InitialContent(const int & x, const int & y) : num1(x), num2(y) {}
	~InitialContent() {};
private:
	InitialContent();
};

#endif // !_CONTENT_H
