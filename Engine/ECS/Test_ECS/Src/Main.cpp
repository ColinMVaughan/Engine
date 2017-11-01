#include <gtest\gtest.h>
#include "ComponentManagerTest.h"
#include "SystemManagerTest.h"
#include "ECSTest.h"
#include "Demo.h"

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	//RUN_ALL_TESTS();

	Demo Game;
	Game.Initalize();
	Game.Update();
	Game.Unload();


	return 0;
}