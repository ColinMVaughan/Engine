#pragma once
#include "ECS.h"
#include "Detail.h"

namespace ECS
{
	namespace DynamicDetail
	{



		typedef bool(*DynamicComponentFunc)(Scene*, Entity, ECS::detail::ComponentAction);
		typedef bool(*DynamicSystemFunc)(Scene*, ECS::detail::SystemAction);

		typedef std::map<std::string, DynamicComponentFunc> DynamicComponentRegistry;
		typedef std::map<std::string, DynamicSystemFunc> DynamicSystemRegistry;


		inline DynamicComponentRegistry& GetDynamicComponentRegistry()
		{
			static DynamicComponentRegistry reg;
			return reg;
		}

		inline DynamicSystemRegistry& GetDynamicSystemRegistry()
		{
			static DynamicSystemRegistry reg;
			return reg;
		}


	}

}