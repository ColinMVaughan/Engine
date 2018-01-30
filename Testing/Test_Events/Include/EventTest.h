#ifndef _EVENTS_TESTING_H_
#define _EVENTS_TESTING_H_

#include <gtest\gtest.h>
#include <EventManager.h>
#include <string>

class TestEvent : public IEvent
{
public:
	std::string Message;
};

class TestEvent2 : public IEvent
{
public:

};

class TestEventHandler
{
public:
	void HandleEvent(const TestEvent& a_event)
	{
		EventRecieved = true;
	}

	bool EventRecieved = false;
};

//--------------------------------------------------------------
//							TESTS
//--------------------------------------------------------------

//Test that a listner can be added to the event manager
TEST(EventManagerTest, AddListnerTest)
{
	TestEventHandler handler;

	EventManager manager;
	manager.AddListner<TestEvent>((std::bind(&TestEventHandler::HandleEvent, handler, std::placeholders::_1)));

	TestEvent eve;
	manager.DispatchEvent<TestEvent>(eve);

	EXPECT_TRUE(handler.EventRecieved);
}

//Test that events only trigger listeners registered to it
TEST(EventManagerTest, EventDispatch)
{
	TestEventHandler handler;
	EventManager manager;
	manager.AddListner<TestEvent>(std::bind(&TestEventHandler::HandleEvent, handler, std::placeholders::_1));
	
	manager.DispatchEvent<TestEvent2>(TestEvent2());
	EXPECT_FALSE(handler.EventRecieved);

	manager.DispatchEvent<TestEvent>(TestEvent());
	EXPECT_TRUE(handler.EventRecieved);

}

//Test that Events are cleaned up after dispatching to lisners.
TEST(EventManagerTest, EventMemoryManagement)
{
	EXPECT_TRUE(true);
}

#endif