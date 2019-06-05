#pragma once

#ifndef _INPUT_H
#define _INPUT_H

#include "System.h"
#include "ComponentHeaders.h"

#include <SDL.h>

#define DEG_TO_RAD 0.01745329;
#define RAD_TO_DEG 57.29578;
#define MIN_UPWARDS_TILT_DEG 1.0;

class Input :
	public System
{
public:

	
	Input(TaskManager & tm, SceneManager & sm);
	~Input();

	bool Load();

	void Close(void* content);

	void HandleEvent(const EventType & e, void * data);

	JOB_RETURN Update(void * ptr);

	void ChangeInput();
private:

	void PlayerControls(PlayerControllerComponent * pc_cp, Transform * transform);

	const float player_speed = 10.f;
	const float camera_rotation_deg = 10.f;

	const float running = 2.f;

	bool buttonHeld = false, Tbutton = false;
};

#endif // !_INPUT_H
