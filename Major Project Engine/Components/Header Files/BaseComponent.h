#pragma once
class BaseComponent
{
public:
	virtual ~BaseComponent() {}
	virtual void receive(int message) = 0;
};