//#include "SolarSystemApp.h"
#include "OBJLoaderApp.h"

void main()
{
	//SolarSystemApp* app = new SolarSystemApp();
	OBJLoaderApp* app = new OBJLoaderApp();
	if (app->StartUp())
	{
		while (app->Update())
		{
			app->Draw();
		}
		app->ShutDown();
	}
	delete app;
	return;
}