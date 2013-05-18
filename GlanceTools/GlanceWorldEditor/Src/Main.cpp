/*=================================================================================================
  Glance World Editor - (C) 2013 Daniel L. Watkins

  Filename: Main.cpp
  Created: 4/20/2013 @ 16:42 UTC-6
=================================================================================================*/
#include "Main.h"

using namespace ge;

Window app;
Root root;
gwe::WorldEditor worldEditor(&root);
SharedPtr<Console> console;

/*=============================================================================
--
=============================================================================*/
int main()
{
	VideoSettings *videoSettings = app.GetVideoSettings();
	videoSettings->title = String()+"Glance World Editor "+gwe::gGweVersion+" - (GE: "+gSDKVersion+")";
	videoSettings->verticalSync = true;
	videoSettings->allowResize = true;

	app.Init("Glance World Editor");
	app.SetBackgroundColor(gGray);

	console = SharedPtr<Console>(new Console);
	console->Init(app.GetWidth(), app.GetHeight()/2.5, &app);
	gDebug.AddDebugProtocol(console);

	ClientInit();

	while (!app.GetQuit())
	{
		app.Update();
		console->Update();

		if (app.GetInput()->GetKeyState(GK_Grave, KEY_RELEASED))
			console->ToggleVisible();
		
		if (app.GetWindow()->GetEvent(*app.GetInput()->GetEvent()))
		{
			if (app.GetInput()->GetEvent()->Type == sf::Event::Closed)
				app.SetQuit(true);
			else if (app.GetInput()->GetKeyState(GK_Escape, KEY_DOWN))
				app.SetQuit(true);
		}
	}

	app.Shutdown();

    return EXIT_SUCCESS;
}


/*=============================================================================
--
=============================================================================*/
bool ge::ClientInit()
{
	root.Init(&app);
	worldEditor.Init();

	return true;
}



/*=============================================================================
--
=============================================================================*/
void ge::ClientLoop()
{
	root.Update();
	worldEditor.Update();
	root.Draw();
	console->Draw();
}