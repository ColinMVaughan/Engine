#pragma once
#include <ComponentReflection.h>
class TestComp
{
public:
	int Number = 0;
};

//Registers the component with a string so it can be added int
COMPONENT_REGISTER(TestComp, "TestComp")