#pragma once

#ifndef _INPUT_H
#define _INPUT_H

#include "System.h"
#include "ComponentHeaders.h"

#include <SDL.h>

#define DEG_TO_RAD 0.01745329;
#define RAD_TO_DEG 57.29578;
#define MIN_UPWARDS_TILT_DEG 1.0;

/*
The Input System deals with player input with mouse and keyboard.
I was planning on controller but I ran out of time

Inherits System
*/
class Input :
	public System
{
public:

	
	Input(TaskManager & tm, SceneManager & sm);
	~Input();

	/*
	Load the input system.
	Mostly load the sdl controller.
	*/
	bool Load();

	/*
	Close the input system
	*/
	void Close(void* content);

	/*
	Handle any events that come in
	*/
	void HandleEvent(const EventType & e, void * data);

	/*
	Update the input system for all players
	*/
	JOB_RETURN Update(void * ptr);

	/*
	change the input to something else
	Unused
	*/
	void ChangeInput();
private:
	/*
	Set player transform with the current state of the keyboard and mouse.
	Adjust poisiton and rotation
	Then send it off to the physics to update
	*/
	void PlayerControls(PlayerControllerComponent * pc_cp, Transform * transform);

	/*
	When game is paused still accept input from player
	Only button is the Escape button available
	*/
	JOB_RETURN PausedControls(void * ptr);

	const float player_speed = 10.f;			// players walking speed
	const float camera_rotation_deg = 10.f;		// players mouse rotation speed

	const float running = 2.f;					// players running speed

	bool buttonHeld = false, Tbutton = false, escapeButton = false, Pbutton = false;	// Some flags so we don't have to worry about holding keys down
};

#endif // !_INPUT_H
