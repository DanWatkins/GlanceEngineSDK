/*=================================================================================================
  Glance World Editor - (C) 2013 Daniel L. Watkins

  Filename: WorldEditor.h
  Created: 4/20/2013 @ 20:42 UTC-6
=================================================================================================*/
#ifndef _gwe_WORLD_EDITOR_H
#define _gwe_WORLD_EDITOR_H

#include "Main.h"

namespace ge
{
	namespace gwe
	{
		class WorldEditor
		{
		private:
			gui::Root *mRoot;
			SharedPtr<world::World> mWorld;
			SharedPtr<world::Camera> mCamera;

			//element shortcuts
			WeakPtr<ContextMenu> mFileMenu;
			WeakPtr<DialogBox> mOpenDialog;
			WeakPtr<Panel> mTilesetPanel;

			void _CreateUserInterface();
			void _CreateOpenDialog();
			
		public:
			WorldEditor(gui::Root *root) { mRoot = root; }

			void Init();
			void Update();

			void LoadWorld(String worldPath);
			void _UpdateScrolling();

			gui::Root *GetRoot() { return mRoot; }
		};
	};
};

#endif

