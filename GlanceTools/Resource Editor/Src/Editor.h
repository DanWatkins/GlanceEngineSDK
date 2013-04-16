/*=================================================================================================
  Glance Resource Editor - (C) 2013 Daniel L. Watkins

  Filename: Editor.h
  Created: 2/12/2013
=================================================================================================*/
#ifndef gre_EDITOR_H
#define gre_EDITOR_H

#include "Main.h"

namespace ge
{
	namespace gre
	{
		/*=============================================================================
		-- Abstract base Editor class that declares essential functionality such as
		   saving and reloading content.
		=============================================================================*/
		class Editor
		{
		private:
			gui::Root *mRoot;

		public:
			Editor() { mRoot = NULL; }

			virtual void CreateUserInterface() = 0;
			virtual bool LoadContent() = 0;
			virtual void SaveContent() = 0;
			virtual void Update() {}

			void SetRoot(gui::Root *root) { mRoot = root; }
			gui::Root *GetRoot() { return mRoot; }
		};
	}
}

#endif

