/*=================================================================================================
  Glance Resource Editor - (C) 2013 Daniel L. Watkins

  Filename: ResourceEditor.cpp
  Created: 2/12/2013
=================================================================================================*/
#include "Main.h"

namespace ge
{
	namespace gre
	{
		/*=============================================================================
		-- Creates the main shell of the applications UI.
		=============================================================================*/
		void ResourceEditor::CreateUserInterface()
		{
			Window *window = GetRoot()->GetWindow();

			WeakPtr<MenuBar> menuBar = GetRoot()->CreateMenuBar(EID_MAIN_MENU_BAR, EID_ROOT, Vector2D<int>(0,0), window->GetWidth());
				WeakPtr<ContextMenu> fileMenu = menuBar.lock()->AddMenu("File");
					fileMenu.lock()->AddItem("Reload (Ctrl+R)");
					fileMenu.lock()->AddItem("Save (Ctrl+S)");
					fileMenu.lock()->AddItem("Exit (Alt+F4)");
				WeakPtr<ContextMenu> editMenu = menuBar.lock()->AddMenu("Edit");
				WeakPtr<ContextMenu> viewMenu = menuBar.lock()->AddMenu("View");
				WeakPtr<ContextMenu> helpMenu = menuBar.lock()->AddMenu("Help");


			WeakPtr<TabBar> tabBar = GetRoot()->CreateTabBar(EID_RESOURCE_TAB_BAR, EID_ROOT, Vector2D<int>(0,40), window->GetWidth(), window->GetHeight()-40);
				WeakPtr<TabPage> entityPage = tabBar.lock()->AddPage("Entities");
					mEntityEditor = SharedPtr<EntityEditor>(new EntityEditor(GetRoot(), this, entityPage));
					mEntityEditor->CreateUserInterface();
					mEntityEditor->LoadContent();
				WeakPtr<TabPage> graphicsPage = tabBar.lock()->AddPage("Graphics");
				WeakPtr<TabPage> soundsPage = tabBar.lock()->AddPage("Sounds");
				WeakPtr<TabPage> animationsPage = tabBar.lock()->AddPage("Animations");
				WeakPtr<TabPage> tilesetsPage = tabBar.lock()->AddPage("Tilesets");
		}


		/*=============================================================================
		-- Loads the content for the entire application. All sub-editors will also
		   have their specific content loaded.
		=============================================================================*/
		bool ResourceEditor::LoadContent()
		{
			return true;
		}


		/*=============================================================================
		-- Saves the content for the entire application. All sub-editors will also
		   have their specific content saved.
		=============================================================================*/
		void ResourceEditor::SaveContent()
		{
		}


		void ResourceEditor::Update()
		{
			mEntityEditor->Update();
		}
	}
}