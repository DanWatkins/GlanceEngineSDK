/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: ContextMenu.cpp
=================================================================================================*/

#include "../../GUI.h"

namespace ge
{
	namespace gui
	{
		ContextMenu::ContextMenu()
		{
			mShowHighlight			= false;
			mShowContext			= false;
			mIdTrack				= 1;
			mSelectedItem			= -1;
			mWaitForMBRToRelease	= false;
			SetCanFocus(false);
		}


		/*=============================================================================
		-- Initializes the ContextMenu after it has been added to the Root.
		   Automatically called from its corresponding Root::Create() method.
		=============================================================================*/
		bool ContextMenu::_PostInit()
		{
			mImgBackground.Init(GetWidth(), GetHeight(), GetWindow(), true);
			((FrameImage*)GetRoot()->GetAssetManager()->GetAsset("contextmenu_background"))->PassImagesToSprite(&mImgBackground);
			mImgHighlight.Init(GetWindow(), ((Image*)GetRoot()->GetAssetManager()->GetAsset("highlight.png"))->GetImage());

			return true;
		}


		void ContextMenu::_Update()
		{
			//should the menu be hidden?
			if (!GetHasFocus()  &&  !_GetScheduledToBringToFocus())
				SetShowContext(false);

			if (mWaitForMBRToRelease)
				mWaitForMBRToRelease = GetWindow()->GetInput()->GetButtonState(GMB_Right);

			//was the right click region clicked?
			if (GetWindow()->GetInput()->GetButtonState(GMB_Right) && !mWaitForMBRToRelease)
			{
				Rect<int> absoluteRightClickRegion;

				Vector2D<int> parentPos;

				if (!GetParent().expired())
					parentPos = GetParent().lock()->GetScreenPos();

				absoluteRightClickRegion.Set(mRightClickRegion.GetLeft()+parentPos.x, mRightClickRegion.GetTop()+parentPos.y,
												mRightClickRegion.GetWidth(), mRightClickRegion.GetHeight());

				Vector2D<int> mousePos = GetWindow()->GetInput()->GetMousePos();

				if (absoluteRightClickRegion.Intersect(mousePos))
				{
					SetShowContext(true);
					_ScheduleToBringToFocus();
					SetRelPos(mousePos - parentPos);
					mWaitForMBRToRelease = true;
				}
			}
		}


		/*=============================================================================
		-- Draws the button depending on the state and the caption overlay.
		=============================================================================*/
		void ContextMenu::_Draw()
		{
			if (mShowContext && mChildren.size() > 0)
			{
				mImgBackground.Draw();

				if (mShowHighlight)
				{
					mImgHighlight.Draw();
					mShowHighlight = false;
				}

				std::vector< SharedPtr<Element> >::iterator iter = mChildren.begin();
				while (iter != mChildren.end())
				{
					GetRoot()->GetDefaultText().lock()->SetText( DynamicPtrCast<ContextMenu>((*iter))->GetCaption() );
					GetRoot()->GetDefaultText().lock()->SetScreenPos( Vector2D<int>(GUI_MENU_TEXT_PADH+GetScreenPos().x, (*iter)->GetScreenPos().y+1) ); //TODO get better number for 12 and define it
					GetRoot()->GetDefaultText().lock()->_Draw();
					iter++;
				}

				GetRoot()->GetDefaultText().lock()->SetText("");
			}
		}


		/*=============================================================================
		-- Message handling.
		=============================================================================*/
		void ContextMenu::_SendMessage(Message message)
		{
			switch (message)
			{
				case HAS_FOCUS:
				{
					SetShowContext(true);
					break;
				}

				case CURSOR_OVER:
				{
					Vector2D<int> mousePos(GetWindow()->GetInput()->GetMouseX(), GetWindow()->GetInput()->GetMouseY());
					int item = GetItemAtPoint(mousePos);
					if (item >= 0)
					{
						mImgHighlight.SetPos(GUI_MENU_BORDER_PAD+GetScreenPos().x, GetItem(item).lock()->GetScreenPos().y);
						mShowHighlight = true;
					}
					break;
				}

				case MBL_DOWN_OVER:
				{
					Vector2D<int> mousePos;
					mousePos.Set(GetWindow()->GetInput()->GetMouseX(), GetWindow()->GetInput()->GetMouseY());
					int item = GetItemAtPoint(mousePos);
					if (item >= 0)
					{
						mSelectedItem = item;
						SetShowContext(false);
					}
					break;
				}
			}
		}


		/*=============================================================================
		-- 
		=============================================================================*/
		int ContextMenu::GetItemAtPoint(Vector2D<int> point)
		{
			int n=0;
			std::vector< SharedPtr<Element> >::iterator iter = mChildren.begin();
			while (iter != mChildren.end())
			{
				Rect<int> item;
				item.Set(GUI_MENU_BORDER_PAD+GetScreenPos().x, (*iter)->GetScreenPos().y, GetItemWidth(), GetItemHeight());
				if ( item.Intersect(point) )
					return n;

				n++;
				iter++;
			}

			return -1;
		}


