/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: ButtonCaption.h
=================================================================================================*/

#include "../../GUI.h"

#ifndef _ge_BUTTON_CAPTION_H
#define _ge_BUTTON_CAPTION_H

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- A button with text in the middle.
		=============================================================================*/
		class ButtonCaption : public Element
		{
		private:
			SpriteFrame mImgUp, mImgDown;
			String mCaption;


		protected:
			void _Update();
			bool _PostInit();
			void _Draw();
			void _SendMessage(Message message);
			void _UpdateAssets();


		public:
			friend class Root;

			ButtonCaption();

			void SetCaption(String caption);
		};
	};
};


#endif

