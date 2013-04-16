/*=================================================================================================
  Glance Engine Testbed - (C) 2010-2013 Daniel L. Watkins

  Filename: Main.cpp
=================================================================================================*/
#include "Main.h"

using namespace ge;

Window app;
Root root;
gre::ResourceEditor resourceEditor(&root);
SharedPtr<Console> console;

/*=============================================================================
--
=============================================================================*/
int main()
{
	VideoSettings *videoSettings = app.GetVideoSettings();
	videoSettings->title = String()+"Glance Resource Editor "+gre::GetVersionText()+" - (GE: "+GetVersionText()+")";
	videoSettings->verticalSync = true;
	videoSettings->allowResize = true;

	app.Init("ResourceEditor");
	app.SetBackgroundColor(gWhite);

	console = SharedPtr<Console>(new Console);
	console->Init(app.GetWidth(), app.GetHeight()/2.5, &app);
	gDebug.AddDebugProtocol(console);

	ClientInit();

	while (!app.GetQuit())
	{
		app.Update();
		resourceEditor.Update();
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
	resourceEditor.CreateUserInterface();

	return true;
}



/*=============================================================================
--
=============================================================================*/
void ge::ClientLoop()
{
	root.Update();
	root.Draw();
	console->Draw();
}