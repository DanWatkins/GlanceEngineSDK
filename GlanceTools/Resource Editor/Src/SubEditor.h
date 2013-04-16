/*=================================================================================================
  Glance Resource Editor - (C) 2013 Daniel L. Watkins

  Filename: SubEditor.h
  Created: 2/13/2013
=================================================================================================*/
#ifndef gre_SUB_EDITOR_H
#define gre_SUB_EDITOR_H

#include "Main.h"

namespace ge
{
	namespace gre
	{
		/*=============================================================================
		-- Abstract base Editor class that declares essential functionality such as
		   saving and reloading content.
		=============================================================================*/
		class SubEditor : public Editor
		{
		private:
			ResourceEditor *mResourceEditor;
			WeakPtr<gui::TabPage> mEditorTabPage;

		public:
			SubEditor() { mResourceEditor = NULL; }

			void SetResourceEditor(ResourceEditor *resourceEditor) { mResourceEditor = resourceEditor; }
			ResourceEditor *GetResourceEditor() { return mResourceEditor; }
			void SetEditorTabPage(WeakPtr<gui::TabPage> tabPage) { mEditorTabPage = tabPage; }
			WeakPtr<gui::TabPage> GetEditorTabPage() { return mEditorTabPage; }
		};
	}
}

#endif

