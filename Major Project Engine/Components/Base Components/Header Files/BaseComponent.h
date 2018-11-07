#pragma once
class BaseComponent
{
public:
	virtual ~BaseComponent() {}
	virtual void receive(int message) = 0;

	void set_death() { death_flag = true; }
private:
	bool death_flag = false;
};