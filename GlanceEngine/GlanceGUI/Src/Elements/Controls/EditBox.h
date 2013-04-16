/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: EditBox.h
=================================================================================================*/

#include "../../GUI.h"

#ifndef _ge_EDITBOX_H
#define _ge_EDITBOX_H

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- EditBox that allows the user to enter text on a single line.
		=============================================================================*/
		class EditBox : public Element
		{
		private:
			Sprite mImgCaret;				//caret that shows where the text entered will go
			SpriteFrame mImgBorder;			//border around the box (usually filled in)
			String mCaption;
			WeakPtr<Text> mText;			//text in the box
			Timer mCaretTimer;				//used to make the caret "blink"
			bool mCaretIsShowing;			//used to remember if timer is counting until it should be hidden or shown

			void _ForceShowCaretReset();

		protected:
			friend class Root;

			void _Update();
			bool _PostInit();
			void _Draw();
			void _SendMessage(Message message);
			void _UpdateAssets();

		public:
			EditBox();
			~EditBox() {}

			void SetCaption(String caption);
			String GetCaption() { return mCaption; }
		};
	};
};


#endif

