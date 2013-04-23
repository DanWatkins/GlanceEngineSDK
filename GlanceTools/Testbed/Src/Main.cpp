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



	root.CreateDialogBox(123123, -1, Vector2D<int>(40,60), 500, 400, "Dialog Box");
		WeakPtr<TabBar> tabBar = root.CreateTabBar(26, 123123, Vector2D<int>(3, 40), 494, 357);
			WeakPtr<TabPage> page1 = tabBar.lock()->AddPage("Tab Page 1");
				WeakPtr<GroupBox> gbox1 = root.CreateGroupBox(325125, page1, Vector2D<int>(10,40), 474, 307, "Groupbox");
					root.CreateButtonCaption(572389571094, gbox1, Vector2D<int>(10,15), 120, 24, "Button");
					root.CreateCheckBox(93572915, gbox1, Vector2D<int>(90,60), "Checkbox");
					root.CreateEditBox(25923579, gbox1, Vector2D<int>(10,95), 120, 24, "Edit Box");
					root.CreateSlider(253820, gbox1, Vector2D<int>(10,140), 120);

				WeakPtr<ComboBox> cbox123 = root.CreateComboBox(235235, gbox1, Vector2D<int>(160,15), 200);
					cbox123.lock()->AddCell("ComboBox - Option 1", "");
				WeakPtr<ListBox> lbox123 = root.CreateListBox(325325, gbox1, Vector2D<int>(160,50), 200, 200);
					for (int n=0; n<30; n++)
						lbox123.lock()->AddCell(String()+"Cell "+ToString(n), ToString(n));

					WeakPtr<ProgressBar> pbar123 = root.CreateProgressBar(153235, gbox1, Vector2D<int>(10,267), 454, 30, 72.0);
					pbar123.lock()->SetPercent(72.0);

			WeakPtr<TabPage> page2 = tabBar.lock()->AddPage("Tab Page 2");
			WeakPtr<TabPage> page3 = tabBar.lock()->AddPage("Tab Page 3");

	WeakPtr<MenuBar> menuBar = root.CreateMenuBar(2, -1, Vector2D<int>(0,0), app.GetVideoSettings()->width);
		WeakPtr<ContextMenu> fileMenu = menuBar.lock()->AddMenu("File");
			WeakPtr<ContextMenu> newMenu = fileMenu.lock()->AddItem("New");
				newMenu.lock()->AddItem("World");
				newMenu.lock()->AddItem("Resource Pack");
			fileMenu.lock()->AddItem("Open");
			fileMenu.lock()->AddItem("Save");
			fileMenu.lock()->AddItem("Save As...");
			fileMenu.lock()->AddItem("Save All");
			fileMenu.lock()->AddItem("Export Resource  ");
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

}


/*=============================================================================
--
=============================================================================*/
bool ge::ClientInit()
{
	LoadConfigurationFile();

	root.Init(&app);
	BuildInterface();
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

	//root.Draw();

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