		/*=============================================================================
		-- Updates the position and size of each asset.
		=============================================================================*/
		void ContextMenu::_UpdateAssets()
		{
			if (mShowContext)
			{
				mImgBackground.SetPos(GetScreenPos());
				mImgBackground.SetSize(GetWidth(), GetHeight());
				mImgHighlight.SetSize(GetWidth()-GUI_MENU_BORDER_PAD*2, GetItemHeight());

				//auto-size to the items
				int count = GetItemCount();

				unsigned height = count*GetItemHeight()+GUI_MENU_BORDER_PAD*2;

				//find the longest caption
				unsigned maxLength = 0;
				std::vector< SharedPtr<Element> >::iterator iter = mChildren.begin();
				while (iter != mChildren.end())
				{
					WeakPtr<ContextMenu> contextMenu = DynamicPtrCast<ContextMenu>(*iter);
					unsigned length = GetRoot()->GetDefaultFont()->GetTextLengthPx(contextMenu.lock()->GetCaption());
					if (maxLength < length)
						maxLength = length;

					iter++;
				}

				//add necessary padding to the length
				maxLength += GUI_MENU_BORDER_PAD*2;

				if (maxLength < GUI_MENU_MIN_WIDTH)
					maxLength = GUI_MENU_MIN_WIDTH;


				//stop infinite recursion
				if (maxLength != GetWidth()  ||  height != GetHeight())
					_SetSize(maxLength, height);
			}
		}


		void ContextMenu::SetShowContext(bool showContext)
		{
			bool oldShowContext = mShowContext;
			mShowContext = showContext;

			if (showContext == true)
				_SetSize(mSavedSize.x, mSavedSize.y);
			else if (oldShowContext && !showContext)
			{
				//save and then shrink the size to 0 by 0 so the Root doesn't give it focus when the area it occupies is selected
				mSavedSize.Set(GetWidth(), GetHeight());
				_SetSize(0, 0);

				//let the host menu bar know it is being dismissed
				if (!mHostMenuBar.expired())
					mHostMenuBar.lock()->_SelectedMenuClosed( WeakPtr<ContextMenu>( DynamicPtrCast<ContextMenu>(_GetAsWeakPtr().lock()) ) );
			}
		}


		/*=============================================================================
		-- 
		=============================================================================*/
		WeakPtr<ContextMenu> ContextMenu::GetItem(int n)
		{
			std::vector< SharedPtr<Element> >::iterator iter = mChildren.begin();
			for (int m=0; iter != mChildren.end(); iter++)
			{
				if (m == n)
					return DynamicPtrCast<ContextMenu>(*iter);

				m++;
			}

			return WeakPtr<ContextMenu>();
		}


		/*=============================================================================
		-- 
		=============================================================================*/
		int ContextMenu::GetItemCount()
		{
			int count = 0;

			for (unsigned n=0; n<mChildren.size(); n++)
			{
				if (!GetItem(n).expired())
					count++;
			}

			return count;
		}


		/*=============================================================================
		-- Adds a new item to the context menu
		=============================================================================*/
		WeakPtr<ContextMenu> ContextMenu::AddItem(String name)
		{
			WeakPtr<ContextMenu> newMenu;

			//the first item needs a larger vertical offset
			if (mChildren.size() == 0)
				newMenu = GetRoot()->CreateContextMenu(-mIdTrack, _GetAsWeakPtr(), Vector2D<int>(GetWidth(), GUI_MENU_BORDER_PAD ), 0, 0, name);
			else
				newMenu = GetRoot()->CreateContextMenu(-mIdTrack, _GetAsWeakPtr(), Vector2D<int>(GetWidth(), GetItemCount()*GetItemHeight() + GUI_MENU_BORDER_PAD), 0, 0, name);

			_UpdateAssets();


			mIdTrack++;
			return newMenu;
		}


		/*=============================================================================
		-- 
		=============================================================================*/
		int ContextMenu::GetItemWidth()
		{
			return GetWidth()-GUI_MENU_BORDER_PAD*2;
		}
	

		/*=============================================================================
		-- 
		=============================================================================*/
		int ContextMenu::GetItemHeight()
		{
			return GetRoot()->GetDefaultFont()->GetCharHeightPx()+GUI_MENU_PADV*2;
		}


		String ContextMenu::GetSelectedCaption()
		{
			WeakPtr<ContextMenu> selected = GetItem(mSelectedItem);
			if (!selected.expired())
				return selected.lock()->GetCaption();

			return "";
		}
	};
};