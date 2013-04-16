/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: ListBox.h
=================================================================================================*/

#include "../../GUI.h"

#ifndef _ge_LISTBOX_H
#define _ge_LISTBOX_H

namespace ge
{
	namespace gui
	{
		const int LISTBOX_CELL_PADDING = 3;
		const int LISTBOX_CELL_HEIGHT = 24;

		/*=============================================================================
		-- ListBox element to pop up a menu for making choices. Items in the menu
		   are just ContextMenus.
		=============================================================================*/
		class ListBox : public Element
		{
		private:
			SpriteFrame mImgBackground;				//background image,
			int mIdTrack;							//used to keep track of the ids available
			WeakPtr<ScrollBar> mScrollBar;
			std::vector< WeakPtr<Cell> > mCells;	//all cells are also children
			std::vector< WeakPtr<Cell> > mVisibleCells;
			WeakPtr<Cell> mSelectedCell;

		protected:
			bool _PostInit();
			void _Update();
			void _Draw();
			void _SendMessage(Message message);
			void _UpdateAssets();

			void _CalculateVisibleCells();


		public:
			friend class Root;
			friend class ComboBox;

			ListBox();

			WeakPtr<Cell> GetCell(int n);
			void SelectCell(WeakPtr<Cell> cell);
			int GetCellCount();
			WeakPtr<Cell> AddCell(String name, String uid);
			int GetCellWidth();
			int GetCellHeight();
			WeakPtr<Cell> GetCellAtPoint(Vector2D<int> point);
			WeakPtr<Cell> GetSelectedCell() { return mSelectedCell; }
			void SetHeightForViewableCellCount(int numberOfVisibleCells);
			Rect<int> GetScrollBarRectRegion();
		};
	};
};


#endif

