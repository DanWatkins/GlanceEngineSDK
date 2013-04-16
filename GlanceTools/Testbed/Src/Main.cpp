/*=================================================================================================
  Glance Engine Testbed - (C) 2010-2013 Daniel L. Watkins

  Filename: Main.cpp
=================================================================================================*/
#include "Main.h"


Window app;
world::World level;
world::Camera camera;
gui::Root root;

long long tick = 0;
double speedMult = 0.250;


/*=============================================================================
--
=============================================================================*/
int main()
{
	VideoSettings *videoSettings = app.GetVideoSettings();
	videoSettings->verticalSync = true;
	videoSettings->allowResize = true;
	
	app.Init("Testbed");
	app.SetBackgroundColor(gBlue);
	ClientInit();

	while (!app.GetQuit())
	{
		app.Update();

		if (app.GetWindow()->GetEvent(*app.GetInput()->GetEvent()))
		{
			if (app.GetInput()->GetEvent()->Type == sf::Event::Closed)
				app.SetQuit(true);
			else if (app.GetInput()->GetKeyState(GK_Escape, KEY_DOWN))
				app.SetQuit(true);
		}

		tick++;
	}

	app.Shutdown();

    return EXIT_SUCCESS;
}


/*=============================================================================
--
=============================================================================*/
void LoadWorld(String world)
{
	if (io::IsFile(world))
	{
		level.LoadWorldFromFile(world);
		camera.SetTarget(level.GetPlayer()->GetTarget());
	}
}


/*=============================================================================
--
=============================================================================*/
void LoadConfigurationFile()
{
	HMLFile file;
	file.Open("config.ini");
	if (file.IsOpen())
	{
		if (file.SeekElementOpen("<metrics>"))
		{
			while (!file.AtElementClose("</metrics>"))
			{
				if (file.TokenIsDeclaration("#screenWidth"))
					app.GetVideoSettings()->width = ToInt(file.SeekGetToken());
				else if (file.TokenIsDeclaration("#screenHeight"))
					app.GetVideoSettings()->height = ToInt(file.SeekGetToken());
				else if (file.TokenIsDeclaration("#fullscreen"))
					app.GetVideoSettings()->fullscreen = ToBool(file.SeekGetToken());
				else if (file.TokenIsDeclaration("#verticalSync"))
					app.GetVideoSettings()->verticalSync = ToBool(file.SeekGetToken());

				file.Seek();
			}
		}
	}
}


/*=============================================================================
--
=============================================================================*/
void BuildInterface()
{
	using namespace gui;

	root.CreateDialogBox(1, -1, Vector2D<int>(200,100), 400, 250, "Create New World");
	WeakPtr<MenuBar> menuBar = root.CreateMenuBar(2, -1, Vector2D<int>(0,0), app.GetVideoSettings()->width);
		WeakPtr<ContextMenu> fileMenu = menuBar.lock()->AddMenu("File");
			WeakPtr<ContextMenu> newMenu = fileMenu.lock()->AddItem("New");
				newMenu.lock()->AddItem("World");
				newMenu.lock()->AddItem("Resource Pack");
			fileMenu.lock()->AddItem("Open");
			fileMenu.lock()->AddItem("Save");
			fileMenu.lock()->AddItem("Save As...");
			fileMenu.lock()->AddItem("Save All");
			fileMenu.lock()->AddItem("Export Resource Pack");
			fileMenu.lock()->AddItem("Exit");

		WeakPtr<ContextMenu> editMenu = menuBar.lock()->AddMenu("Edit");
			editMenu.lock()->AddItem("Undo");
			editMenu.lock()->AddItem("Redo");
			editMenu.lock()->AddItem("Cut");
			editMenu.lock()->AddItem("Copy");
			editMenu.lock()->AddItem("Paste");
			editMenu.lock()->AddItem("Select All");
			editMenu.lock()->AddItem("Select All Visible");
			editMenu.lock()->AddItem("Select All Advanced");
			editMenu.lock()->AddItem("Deselect All");
			editMenu.lock()->AddItem("Delete");

	WeakPtr<ContextMenu> menu1 = root.CreateContextMenu(22, -1, Vector2D<int>(0,0), 40, 100, "LOL", Rect<int>(0, 0, 400, 400));
	menu1.lock()->AddItem("Howdy");
	menu1.lock()->AddItem("Die");
	menu1.lock()->AddItem("Leave");

	WeakPtr<ContextMenu> menu2 = root.CreateContextMenu(23, -1, Vector2D<int>(0,0), 40, 100, "HAHA", Rect<int>(0, 0, 400, 400));
	menu2.lock()->AddItem("Howdy2");
	menu2.lock()->AddItem("Die2");
	menu2.lock()->AddItem("Leave2");

	

	

	WeakPtr<TabBar> tabBar = root.CreateTabBar(26, 1, Vector2D<int>(10, 40), 380, 200);
		WeakPtr<TabPage> page1 = tabBar.lock()->AddPage("Page 1");
			root.CreateSlider(26101, page1, Vector2D<int>(10,100), 200);
		WeakPtr<TabPage> page2 = tabBar.lock()->AddPage("Page 2");
			root.CreateButtonImage(26201, page2, Vector2D<int>(10,100), 16, 16, "x_button.bmp");
		WeakPtr<TabPage> page3 = tabBar.lock()->AddPage("Page 3");
			root.CreateCheckBox(26301, page3, Vector2D<int>(10,50), "Fullscreen");
}


