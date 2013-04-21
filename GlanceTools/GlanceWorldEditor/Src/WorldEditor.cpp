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
		void WorldEditor::_CreateUserInterface()
		{
			using namespace gui;

			if (!mRoot)
				gDebug.Error(String()+"Bad root object. Cannot create user interface for WorldEditor instance "+ToString(this));

			//create the main menu bar
			WeakPtr<MenuBar> menuBar = mRoot->CreateMenuBar(gwe::EID_MENU_BAR_MAIN, -1, Vector2D<int>(), mRoot->GetWindow()->GetWidth());
				mFileMenu = menuBar.lock()->AddMenu("File");
					 mFileMenu.lock()->AddItem("Open");
					mFileMenu.lock()->AddItem("Exit");
				WeakPtr<ContextMenu> cm2 = menuBar.lock()->AddMenu("Help");
					cm2.lock()->AddItem("About");
		}


		void WorldEditor::_CreateOpenDialog()
		{
			mOpenDialog = mRoot->CreateDialogBox(EID_OPEN_DIALOG, -1, Vector2D<int>(50,50), 300, 150, "Open World");
		}


		void WorldEditor::Init()
		{
			_CreateUserInterface();
		}


		void WorldEditor::Update()
		{
			if (mFileMenu.lock()->GetSelectedCaption() == "Open"  &&  mOpenDialog.expired())
				_CreateOpenDialog();
		}
	};
};