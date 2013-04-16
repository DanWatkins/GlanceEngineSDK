/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Text.h
=================================================================================================*/

#include "../../GUI.h"

#ifndef _ge_TEXT_H
#define _ge_TEXT_H

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- Text element to display text. It can also wrap inside of a rectangle.
		=============================================================================*/
		class Text : public Element
		{
		private:
			String mText;
			Font *mFont;
			TextAlignment mTextAlignment;

		protected:
			//general
			void _Draw();
			void _UpdateAssets();


		public:
			friend class Root;
			friend class GroupBox;
			friend class ContextMenu;
			friend class MenuBar;
			friend class ListBox;

			Text();

			//accessors and mutators
			void SetText(String text) { mText = text; }
			void AppendText(String text) { mText.PushBack(text); }
			String GetText() { return mText; }
			void SetFont(Font *font) { mFont = font; }
			Font *GetFont() { return mFont; }
			void SetTextAlignment(TextAlignment alignment) { mTextAlignment = alignment; }
			TextAlignment GetTextAlignment() { return mTextAlignment; }
		};
	};
};

#endif

