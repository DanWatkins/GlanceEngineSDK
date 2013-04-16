/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: DialogBox.h
=================================================================================================*/

#include "../../GUI.h"

#ifndef _ge_DIALOGBOX_H
#define _ge_DIALOGBOX_H

namespace ge
{
	namespace gui
	{
		const int TITLE_BAR_HEIGHT = 32;
		const int CLOSE_BUTTON_PAD = 4;
		const int CLOSE_BUTTON_SIZE = 24;

		/*=============================================================================
		-- DialogBox to house elements that can be moved around together.
		=============================================================================*/
		class DialogBox : public Element
		{
		private:
			SpriteFrame mImgTitleBar, mImgClientArea;
			WeakPtr<Drager> mTitleBarDrager;
			String mCaption;
			WeakPtr<ButtonImage> mCloseButton;

		protected:
			bool _PostInit();
			void _Update();
			void _Draw();
			void _UpdateAssets();

		public:
			friend class Root;

			DialogBox() {}

			void SetCaption(String caption);
		};
	};
};


#endif

