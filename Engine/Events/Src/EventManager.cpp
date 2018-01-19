#include "EventManager.h"

//void EventManager::AddListner(int a_event, FunctionType&& a_func)
//{
//	Listners[a_event].push_back(a_func);
//}

void EventManager::RemoveListner()
{

}

//void EventManager::DispatchEvent(int a_entity, IEvent& a_event)
//{
//	int type = a_event.ID;
//	if (Listners.find(type) == Listners.end())
//		return;
//
//	auto&& ListnerList = Listners.at(type);
//	for (auto&& listner : ListnerList)
//		listner(a_event);
//}