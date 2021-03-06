/*=================================================================================================
  Glance World Editor - (C) 2013 Daniel L. Watkins

  Filename: WorldEditor.cpp
  Created: 4/20/2013 @ 20:42 UTC-6
=================================================================================================*/
#include "Main.h"

namespace ge
{
	namespace gwe
	{
		/*=============================================================================
		-- Generates the default user interface at start up.
		=============================================================================*/
		void WorldEditor::_CreateUserInterface()
		{
			using namespace gui;

			if (!mRoot)
				gDebug.Error(String()+"Bad root object. Cannot create user interface for WorldEditor instance "+ToString(this));
			else
			{
				//create the main menu bar
				WeakPtr<MenuBar> menuBar = mRoot->CreateMenuBar(gwe::EID_MENU_BAR_MAIN, -1, Vector2D<int>(), mRoot->GetWindow()->GetWidth());
					mFileMenu = menuBar.lock()->AddMenu("File");
						 mFileMenu.lock()->AddItem("Open");
						mFileMenu.lock()->AddItem("Exit");
					WeakPtr<ContextMenu> cm2 = menuBar.lock()->AddMenu("Help");
						cm2.lock()->AddItem("About");
			}
		}


		/*=============================================================================
		-- Creates the open world dialog.
		=============================================================================*/
		void WorldEditor::_CreateOpenDialog()
		{
			int width = 300;
			int height = 600;
			Vector2D<int> centeredPosition(mRoot->GetWindow()->GetWidth()/2-width/2, mRoot->GetWindow()->GetHeight()/2-height/2);
			mOpenDialog = mRoot->CreateDialogBox(EID_OPEN_DIALOG, -1, centeredPosition, width, height, "Open World");
				WeakPtr<ListBox> worldListBox = mRoot->CreateListBox(EID_OPEN_WORLD_LISTBOX, mOpenDialog, Vector2D<int>(10,40), 200, 150);
				WeakPtr<ButtonCaption> openButton = mRoot->CreateButtonCaption(EID_OPEN_WORLD_BUTTON, mOpenDialog, Vector2D<int>(220,40), 70, 24, "Open");
				openButton.lock()->AddElementListener(this);
				mOpenDialog.lock()->AddElementListener(this);

			io::DirectoryListing worldListing = io::GetFilesInDirectory(DIR_WORLDS, ".world");
			io::DirectoryListing::iterator iter = worldListing.begin();
			while (iter != worldListing.end())
			{
				String shortendPath = io::Standardize(*iter);
				shortendPath.RemoveUntil("/",128,true);
				worldListBox.lock()->AddCell(shortendPath, *iter);

				iter++;
			}
		}


		/*=============================================================================
		-- Init
		=============================================================================*/
		void WorldEditor::Init()
		{
			_CreateUserInterface();
		}


		/*=============================================================================
		-- Update
		=============================================================================*/
		void WorldEditor::Update()
		{
			if (mWorld)
				mWorld->Update(1.0);

			_UpdateScrolling();

			if (mFileMenu.lock()->GetSelectedCaption() == "Open"  &&  mOpenDialog.expired())
				_CreateOpenDialog();

			/*if (mOpenDialog.expired() == false)
			{
				WeakPtr<ButtonCaption> openWorld = DynamicPtrCast<ButtonCaption>(mOpenDialog.lock()->GetChild(EID_OPEN_WORLD_BUTTON).lock());
				if (!openWorld.expired()  &&  openWorld.lock()->GetState() == State::RELEASED)
				{
					WeakPtr<ListBox> worldListBox = DynamicPtrCast<ListBox>(mOpenDialog.lock()->GetChild(EID_OPEN_WORLD_LISTBOX).lock());
					String worldPath = worldListBox.lock()->GetSelectedCell().lock()->GetUID();
					LoadWorld(worldPath);
					mOpenDialog.lock()->ScheduleToBeRemoved();
				}
			}*/
				

			mFileMenu.lock()->ClearSelectedCaption();

			if (mCamera)
				mCamera->Draw();
		}


