/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: ChecKBox.h
=================================================================================================*/

#include "../../GUI.h"

#ifndef _ge_CHECKBOX_H
#define _ge_CHECKBOX_H

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- CheckBox that allows the user to set a boolean flag.
		=============================================================================*/
		class CheckBox : public Element
		{
		private:
			Sprite mImgChecked, mImgUnchecked;
			Sprite mImgCheckedDisabled, mImgUncheckedDisabled;
			String mCaption;										//caption aligned to the right
			bool mPressed;											//true for the first frame it is down, then false the rest
			TextAlignment mTextAlignment;							//specifies the side the text is on

		protected:
			void _Update();
			bool _PostInit();
			void _Draw();
			void _SendMessage(Message message);
			void _UpdateAssets();


		public:
			friend class Root;

			CheckBox();
			~CheckBox() {}

			void SetChecked(bool checked);

			//accessor and mutators
			void SetCaption(String caption) { mCaption = caption; }
			String GetCaption() { return mCaption; }
			void SetTextAlginment(TextAlignment textAlignment) { mTextAlignment = textAlignment; }
			TextAlignment GetTextAlignment() { return mTextAlignment; }
		};
	};
};


#endif

