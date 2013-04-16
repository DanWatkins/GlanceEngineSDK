/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Cell.h
  Created: 2/2/13 by Dan Watkins
=================================================================================================*/

#include "../../GUI.h"

#ifndef _ge_CELL_H
#define _ge_CELL_H

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- The core functionality for all GUI elements.
		=============================================================================*/
		class Cell : public Element
		{
		private:
			Sprite mBackgroundImage;
			WeakPtr<Text> mCaption;
			String mUID;

		protected:
			bool _PostInit();
			void _Draw();
			void _SendMessage(Message message) {}
			void _UpdateAssets();

		public:
			void SetInternalCaption(WeakPtr<Text> caption) { mCaption = caption; }

			void SetBackgroundImage(String name);
			String GetCaption() { return mCaption.lock()->GetText(); }
			void SetUID(String uid) { mUID = uid; }
			String GetUID() { return mUID; }
		};
	};
};

#endif