		/*=============================================================================
		-- Callback method for responding to gui::Element messages.
		=============================================================================*/
		void WorldEditor::SendElementMessage(ElementEvent elementEvent, WeakPtr<Element> element, String eventParam)
		{
			std::cout << "ElemenetEvent [" << (int)elementEvent << "] for element instance at 0x" << element.lock().get() << " with event parameter " << eventParam.GetStd() << std::endl;
			int elementId = element.lock()->GetId();

			switch (elementId)
			{
				case EID_OPEN_WORLD_BUTTON:
				{
					if (elementEvent == ElementEvent::RELEASED)
					{
						WeakPtr<ListBox> worldListBox = DynamicPtrCast<ListBox>(mOpenDialog.lock()->GetChild(EID_OPEN_WORLD_LISTBOX).lock());
						String worldPath = worldListBox.lock()->GetSelectedCell().lock()->GetUID();
						LoadWorld(worldPath);
						mOpenDialog.lock()->ScheduleToBeRemoved();
					}
				}
			}
		}


		/*=============================================================================
		-- Loads the specified world and sets the camera to view it.
		=============================================================================*/
		void WorldEditor::LoadWorld(String worldPath)
		{
			mWorld = SharedPtr<world::World>(new world::World);
			mWorld->Init();
			mWorld->LoadWorldFromFile(worldPath);

			mCamera = SharedPtr<world::Camera>(new world::Camera);
			mCamera->SetWindow(mRoot->GetWindow());
			mCamera->SetWorld(mWorld.get());  //TODO the camera should store a WeakPtr to the world, not raw
			mCamera->SetViewSize(mRoot->GetWindow()->GetWidth(), mRoot->GetWindow()->GetHeight()-30);
			mCamera->SetScreenPos(0,30);

			mEntityListBox = mRoot->CreateListBox(1253, -1, Vector2D<int>(0,24), 160, mRoot->GetWindow()->GetHeight()-24);

			for (int n=0; n<mWorld->GetEntityManager()->TemplateEntityCount(); n++)
			{
				WeakPtr<world::Entity> templateEntity = mWorld->GetEntityManager()->GetTemplateEntity(n);
				mEntityListBox.lock()->AddCell(templateEntity.lock()->GetDisplayName(), templateEntity.lock()->GetName());
			}
		}


		/*=============================================================================
		-- Updates scrolling based on WASD
		=============================================================================*/
		void WorldEditor::_UpdateScrolling()
		{
			//TODO implement scrolling by holding right-click and moving
			//TODO implement scrolling by moving the cursor to the edge of the window

			if (!mCamera)
				return;

			double SP = 7.5;
			#define DIAG 0.7071067811865475*SP

			bool up = mRoot->GetWindow()->GetInput()->GetKeyState(GK_W, KEY_DOWN);
			bool down = mRoot->GetWindow()->GetInput()->GetKeyState(GK_S, KEY_DOWN);
			bool left = mRoot->GetWindow()->GetInput()->GetKeyState(GK_A, KEY_DOWN);
			bool right = mRoot->GetWindow()->GetInput()->GetKeyState(GK_D, KEY_DOWN);

			//west
			if (!up && !down && left && !right)
				mCamera->Move(-SP, 0.0, 0.0);
			//north west
			if (up && !down && left && !right)
				mCamera->Move(-DIAG, -DIAG, 0.0);
			//north
			else if (up && !down && !left && !right)
					mCamera->Move(0.0, -SP, 0.0);
			//north east
			else if (up && !down && !left && right)
				mCamera->Move(DIAG, -DIAG, 0.0);
			//east
			else if (!up && !down && !left && right)
				mCamera->Move(SP, 0.0, 0.0);
			//south east
			else if (!up && down && !left && right)
				mCamera->Move(DIAG, DIAG, 0.0);
				//south
			else if (!up && down && !left && !right)
				mCamera->Move(0.0, SP, 0.0);
			//south west
			else if (!up && down && left && !right)
				mCamera->Move(-DIAG, DIAG, 0.0);
		}
	};
};