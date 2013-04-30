/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: ListBox.cpp
=================================================================================================*/

#include "../../GUI.h"

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- Constructor for ListBox.
		=============================================================================*/
		ListBox::ListBox()
		{
			mIdTrack		= 2;
		}


		/*=============================================================================
		-- Initializes the ListBox after it has been added to the Root.
		   Automatically called from its corresponding Root::Create() method.
		=============================================================================*/
		bool ListBox::_PostInit()
		{
			mImgBackground.Init(GetWidth(), GetHeight(), GetWindow(), true);
			((FrameImage*)GetRoot()->GetAssetManager()
									->GetAsset("listbox_background"))->PassImagesToSprite(&mImgBackground);

			mScrollBar = GetRoot()->CreateScrollBar(-1, _GetAsWeakPtr(),
													Vector2D<int>(GetWidth()-SCROLL_BAR_BUTTON_SIZE-3,3),
													SCROLL_BAR_BUTTON_SIZE, GetHeight()-6,
													SpanDirection::VERTICAL);

			mScrollBar.lock()->SetScrollPosition(0.0);

			return true;
		}


		/*=============================================================================
		-- Updates the ListBox.
		=============================================================================*/
		void ListBox::_Update()
		{
			_CalculateVisibleCells();

			mScrollBar.lock()->SetVirtualScrollSize(mCells.size()*GetCellHeight());
		}


		/*=============================================================================
		-- Draws the button depending on the state and the caption overlay.
		=============================================================================*/
		void ListBox::_Draw()
		{
			mImgBackground.Draw();
		}


		/*=============================================================================
		-- Message handling.
		=============================================================================*/
		void ListBox::_SendMessage(Message message)
		{
			switch (message)
			{
				//was a cell selected?
				case MBL_DOWN_OVER:
				{
					if (!mSelectedCell.expired())
						mSelectedCell.lock()->SetBackgroundImage("cell.png");

					SelectCell(GetCellAtPoint(GetWindow()->GetInput()->GetMousePos()));

					if (!mSelectedCell.expired())
						mSelectedCell.lock()->SetBackgroundImage("cell_selected.png");

					break;
				}
			}
		}


		void ListBox::SelectCell(WeakPtr<Cell> cell)
		{
			//ensure the cell is actually a cell in the listbox
			std::vector< WeakPtr<Cell> >::iterator iter = mCells.begin();
			while (iter != mCells.end())
			{
				if ((*iter).lock() == cell.lock())
				{
					mSelectedCell = cell;
					_SendElementMessageToListeners(ElementEvent::SELECTED, mSelectedCell.lock()->GetUID());
					return;
				}

				iter++;
			}
		}


		/*=============================================================================
		-- Returns the cell at the absolute screen position @point. If the point is not
		   within the bounds of where cells are, then no cell will be returned.
		=============================================================================*/
		WeakPtr<Cell> ListBox::GetCellAtPoint(Vector2D<int> point)
		{
			_CalculateVisibleCells();

			Vector2D<int> screenPos = GetScreenPos();

			if (point.x > screenPos.x+LISTBOX_CELL_PADDING  &&  point.x < screenPos.x+GetCellWidth()+LISTBOX_CELL_PADDING)
			{
				int selectedVisibleCellIndex = (point.y-screenPos.y-LISTBOX_CELL_PADDING) / GetCellHeight();

				if (selectedVisibleCellIndex < (int)mVisibleCells.size()  &&  selectedVisibleCellIndex >= 0)
					return mVisibleCells.at(selectedVisibleCellIndex);
			}

			return WeakPtr<Cell>();
		}


		/*=============================================================================
		-- Updates the position and size of each asset.
		=============================================================================*/
		void ListBox::_UpdateAssets()
		{
			mImgBackground.SetPos(GetScreenPos());
			mImgBackground.SetSize(GetWidth(), GetHeight());

			//tell every cell the new sizes
			std::vector< WeakPtr<Cell> >::iterator iter = mVisibleCells.begin();
			while (iter != mVisibleCells.end())
			{
				(*iter).lock()->SetSize(GetCellWidth(), GetCellHeight());
				iter++;
			}
		}


		/*=============================================================================
		-- Returns the cell at the specified index.
		=============================================================================*/
		WeakPtr<Cell> ListBox::GetCell(int n)
		{
			return mCells.at(n);
		}


		/*=============================================================================
		-- Returns the number of cells.
		=============================================================================*/
		int ListBox::GetCellCount()
		{
			return mCells.size();
		}


		/*=============================================================================
		-- Adds a new item to the context menu. Items are just Picture objects.
		=============================================================================*/
		WeakPtr<Cell> ListBox::AddCell(String name, String uid)
		{
			mCells.push_back( GetRoot()->CreateCell(-mIdTrack, DynamicPtrCast<ListBox>(_GetAsWeakPtr().lock()), Vector2D<int>(0,0),
													GetCellWidth(), GetCellHeight()) );

			mCells.back().lock()->SetBackgroundImage("cell.png");
			mCells.back().lock()->SetSolid(false);

			mCells.back().lock()->SetInternalCaption(GetRoot()->CreateText(-1243, mCells.back(), Vector2D<int>(2,2), name));
			mCells.back().lock()->SetUID(uid);

			mIdTrack++;

			return mCells.back();
		}


		/*=============================================================================
		-- Returns the width in pixels cells should be.
		=============================================================================*/
		int ListBox::GetCellWidth()
		{
			return GetWidth()-LISTBOX_CELL_PADDING*2-SCROLL_BAR_BUTTON_SIZE;
		}
	

		/*=============================================================================
		-- Returns the height in pixels cells should be.
		=============================================================================*/
		int ListBox::GetCellHeight()
		{
			return LISTBOX_CELL_HEIGHT;
		}


		/*=============================================================================
		-- Populates @mVisibleCells with the cells that are visible.
		=============================================================================*/
		void ListBox::_CalculateVisibleCells()
		{
			int totalCells = mChildren.size()-1;  //TODO we are assuming all children are cells and one is the scroll bar
			int cellViewHeight = GetHeight() - LISTBOX_CELL_PADDING*2;
			int cellsThatCanBeAtTop = totalCells - (cellViewHeight/GetCellHeight());
			
			if (cellsThatCanBeAtTop < 0)
				cellsThatCanBeAtTop = totalCells;

			int currentTopCellIndex = (int)std::floor((double)cellsThatCanBeAtTop * mScrollBar.lock()->GetScrollPosition());

			mVisibleCells.clear();
			
			//copy over the visible cells from the mChildren vector to the mVisibleCells vector
			int cellIndex = 0;	//first child is scrollbar
			int relativeCellIndex = 0;
			std::vector< WeakPtr<Cell> >::iterator iter = mCells.begin();

			while (iter != mCells.end())
			{
				if (cellIndex >= currentTopCellIndex  &&  cellIndex < currentTopCellIndex+(cellViewHeight/GetCellHeight()))
				{
					mVisibleCells.push_back(*iter);
					(*iter).lock()->SetDraws(true);
					(*iter).lock()->SetDisabled(false);

					Vector2D<int> relPos;
					relPos.x = LISTBOX_CELL_PADDING;
					relPos.y = LISTBOX_CELL_PADDING + GetCellHeight()*relativeCellIndex;
					(*iter).lock()->SetRelPos(relPos);

					relativeCellIndex++;
				}
				else if (cellIndex != -1)  //don't want to disable scroll bar
				{
					(*iter).lock()->SetDraws(false);
					(*iter).lock()->SetDisabled(true);
				}

				cellIndex++;
				iter++;
			}
		}


		void ListBox::SetHeightForViewableCellCount(int numberOfVisibleCells)
		{
			SetSize(GetWidth(), numberOfVisibleCells*GetCellHeight() + LISTBOX_CELL_PADDING*2);
		}


		Rect<int> ListBox::GetScrollBarRectRegion()
		{
			Rect<int> scrollBarRect = Rect<int>(mScrollBar.lock()->GetScreenPos().x, mScrollBar.lock()->GetScreenPos().y,
											 mScrollBar.lock()->GetWidth(), mScrollBar.lock()->GetHeight());

			return scrollBarRect;
		}
	};
};