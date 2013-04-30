/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: ComboBox.h
  Created: 2/26/13
=================================================================================================*/

#include "../../GUI.h"

#ifndef _ge_COMBOBOX_H
#define _ge_COMBOBOX_H

namespace ge
{
	namespace gui
	{
		const int COMBOBOX_HEIGHT				= 16;
		const int COMBOBOX_DROPDOWN_BUTTON_SIZE	= 16;

		/*=============================================================================
		-- ComboBox...duh.
		=============================================================================*/
		class ComboBox : public Element
		{
		private:
			Rectangle mBackground;
			WeakPtr<ListBox> mListBox;
			WeakPtr<ButtonImage> mDropDownButton;
			unsigned mMaxVerticalSpan;					//max height before scroll bars will be used (listbox and combobox combined)
			WeakPtr<Cell> mLastSelection;

			bool mMblJustPressed;
			bool mMblDownLastUpdate;

		protected:
			bool _PostInit();
			void _Update();
			void _Draw();
			void _UpdateAssets();


		public:
			friend class Root;

			ComboBox();

			WeakPtr<Cell> AddCell(String name, String uid);

			void ShowListBox(bool show);
			bool GetShowingListBox();
			void SetMaxVerticalSpan(unsigned maxVerticalSpan) { mMaxVerticalSpan = maxVerticalSpan; _UpdateAssets(); }
			unsigned GetMaxVerticalSpan() { return mMaxVerticalSpan; }
		};
	};
};


#endif

