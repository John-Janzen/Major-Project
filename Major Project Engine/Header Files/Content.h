#pragma once

#ifndef _CONTENT_H
#define _CONTENT_H

struct Content
{
	virtual ~Content() {};
};

struct PhysicsComponentContent : public Content
{
	PhysicsComponent * p_cp;
	Transform * trans;
	PhysicsComponentContent(PhysicsComponent * p, Transform * t) : p_cp(p), trans(t) {}
	~PhysicsComponentContent() { p_cp = nullptr; trans = nullptr; }
};

#endif // !_CONTENT_H
