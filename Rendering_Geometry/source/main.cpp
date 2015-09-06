//#include "SolarSystemApp.h"
#include "RenderingGeometryApp.h"

void main()
{
	//SolarSystemApp* app = new SolarSystemApp();
	RenderingGeometryApp* app = new RenderingGeometryApp();
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