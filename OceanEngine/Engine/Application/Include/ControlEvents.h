#pragma once
#include <SDL/SDL_events.h>
#include <EventManager.h>

#include <glm/vec2.hpp>

class KeyPressedEvent : public IEvent
{
public:

	inline void GetButtonPressed()
	{
		InputEvent.keysym;
	}

	SDL_KeyboardEvent InputEvent;
};

class KeyReleasedEvent : public IEvent
{
public:
	inline void GetButtonReleased()
	{
		InputEvent.keysym;
	}
	SDL_KeyboardEvent InputEvent;
};

class MouseClickedEvent : public IEvent
{
public:
	inline void GetButtonReleased()
	{

	}
	SDL_MouseButtonEvent InputEvent;
};

class MouseReleasedEvent : public IEvent
{
public:
	inline void GetButtonReleased()
	{

	}
	SDL_MouseButtonEvent InputEvent;
};

class MouseMovedEvent : public IEvent
{
public:
	inline glm::vec2 GetVelocity()
	{
		return glm::vec2();
	}
	SDL_MouseMotionEvent InputEvent;
};

class ScrollWheelEvent : public IEvent
{
public:
	inline float GetScrollAmmount()
	{
		return 0.0f;
	}
	SDL_MouseWheelEvent InputEvent;
};