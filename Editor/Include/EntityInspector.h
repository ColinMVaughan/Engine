//#ifndef ENTITY_INSPECTOR_H
//#define ENTITY_INSPECTOR_H
//#include <ECS.h>
//
////template <typename T>
////class ComponentReflection
////{
////public:
////	virtual void ReflectData();
////	virtual void AddToEntity()
////	{
////
////	}
////};
////
////class TestComp : public ComponentReflection<TestComp>
////{
////	
////};
//
//
//
////---------------------------------------------------------------------------
//
//
//
//class EntityInspector
//{
//	typedef void(ECS::Scene::* AddComponentFunc)(ECS::Entity);
//public:
//	EntityInspector(ECS::Scene* a_Scene)
//		:m_Scene(a_Scene) {}
//
//	//void InspectEntity(ECS::Entity entity);
//
//	template<class Component>
//	void RegisterComponent()
//	{
//		test = &ECS::Scene::AddComponent<Component>;
//	}
//
//	void CallIt(ECS::Entity entity)
//	{
//		//test(entity);
//	}
//
//
//private:
//	ECS::Scene* m_Scene;
//	AddComponentFunc test;
//};
//
//
//#endif