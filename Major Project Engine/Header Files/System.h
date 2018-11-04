#pragma once

class Game;

class System
{
public:
	explicit System(Game & app);
	virtual ~System() = 0;

	virtual bool Load() = 0;
	virtual void Close() = 0;

private:
	Game & parent_app;
};

inline System::System(Game & app) : parent_app(app) {}

inline System::~System(){}
