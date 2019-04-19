#pragma once

#ifndef _CONTENT_H
#define _CONTENT_H

class Content
{
public:
	virtual ~Content() {};
};

struct PhysicsComponentContent : public Content
{
	PhysicsComponent * p_cp;
	Transform * trans;
	PhysicsComponentContent(PhysicsComponent * p, Transform * t) : p_cp(p), trans(t) {}
	~PhysicsComponentContent() { p_cp = nullptr; trans = nullptr; }
};

struct RenderComponentContent : public Content
{
	RenderComponent * r_cp;
	Transform * trans;
	RenderComponentContent(RenderComponent * r, Transform * t) : r_cp(r), trans(t) {}
	~RenderComponentContent() { r_cp = nullptr; trans = nullptr; }
};

#endif // !_CONTENT_H
