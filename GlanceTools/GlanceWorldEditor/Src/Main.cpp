/*=================================================================================================
  Glance World Editor - (C) 2013 Daniel L. Watkins

  Filename: Main.cpp
  Created: 4/20/2013 @ 16:42 UTC-6
=================================================================================================*/
#include "Main.h"

using namespace ge;

Window app;
Root root;
SharedPtr<Console> console;

/*=============================================================================
--
=============================================================================*/
int main()
{
	VideoSettings *videoSettings = app.GetVideoSettings();
	videoSettings->title = String()+"Glance Resource Editor "+gwe::gGweVersion+" - (GE: "+GetVersionText()+")";
	videoSettings->verticalSync = true;
	videoSettings->allowResize = true;

	app.Init("Glance World Editor");
	app.SetBackgroundColor(gWhite);

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

	using namespace gui;

	WeakPtr<MenuBar> menuBar = root.CreateMenuBar(gwe::EID_MENU_BAR_MAIN, -1, Vector2D<int>(), app.GetWidth());
		WeakPtr<ContextMenu> cm1 = menuBar.lock()->AddMenu("File");
			cm1.lock()->AddItem("New");
			cm1.lock()->AddItem("Open");
			cm1.lock()->AddItem("Exit");
		WeakPtr<ContextMenu> cm2 = menuBar.lock()->AddMenu("Help");
			cm2.lock()->AddItem("About");


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