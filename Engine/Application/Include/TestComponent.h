#pragma once
#include <ComponentReflection.h>
#include <iostream>
class TestComp
{
public:
	int Number = 0;
	void Display()
	{
		std::cout << "\nWHAT!";
	}
};

//Registers the component with a string so it can be added int
COMPONENT_REGISTER(TestComp, "TestComp")
