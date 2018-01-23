#pragma once
//#include <SystemManager.h>
//#include <ComponentManager.h>
//#include <ECS.h>
//#include <thread>
//
//class Map
//{
//public:
//	char data[10][10];
//};
//
//
//
//class DrawMapSystem : public ECS::System<Map>
//{
//public:
//	DrawMapSystem(ECS::ComponentManager* a_Cmanager, EventManager* a_eManager) : System(a_Cmanager, a_eManager)
//	{
//		
//	}
//
//	virtual void Update(unsigned int entity)
//	{
//
//		Map* map = m_CManager->GetComponent<Map>(entity);
//
//		system("cls");
//		for(int y = 0; y < 10; y++)
//		{
//			std::cout << "\n";
//			for(int x = 0; x < 10; x++)
//			{
//				std::cout << map->data[y][x];
//			}
//		}
//		std::this_thread::sleep_for(std::chrono::milliseconds(50));
//	}
//};
//
//
//
//class Demo
//{
//
//public:
//
//	Demo()
//	{
//		
//	}
//	void Initalize()
//	{
//		//Initalize ECS Managers
//		EventManager eManager;
//		ECS::ComponentManager* m_CompMan = new ECS::ComponentManager;
//		ECS::SystemManager*    m_SysMan  = new ECS::SystemManager(m_CompMan, eManager);
//		m_ECS                       = new ECS::Scene(m_SysMan, m_CompMan);
//
//		//Create Entities
//		m_Map = m_ECS->CreateEntity();
//
//		//Add Systems
//		m_ECS->AddSystem<DrawMapSystem>();
//
//
//		//Add Components for Map
//		m_ECS->AddComponent<Map>(m_Map);
//		Map* map = m_ECS->GetComponent<Map>(m_Map);
//		for(int y = 0; y< 10; y++)
//		{
//			for (int x = 0; x< 10; x++)
//			{
//				map->data[y][x] = '@';
//			}
//		}
//
//
//	}
//	void Update()
//	{
//		for(;;)
//		{
//			m_ECS->UpdateSystems(0.0);
//		}
//	}
//
//	void Unload()
//	{
//		
//	}
//
//private:
//	ECS::Scene*   m_ECS;
//
//	ECS::Entity m_Map;
//	ECS::Entity m_Player;
//};