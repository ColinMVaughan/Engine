#include "Application.h"


class Demo : public Application
{
	void DoInitalize() override
	{

	}

	void DoUpdate() override
	{

	}
};



int main()
{
	Demo demo;
	demo.Initalize();
	demo.Update();
	
	return 0;
}