/*=============================================================================
--
=============================================================================*/
bool ge::ClientInit()
{
	LoadConfigurationFile();

	root.Init(&app);
	//BuildInterface();
	root.GetDefaultText().lock()->SetRelPos(Vector2D<int>(10,10));

	level.Init();
	camera.SetWorld(&level);
	camera.SetWindow(&app);
	camera.SetViewSize(app.GetVideoSettings()->width, app.GetVideoSettings()->height);

	LoadWorld("../Worlds/island.world");

	return true;
}


/*=============================================================================
--
=============================================================================*/
void UpdatePlayerInventory()
{
	if (app.GetInput()->GetKeyState(GK_C, KEY_RELEASED))
		level.GetPlayer()->GetTarget().lock()->GetInventory().lock()->CycleActiveObject();

	if (app.GetInput()->GetKeyState(GK_X, KEY_RELEASED))
		level.GetPlayer()->GetTarget().lock()->DropActiveObject();
}


/*=============================================================================
--
=============================================================================*/
void UpdatePlayerMovement()
{
	double SP = speedMult;
	#define DIAG 0.7071067811865475*SP

	bool up = app.GetInput()->GetKeyState(GK_W, KEY_DOWN);
	bool down = app.GetInput()->GetKeyState(GK_S, KEY_DOWN);
	bool left = app.GetInput()->GetKeyState(GK_A, KEY_DOWN);
	bool right = app.GetInput()->GetKeyState(GK_D, KEY_DOWN);

	//west
	if (!up && !down && left && !right)
		camera.MoveTarget(-SP, 0.0, 0.0);
	//north west
	if (up && !down && left && !right)
		camera.MoveTarget(-DIAG, -DIAG, 0.0);
	//north
	else if (up && !down && !left && !right)
			camera.MoveTarget(0.0, -SP, 0.0);
	//north east
	else if (up && !down && !left && right)
		camera.MoveTarget(DIAG, -DIAG, 0.0);
	//east
	else if (!up && !down && !left && right)
		camera.MoveTarget(SP, 0.0, 0.0);
	//south east
	else if (!up && down && !left && right)
		camera.MoveTarget(DIAG, DIAG, 0.0);
		//south
	else if (!up && down && !left && !right)
		camera.MoveTarget(0.0, SP, 0.0);
	//south west
	else if (!up && down && left && !right)
		camera.MoveTarget(-DIAG, DIAG, 0.0);

	if (app.GetInput()->GetKeyState(GK_Q, ge::KEY_DOWN))
		level.GetPlayer()->GetTarget().lock()->Move(0.0,0.0,speedMult*0.2);
}

Timer fpsClockUpdate;

/*=============================================================================
--
=============================================================================*/
void ge::ClientLoop()
{
	if (app.GetWasResized())
		camera.SetViewSize(app.GetWidth(), app.GetHeight());

	level.Update(app.GetLastUpdateTime());

	camera.Draw();

	root.Update();
	

	UpdatePlayerInventory();
	UpdatePlayerMovement();

	/*if (fpsClockUpdate.GetTimeAndEnd() > 250)
	{
		root.GetDefaultText().lock()->SetText(String() + "FPS [" + ToString(Round(app.GetUpdateFPS()),0) + "]");
		fpsClockUpdate.Start();
	}*/

	/*WeakPtr<ge::world::Entity> entityPos2  = level.GetEntityManager()->GetEntity(1);
	Vector3D<double> pos = entityPos2.lock()->GetPos();
	root.GetDefaultText().lock()->SetText(String() + ToString(pos.x) + " | " + ToString(pos.y) + " | " + ToString(pos.z));
	root.Draw();*/

	root.Draw();

	//increase decrease speed
	if (app.GetInput()->GetKeyState(GK_LShift, KEY_DOWN))
	{
		if (app.GetInput()->GetKeyState(GK_F1, KEY_RELEASED))
			speedMult -= 0.5;
		if (app.GetInput()->GetKeyState(GK_F2, KEY_RELEASED))
			speedMult += 0.5;
	}
	else
	{
		if (app.GetInput()->GetKeyState(GK_F1, KEY_RELEASED))
			speedMult -= 0.1;
		if (app.GetInput()->GetKeyState(GK_F2, KEY_RELEASED))
			speedMult += 0.1;
	}

	if (app.GetInput()->GetKeyState(GK_F4, KEY_DOWN))
		LoadWorld("../Worlds/island.world");
	else if (app.GetInput()->GetKeyState(GK_F5, KEY_DOWN))
		LoadWorld("../Worlds/staircase.world");
	else if (app.GetInput()->GetKeyState(GK_F6, KEY_DOWN))
		LoadWorld("../Worlds/test.world");
}
