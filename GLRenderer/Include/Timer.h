#pragma once
#include <iostream>
#include<SDL\SDL_timer.h>
/*
 * timer class
 * - this lets us keep track of the elapsed time
 *   in our program easily so we can use it in things
 *   like physics calculations 
 */
class Timer
{
public:
	Timer()
	{
		//previousTime = glutGet(GLUT_ELAPSED_TIME);
		this->tick();
	}

	~Timer();

	// update the timer clock
	double tick()
	{
		currentTime = (double)SDL_GetTicks();
		elapsedTime = currentTime - previousTime;
		previousTime = currentTime;
		return elapsedTime;
	}

	// delta time in milliseconds 
	double getElapsedTimeMS()
	{
		return elapsedTime;
	}

	// delta time in seconds
	double getElapsedTimeSeconds()
	{
		return elapsedTime / 1000.f;
	}

	double getCurrentTime()
	{
		return currentTime;
	}

private:
	double currentTime, previousTime, elapsedTime;
};
