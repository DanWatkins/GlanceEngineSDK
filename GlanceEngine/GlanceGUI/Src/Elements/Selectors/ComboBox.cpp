/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: ComboBox.cpp
  Created: 2/26/13
=================================================================================================*/

#include "../../GUI.h"

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- Constructor for ComboBox.
		=============================================================================*/
		ComboBox::ComboBox()
		{
			mMaxVerticalSpan		= 300;
			mMblJustPressed			= false;
			mMblDownLastUpdate		= false;
		}


		/*=============================================================================
		-- Further initializes the ComoboBox after it has been created with a
		   corresponding Root::Create() method.
		=============================================================================*/
		bool ComboBox::_PostInit()
		{
			mListBox = GetRoot()->CreateListBox(IEI_COMBOBOX_LISTBOX, _GetAsWeakPtr(),
												Vector2D<int>(0,COMBOBOX_DROPDOWN_BUTTON_SIZE), GetWidth(), 100);
			ShowListBox(false);

			mDropDownButton = GetRoot()->CreateButtonImage(IEI_COMBOBOX_DROPDOWN_BUTTON, _GetAsWeakPtr(),
															Vector2D<int>(GetWidth()-COMBOBOX_DROPDOWN_BUTTON_SIZE,0),
															COMBOBOX_DROPDOWN_BUTTON_SIZE,
															COMBOBOX_DROPDOWN_BUTTON_SIZE,
															"combobox_dropdown.png");
			mBackground = Rectangle(GetWindow());
			mBackground.SetColor(gGray);
			mBackground.SetFilled(true);
			mBackground.SetP1(GetScreenPos());
			mBackground.SetP2(GetScreenPos().x+GetWidth(), GetScreenPos().y+GetHeight());

			return true;
		}


		/*=============================================================================
		-- Updates the ComboBox.
		=============================================================================*/
		void ComboBox::_Update()
		{
			Vector2D<int> cursorPos = GetWindow()->GetInput()->GetMousePos();
			bool mblDown = GetWindow()->GetInput()->GetButtonState(GMB_Left);

			if (mblDown && !mMblDownLastUpdate)
				mMblJustPressed = true;
			else if (mMblDownLastUpdate)
				mMblJustPressed = false;

			//drop down button was released
			if (mDropDownButton.lock()->GetState() == State::RELEASED)
				ShowListBox(!GetShowingListBox());
			//there was a click outside the box or a selection was made
			else if (mMblJustPressed)
			{
				Rect<int> dropButtonRect = Rect<int>(mDropDownButton.lock()->GetScreenPos().x, mDropDownButton.lock()->GetScreenPos().y,
											 mDropDownButton.lock()->GetWidth(), mDropDownButton.lock()->GetHeight());
				Rect<int> scrollBarRect = mListBox.lock()->GetScrollBarRectRegion();

				if (GetShowingListBox()  &&  mblDown  &&
						 !dropButtonRect.Intersect(cursorPos)  &&  !scrollBarRect.Intersect(cursorPos))
				{
					ShowListBox(false);

					//let the list box know a selection has been made
					//save the old selection because stray clicks cause the
					//selection to get nulled (undesired result)
					WeakPtr<Cell> oldSelectedCell = mListBox.lock()->GetSelectedCell();
					mListBox.lock()->_SendMessage(MBL_DOWN_OVER);

					if (mListBox.lock()->GetSelectedCell().lock().get() != mLastSelection.lock().get())
						_SendElementMessageToListeners(ElementEvent::SELECTED, mListBox.lock()->GetSelectedCell().lock()->GetUID());

					if (mListBox.lock()->GetSelectedCell().expired())
						mListBox.lock()->SelectCell(oldSelectedCell);
				}
			}

			mMblDownLastUpdate = mblDown;
			mLastSelection = mListBox.lock()->GetSelectedCell();
		}


		/*=============================================================================
		-- Draws the ComboBox.
		=============================================================================*/
		void ComboBox::_Draw()
		{
			//draw a background
			mBackground.Draw();

			if (!mListBox.lock()->GetSelectedCell().expired())
				GetRoot()->GetDefaultFont()->Print(mListBox.lock()->GetSelectedCell().lock()->GetCaption(),
													GetScreenPos().x + 4, GetScreenPos().y);
		}


		/*=============================================================================
		-- 
		=============================================================================*/
		void ComboBox::_UpdateAssets()
		{
			//make sure the listbox is not too tall, but is an even size for cells
			if (!mListBox.expired())
			{
				int cellHeight = mListBox.lock()->GetCellHeight();
				int maxListBoxSpan = GetMaxVerticalSpan()-GetHeight();
				int maxCellsWithinHeight = maxListBoxSpan/cellHeight;
				mListBox.lock()->SetHeightForViewableCellCount(maxCellsWithinHeight);
			}

			mBackground.SetP1(GetScreenPos());
			mBackground.SetP2(GetScreenPos().x+GetWidth(), GetScreenPos().y+GetHeight());
		}


		/*=============================================================================
		-- 
		=============================================================================*/
		WeakPtr<Cell> ComboBox::AddCell(String name, String uid)
		{
			WeakPtr<Cell> cell = mListBox.lock()->AddCell(name, uid);
			_UpdateAssets();

			return cell;
		}


		/*=============================================================================
		-- 
		=============================================================================*/
		void ComboBox::ShowListBox(bool show)
		{
			mListBox.lock()->SetDraws(show);
			mListBox.lock()->SetSolid(show);
		}


		bool ComboBox::GetShowingListBox()
		{
			return mListBox.lock()->GetDraws();
		}
	};
};