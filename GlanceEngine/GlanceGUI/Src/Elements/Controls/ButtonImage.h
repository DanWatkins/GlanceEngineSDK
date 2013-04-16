/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: ButtonImage.h
  Created: 11/21/12 by Dan Watkins
=================================================================================================*/

#include "../../GUI.h"

#ifndef _ge_BUTTON_IMAGE_H
#define _ge_BUTTON_IMAGE_H

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- A button with text in the middle.
		=============================================================================*/
		class ButtonImage : public Element
		{
		private:
			Sprite mImage;
			String mImageName;

			void _SetImageName(String imageName);

		protected:
			void _Update();
			bool _PostInit();
			void _Draw();
			void _SendMessage(Message message);
			void _UpdateAssets();

		public:
			friend class Root;
			ButtonImage();
		};
	};
};


#endif

