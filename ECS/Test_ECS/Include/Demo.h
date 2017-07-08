#pragma once
#include <SystemManager.h>
#include <ComponentManager.h>
#include <ECS.h>
#include <thread>

class Map
{
public:
	char data[10][10];
};



class DrawMapSystem : public System<Map>
{
public:
	DrawMapSystem(ComponentManager* a_Cmanager) : System(a_Cmanager)
	{
		
	}

	virtual void Update(unsigned int entity)
	{

		Map* map = m_CManager->GetComponent<Map>(entity);

		system("cls");
		for(int y = 0; y < 10; y++)
		{
			std::cout << "\n";
			for(int x = 0; x < 10; x++)
			{
				std::cout << map->data[y][x];
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
};



class Demo
{

public:

	Demo() : m_Map(0), m_Player(0)
	{
		
	}
	void Initalize()
	{
		//Initalize ECS Managers
		ComponentManager* m_CompMan = new ComponentManager;
		SystemManager*    m_SysMan  = new SystemManager(m_CompMan);
		m_ECS                       = new ECS(m_SysMan, m_CompMan);


		//Create Entities
		m_Map = m_ECS->CreateEntity();

		//Add Systems
		m_ECS->AddSystem<DrawMapSystem>();


		//Add Components for Map
		m_ECS->AddComponent<Map>(m_Map);
		Map* map = m_ECS->GetComponent<Map>(m_Map);
		for(int y = 0; y< 10; y++)
		{
			for (int x = 0; x< 10; x++)
			{
				map->data[y][x] = '@';
			}
		}


	}
	void Update()
	{
		for(;;)
		{
			m_ECS->UpdateSystems();
		}
	}

	void Unload()
	{
		
	}

private:
	ECS*   m_ECS;

	Entity m_Map;
	Entity m_Player;
};