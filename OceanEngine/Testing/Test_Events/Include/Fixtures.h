#pragma once

#include<gtest\gtest.h>
#include<EventManager.h>


//----------------------------------
//			TEST EVENTS
//----------------------------------

class TestEvent : public IEvent
{
public:
	TestEvent() {}
	TestEvent(std::string a_message) :Message(a_message)
	{}

	std::string Message;
};

class TestEvent2 : public IEvent
{
public:

};

//-------------------------------------
//		TEST EVENT HANDLERS
//-------------------------------------

class TestEventHandler
{
public:
	void HandleEvent(const TestEvent& a_event)
	{
		EventRecieved = true;
		return;
	}

	bool EventRecieved = false;
};

class SelfRegisteredListner
{
public:
	SelfRegisteredListner(EventManager& manager)
	{
		REGISTER_EVENT_LISTNER(TestEvent, SelfRegisteredListner::Listen, manager)
		//SelfRegisteredListner* ptr = this;
		//manager.AddListner<TestEvent>(std::bind(&SelfRegisteredListner::Listen, this, std::placeholders::_1));
		//std::bind(&SelfRegisteredListner::Listen, this);
	}

	void Listen(const TestEvent& eve)
	{
		if(eve.Message == "Hello World")
			RecievedEvent = true;
	}


public:
	bool RecievedEvent = false;
};
//--------------------------------------------
//				FIXTURES
//-------------------------------------------

class EventManagerTest : public ::testing::Test
{
public:
	EventManagerTest() {}
	~EventManagerTest() {}

public:
	EventManager m_Manager;
	
};