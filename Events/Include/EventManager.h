#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

//----------------------------------------------
//				Event Manager
//				Colin Vaughan
//
//	
//
//----------------------------------------------

class IEvent
{
	
};


class EventQueue
{
	static void init()
	{
		m_head = 0;
		m_tail = 0;
	}
	void DispatchEvent(IEvent* a_event);
	void RegisterListner();

private:
	static int m_head;
	static int m_tail;

	IEvent m_Queue[100];
};

#endif