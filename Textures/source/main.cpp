#define STB_IMAGE_IMPLEMENTATION
#include "TexturesApp.h"

void main()
{
	TexturesApp* app = new TexturesApp();
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