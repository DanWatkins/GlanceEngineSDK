/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: MenuBar.cpp
=================================================================================================*/

#include "../../GUI.h"

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- Constructor for MenuBar.
		=============================================================================*/
		MenuBar::MenuBar()
		{
			mIdTrack										= 1;
			mShowHighlight									= false;
			mMBLWasPressedTicks								= 0;
			mWaitForMBLToReleaseToStopSkippingMouseMessages	= false;
			SetCanFocus(false);
		}


		/*=============================================================================
		-- Initializes the MenuBar after it has been added to the Root.
		   Automatically called from its corresponding Root::Create() method.
		=============================================================================*/
		bool MenuBar::_PostInit()
		{
			mImgBackground.Init(GetWidth(), GetHeight(), GetWindow(), true);
			((FrameImage*)GetRoot()->GetAssetManager()->GetAsset("menubar_background"))->PassImagesToSprite(&mImgBackground);
			mImgHighlight.Init(GetWindow(), ((Image*)GetRoot()->GetAssetManager()->GetAsset("highlight.png"))->GetImage());

			return true;
		}


		/*=============================================================================
		-- 
		=============================================================================*/
		void MenuBar::_Update()
		{
			//need to keep skipping mouse messages or else the menu bar will keep getting focus and the context menu that
			//was BroughtToFocus() will keep losing focus because it is not under the mouse position when clicked and the user
			//will not instantly bring the mouse button up (accounting for human error here)
			if (mWaitForMBLToReleaseToStopSkippingMouseMessages)
			{
				if (GetWindow()->GetInput()->GetButtonState(GMB_Left))
					GetRoot()->SetSkipMouseMessagesThisUpdate(true);
				else
					mWaitForMBLToReleaseToStopSkippingMouseMessages = false;
			}


			if (mMBLWasPressedTicks == 1)
			{
				mMBLWasPressedTicks = false;
				
				Vector2D<int> mousePos(GetWindow()->GetInput()->GetMouseX(), GetWindow()->GetInput()->GetMouseY());
				WeakPtr<ContextMenu> contextMenu = _GetMenuAtPoint(mousePos);
				if (!contextMenu.expired())
				{
					//only show if there are items in the context menu
					if (contextMenu.lock()->GetChildCount() > 0)
					{
						//hide all menus
						std::vector< SharedPtr<Element> >::iterator iter = mChildren.begin();
						while (iter != mChildren.end())
						{
							WeakPtr<ContextMenu> contextMenuIter = DynamicPtrCast<ContextMenu>(*iter);
							contextMenuIter.lock()->SetShowContext(false);

							iter++;
						}


						if (!contextMenu.expired())
						{
							//let the selected menu know it is selected
							mSelectedMenu = contextMenu;
							contextMenu.lock()->_ScheduleToBringToFocus();
							contextMenu.lock()->SetShowContext(true);
							mShowHighlight = false;

							mWaitForMBLToReleaseToStopSkippingMouseMessages = true;
							GetRoot()->SetSkipMouseMessagesThisUpdate(true);
						}
						//reset the selected menu to nothing because there is not one
						else
							mSelectedMenu.reset();
					}
				}

				mMBLWasPressedTicks = 0;
			}
			else if (mMBLWasPressedTicks == 2)
				mMBLWasPressedTicks = 1;
		}


		/*=============================================================================
		-- Draws the button depending on the state and the caption overlay.
		=============================================================================*/
		void MenuBar::_Draw()
		{
			mImgBackground.Draw();
			
			if (mShowHighlight)
			{
				mImgHighlight.Draw();
				mShowHighlight = false;
			}

			WeakPtr<Text> text = GetRoot()->GetDefaultText();
			std::vector< SharedPtr<Element> >::iterator iter = mChildren.begin();
			while (iter != mChildren.end())
			{
				WeakPtr<ContextMenu> contextMenu = DynamicPtrCast<ContextMenu>((*iter));
				text.lock()->SetText( contextMenu.lock()->GetCaption() );
				text.lock()->SetScreenPos(Vector2D<int>(contextMenu.lock()->GetScreenPos().x+GUI_MENU_TEXT_PADH, GetScreenPos().y+4)); //TODO define 4
				text.lock()->_Draw();

				iter++;
			}
		}


		/*=============================================================================
		-- Message handling.
		=============================================================================*/
		void MenuBar::_SendMessage(Message message)
		{
			switch (message)
			{
				case CURSOR_OVER:
				{
					Vector2D<int> mousePos(GetWindow()->GetInput()->GetMouseX(), GetWindow()->GetInput()->GetMouseY());
					WeakPtr<ContextMenu> contextMenu = _GetMenuAtPoint(mousePos);
					if (!contextMenu.expired())
					{
						//don't highlight if the selected menu is open
						if (mSelectedMenu.expired())
						{
							mImgHighlight.SetPos(contextMenu.lock()->GetScreenPos().x, GetScreenPos().y+GUI_MENU_BORDER_PAD);

							mImgHighlight.SetSize(_GetWidthForText(contextMenu.lock()->GetCaption()), GetHeight()-GUI_MENU_BORDER_PAD*2);


							mShowHighlight = true;
						}
					}
					break;
				}

				case MBL_DOWN_OVER:
				{
					Vector2D<int> mousePos(GetWindow()->GetInput()->GetMouseX(), GetWindow()->GetInput()->GetMouseY());
					WeakPtr<ContextMenu> contextMenu = _GetMenuAtPoint(mousePos);

					//if the menu under the mouse pointer is already the selected one, close it
					if (contextMenu.lock() == mSelectedMenu.lock()  &&  !mSelectedMenu.expired())
						mSelectedMenu.reset();
					//otherwise, set the ticks to 2 and it will be selected after the next update
					else
						mMBLWasPressedTicks = 2;

					break;
				}
			}
		}


		/*=============================================================================
		-- 
		=============================================================================*/
		void MenuBar::_UpdateAssets()
		{
			mImgBackground.SetSize(GetWidth(), GetHeight());
			mImgBackground.SetPos(GetScreenPos());
		}


		/*=============================================================================
		-- 
		=============================================================================*/
		WeakPtr<ContextMenu> MenuBar::_GetMenuAtPoint(Vector2D<int> point)
		{
			std::vector< SharedPtr<Element> >::iterator iter = mChildren.begin();
			while (iter != mChildren.end())
			{
				Rect<int> menu;
				WeakPtr<ContextMenu> contextMenu = DynamicPtrCast<ContextMenu>(*iter);
				menu.Set((*iter)->GetScreenPos().x, GUI_MENU_BORDER_PAD+GetScreenPos().y, _GetWidthForText(contextMenu.lock()->GetCaption()), GetHeight()-GUI_MENU_BORDER_PAD*2);
				if ( menu.Intersect(point) )
					return WeakPtr<ContextMenu>( DynamicPtrCast<ContextMenu>(*iter) );

				iter++;
			}

			return WeakPtr<ContextMenu>();
		}


		/*=============================================================================
		-- 
		=============================================================================*/
		WeakPtr<ContextMenu> MenuBar::GetMenu(String name)
		{
			std::vector< SharedPtr<Element> >::iterator iter = mChildren.begin();
			while (iter != mChildren.end())
			{
				if (name == DynamicPtrCast<ContextMenu>(*iter)->GetCaption())
					return DynamicPtrCast<ContextMenu>(*iter);

				iter++;
			}

			return WeakPtr<ContextMenu>();
		}


		/*=============================================================================
		-- Adds a new menu with the name to the right of the last menu added. The menu
		   added is simply a ContextMenu.
		=============================================================================*/
		WeakPtr<ContextMenu> MenuBar::AddMenu(String name)
		{
			//get the start x pos for the new menu
			int startX = GUI_MENU_BORDER_PAD;
			std::vector< SharedPtr<Element> >::iterator iter = mChildren.begin();
			while (iter != mChildren.end())
			{
				WeakPtr<ContextMenu> contextMenu = DynamicPtrCast<ContextMenu>(*iter);
				startX += _GetWidthForText(contextMenu.lock()->GetCaption());
				iter++;
			}

			//actually create the menu
			WeakPtr<ContextMenu> contextMenu = GetRoot()->CreateContextMenu(-mIdTrack, _GetAsWeakPtr(), Vector2D<int>(startX, GetHeight()), 0, 0, name);
			contextMenu.lock()->_SetHostMenuBar( WeakPtr<MenuBar>( DynamicPtrCast<MenuBar>(_GetAsWeakPtr().lock()) ) );
			mIdTrack++;

			return contextMenu;
		}


		/*=============================================================================
		-- Returns the width of the text including propper padding. This width value
		   is used for spacing the menu headers in the MenuBar.
		=============================================================================*/
		unsigned MenuBar::_GetWidthForText(String text)
		{
			return GetRoot()->GetDefaultFont()->GetTextLengthPx(text) + GUI_MENU_TEXT_PADH*2;
		}


		/*=============================================================================
		-- Called by the selected context menu when it has been closed based on its
		   own criteria.
		=============================================================================*/
		void MenuBar::_SelectedMenuClosed(WeakPtr<ContextMenu> contextMenuClosed)
		{
			if (mSelectedMenu.lock() == contextMenuClosed.lock())
				mSelectedMenu.reset();
		}
	};
};