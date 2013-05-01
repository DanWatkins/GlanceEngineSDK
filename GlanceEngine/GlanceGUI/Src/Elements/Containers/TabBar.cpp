/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: TabBar.cpp
=================================================================================================*/

#include "../../GUI.h"

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- Initializes the TabBar after it has been added to the Root. Automatically
		   called from its corresponding Root::Create() method.
		=============================================================================*/
		bool TabBar::_PostInit()
		{
			//load border sprite frame from memory
			mImgBackground.Init( GetWidth(), GetHeight(), GetRoot()->GetWindow() );
			((FrameImage*)GetRoot()->GetAssetManager()->GetAsset("tabbar_background"))->PassImagesToSprite(&mImgBackground);

			mImgHeader.Init( GetWidth(), GetHeight(), GetRoot()->GetWindow() );
			((FrameImage*)GetRoot()->GetAssetManager()->GetAsset("tabbar_header"))->PassImagesToSprite(&mImgHeader);
			mImgHeader.SetHeight(TABBAR_HEADER_HEIGHT);

			mImgSelectedHeader.Init( GetWidth(), GetHeight(), GetRoot()->GetWindow() );
			((FrameImage*)GetRoot()->GetAssetManager()->GetAsset("tabbar_selected_header"))->PassImagesToSprite(&mImgSelectedHeader);
			mImgSelectedHeader.SetHeight(TABBAR_HEADER_EXPANDED_HEIGHT);

			return true;
		}


		/*=============================================================================
		-- Draws the TabBar.
		=============================================================================*/
		void TabBar::_Draw()
		{
			mImgBackground.Draw();

			//use a SpriteFrame pointer because sometimes @mImgBackground
			//needs to be used and sometimes @mImgHeader needs to be used
			SpriteFrame *headerImage;

			//set the y-pos of both @mImgHeader and @mImgSelectedHeader
			mImgHeader.SetY(GetScreenPos().y+(TABBAR_HEADER_EXPANDED_HEIGHT-TABBAR_HEADER_HEIGHT));
			mImgSelectedHeader.SetY(GetScreenPos().y+(TABBAR_HEADER_EXPANDED_HEIGHT-TABBAR_HEADER_HEIGHT));

			//draw the appropriate header for each page
			std::vector< WeakPtr<TabPage> >::iterator iter = mPages.begin();
			int headerXPos = GetScreenPos().x;

			while (iter != mPages.end())
			{
				//use the right type of header SpriteFrame
				if ((*iter).lock() == mActivePage.lock())
					headerImage = &mImgSelectedHeader;
				else
					headerImage = &mImgHeader;

				//draw the header image and text overlay
				int headerWidth = 10+GetRoot()->GetDefaultFont()->GetTextLengthPx((*iter).lock()->GetCaption());
				headerImage->SetWidth(headerWidth);//TODO ^get a better value than 10^
				headerImage->SetX(headerXPos);
				headerImage->Draw();
				GetRoot()->GetDefaultText().lock()->SetScreenPos( Vector2D<int>(headerXPos+5, GetScreenPos().y+(TABBAR_HEADER_EXPANDED_HEIGHT-TABBAR_HEADER_HEIGHT)+5) );
				GetRoot()->GetDefaultText().lock()->SetText((*iter).lock()->GetCaption());
				GetRoot()->DrawDefaultText();

				headerXPos += headerWidth;
				iter++;
			}
		}


		/*=============================================================================
		-- Updates the scalar metrics of any assets.
		=============================================================================*/
		void TabBar::_UpdateAssets()
		{
			mImgBackground.SetPos(GetScreenPos().x, GetScreenPos().y+TABBAR_HEADER_EXPANDED_HEIGHT);
			mImgBackground.SetSize( GetWidth(), GetHeight()-TABBAR_HEADER_EXPANDED_HEIGHT );
			mImgBackground.ResetPos();
			mImgBackground.ResetSize();
		}


		/*=============================================================================
		-- Creates a new TabPage element and adds it as a child to this. A WeakPtr<>
		   to the TabPage is also stored in a list for just pages.
		=============================================================================*/
		WeakPtr<TabPage> TabBar::AddPage(String caption)
		{
			//make sure there is not already a page with the same name
			bool unique = true;
			std::vector< WeakPtr<TabPage> >::iterator iter = mPages.begin();
			while (iter != mPages.end())
			{
				if ((*iter).lock()->GetCaption() == caption)
					unique = false;

				iter++;
			}

			if (unique == false)
				gDebug.Warning(String()+"Adding a tab page with name="+caption.GetStd()+" but there is already a tab page with that name for element id="+GetId());

			mIdTrack++;
			WeakPtr<TabPage> newPage = GetRoot()->_CreateTabPage(-mIdTrack, DynamicPtrCast<TabBar>(_GetAsWeakPtr().lock()), caption);
			mPages.push_back(newPage);

			//set every other page to not draw except this one
			iter = mPages.begin();
			while (iter != mPages.end())
			{
				(*iter).lock()->SetDraws(false);
				iter++;
			}

			newPage.lock()->SetDraws(true);
			mActivePage = newPage;

			return newPage;
		}


		/*=============================================================================
		-- 
		=============================================================================*/
		void TabBar::_SendMessage(Message message)
		{
			switch (message)
			{
				case MBL_DOWN_OVER:
				{
					//find the tab header under the mouse (if any)
					int headerXPos = GetScreenPos().x;
					Vector2D<int> mousePos = GetWindow()->GetInput()->GetMousePos();
					std::vector< WeakPtr<TabPage> >::iterator iter = mPages.begin();

					while (iter != mPages.end())
					{
						int headerWidth = 10+GetRoot()->GetDefaultFont()->GetTextLengthPx((*iter).lock()->GetCaption());
						Rect<int> header(headerXPos, GetScreenPos().y, headerWidth, TABBAR_HEADER_HEIGHT);

						//is the mouse intersecting a header's rectangle?
						if (header.Intersect(mousePos))
						{
							WeakPtr<TabPage> oldActivePage = mActivePage;
							mActivePage = *iter;

							//set every other page to not draw
							std::vector< WeakPtr<TabPage> >::iterator iter = mPages.begin();
							while (iter != mPages.end())
							{
								(*iter).lock()->SetDraws(false);
								if ((*iter).lock().get() == oldActivePage.lock().get())
									_SendElementMessageToListeners(ElementEvent::PAGE_DISAPEARED, ToString((*iter).lock()->GetId()));
								iter++;
							}

							mActivePage.lock()->SetDraws(true);
							_SendElementMessageToListeners(ElementEvent::PAGE_APPEARED, ToString(mActivePage.lock()->GetId()));

							break;
						}

						//advance to next header
						headerXPos += headerWidth;
						iter++;
					}

					break;
				}
			}
		}
	};
};