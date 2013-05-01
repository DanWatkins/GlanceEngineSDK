/*=================================================================================================
  Glance Resource Editor - (C) 2013 Daniel L. Watkins

  Filename: ResourceEditor.h
  Created: 2/12/2013
=================================================================================================*/
#ifndef gre_RESOURCE_EDITOR_H
#define gre_RESOURCE_EDITOR_H

#include "Main.h"

namespace ge
{
	namespace gre
	{
		/*=============================================================================
		-- Main editor class. Facilitates managment of the entire application and all
		   sub-editors such as EntityEditor and GraphicsEditor.
		=============================================================================*/
		class ResourceEditor : public Editor, public ElementListener
		{
		private:
			SharedPtr<EntityEditor> mEntityEditor;

		public:
			ResourceEditor(gui::Root *root) { SetRoot(root); }

			void SendElementMessage(ElementEvent elementEvent, WeakPtr<Element> element, String eventParam);

			void CreateUserInterface();
			bool LoadContent();
			void SaveContent();
			void Update();
		};
	}
}

#endif

