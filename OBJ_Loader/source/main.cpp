#include "OBJLoaderApp.h"

void main()
{
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