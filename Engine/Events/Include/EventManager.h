#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <functional>
#include <vector>
#include <map>


//Event Interface: All other events must inherit from IEvent.
class IEvent
{
public:
	IEvent();
	virtual ~IEvent();

	int ID = 0;
private:
};

	
//----------------------------------------------
//				Event Manager
//				Colin Vaughan
//
// 
//
//----------------------------------------------
class EventManager
{
public:
	using FunctionType = std::function<void(const IEvent&)>;

	template<typename EventType>
	void AddListner(EventType a_event, FunctionType&& a_func);

	void RemoveListner();

	template<typename EventType>
	void DispatchEvent(EventType& a_event);

private:
	std::map<size_t, std::vector<FunctionType>> Listners;
};



//----------------------------------------------
//			IMPLEMENTATION
//----------------------------------------------

template<typename EventType>
void EventManager::AddListner(EventType a_event, FunctionType&& a_func)
{
	auto ID = typeid(EventType).hash_code();
	Listners[ID].push_back(a_func);
}


template<typename EventType>
void EventManager::DispatchEvent(EventType& a_event)
{

	int type = a_event.ID;
	if (Listners.find(type) == Listners.end())
		return;

	auto&& ListnerList = Listners.at(type);
	for (auto&& listner : ListnerList)
		listner(a_event);
}

#endif