#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <functional>
#include <vector>
#include <map>
#include <type_traits>

//Event Interface: All other events must inherit from IEvent.
class IEvent
{
public:
	IEvent() {}
	virtual ~IEvent() {}

	int ID = 0;
private:
};


//BaseEventHandler used to provide pure virtual interface for dispatching events
class EventDispatcherInterface
{
public:
	virtual void DispatchEvent(IEvent& a_event) = 0;

};

template<typename EventType>
class EventDispatcher : public EventDispatcherInterface
{
public:
	using FunctionType = std::function<void(EventType&)>;

	void DispatchEvent(IEvent& a_event) override
	{
		EventType* convertedEvent = static_cast<EventType*>(&a_event);
		for (auto&& Listner : Functions)
		{
			Listner(*convertedEvent);
		}
	}

	void AddListner(std::function<void(EventType)>&& a_function)
	{
		Functions.push_back(a_function);
	}

private:
	std::vector<FunctionType> Functions;
};
	
//------------------------------------------------------------------------------
//								Event Manager
//								Colin Vaughan
//
// The Event Manager allows communication between otherwise decoupled systems.
// All Systems use the
//
// Systems are responsible for registering an event handling function against the events they want
// to recieve, durning construction.
//
// Once an event has been registered, the system's registered function will be
// called and passed the required event object.
//
// Systems are NOT responsible for managing memory of triggered events
//-----------------------------------------------------------------------------
class EventManager
{
public:
	using FunctionType = std::function<void(const IEvent&)>;

	template<typename EventType>
	void AddListner(std::function<void(const EventType&)>&& a_func);

	void RemoveListner();

	template<typename EventType>
	void DispatchEvent(EventType& a_event);

private:
	std::map<size_t, EventDispatcherInterface*> Dispatchers;
};


//----------------------------------------------
//			IMPLEMENTATION
//----------------------------------------------

//Registers a System's function against an Event. When that event is dispatched,
//All resgistered functions will be called.
template<typename EventType>
void EventManager::AddListner(std::function<void(const EventType&)>&& a_func)
{
	//Ensure that EventType is derrived from IEvent
	if constexpr(std::is_base_of<IEvent, EventType>::value)
	{
		//Get the registered event dispatcher, if none exists create one
		auto ID = typeid(EventType).hash_code();
		EventDispatcherInterface* ED = Dispatchers[ID];
		if (!ED)
		{
			ED = new EventDispatcher<EventType>;
			Dispatchers[ID] = ED;
		}

		//Get the correct event dispatcher and add the listner function to it.
		static_cast<EventDispatcher<EventType>*>(ED)->AddListner(a_func);
	}

}

//Dispatches an Event of type EventType, All corrisponding event handling
//functions are called.
template<typename EventType>
void EventManager::DispatchEvent(EventType& a_event)
{
	//Get the ID of the current event. If dispatcher not found, return.
	int type = typeid(EventType).hash_code();
	if (Dispatchers.find(type) == Dispatchers.end())
		return;

	//Iterate through the vector of Registered dispatcher corrisponding to the Event
	auto&& ListnerList = Dispatchers.at(type);
	ListnerList->DispatchEvent(a_event);
}



#endif