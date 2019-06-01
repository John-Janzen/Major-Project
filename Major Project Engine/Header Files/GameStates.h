#pragma once

#ifndef _GAMESTATES_H
#define _GAMESTATES_H

enum GAME_STATE
{
	NULL_STATE,
	INITIALIZING,
	LOADING,
	PLAYING,
	PAUSED,
	DELOAD,
	DEBUG_LOAD,
	DEBUG_RUN,
	DEBUG_CLOSE,
	EXITING
};

#endif // !_GAMESTATES_H


