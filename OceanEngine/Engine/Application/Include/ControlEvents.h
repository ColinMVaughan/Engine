#pragma once
#include <SDL/SDL_events.h>
#include <EventManager.h>

#include <glm/vec2.hpp>


class KeyPressedEvent : public IEvent
{
public:
	KeyPressedEvent() = default;
	KeyPressedEvent(SDL_Event input) {InputEvent = input.key; }

	SDL_KeyboardEvent InputEvent;
};

class KeyReleasedEvent : public IEvent
{
public:
	KeyReleasedEvent() = default;
	KeyReleasedEvent(SDL_Event input) {InputEvent = input.key;}

	SDL_KeyboardEvent InputEvent;
};

class MouseClickedEvent : public IEvent
{
public:
	MouseClickedEvent() = default;
	MouseClickedEvent(SDL_Event input) { InputEvent = input.button; }

	SDL_MouseButtonEvent InputEvent;
};

class MouseReleasedEvent : public IEvent
{
public:

	MouseReleasedEvent() = default;
	MouseReleasedEvent(SDL_Event input) { InputEvent = input.button; }

	SDL_MouseButtonEvent InputEvent;
};

class MouseMovedEvent : public IEvent
{
public:
	MouseMovedEvent() = default;
	MouseMovedEvent(SDL_Event input) { InputEvent = input.motion; }

	inline glm::vec2 GetVelocity()
	{
		return glm::vec2();
	}
	SDL_MouseMotionEvent InputEvent;
};

class ScrollWheelEvent : public IEvent
{
public:
	ScrollWheelEvent() = default;
	ScrollWheelEvent(SDL_Event input) { InputEvent = input.wheel; }

	inline float GetScrollAmmount()
	{
		return 0.0f;
	}
	SDL_MouseWheelEvent InputEvent;
};


//template<typename InputType>
//class SDLInputEvent : public IEvent
//{
//public:
//	SDLInputEvent() = default;
//	SDLInputEvent(InputType input)
//	{
//		m_Input = input;
//	}
//
//public:
//	InputType m_Input;
//};