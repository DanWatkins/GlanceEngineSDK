/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: ContextMenu.h
=================================================================================================*/

#include "../../GUI.h"

#ifndef _ge_CONTEXTMENU_H
#define _ge_CONTEXTMENU_H

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- ContextMenu element to pop up a menu for making choices. Items in the menu
		   are just ContextMenus.
		=============================================================================*/
		class ContextMenu : public Element
		{
		private:
			SpriteFrame mImgBackground;				//background image,
			Sprite mImgHighlight;					//highlight image that goes over the selection
			Sprite mImgBreak, mImgDisclosure;		//horizontal break,dDisclosure arrow indicatior
			String mCaption;						//used for the titles of the items.
			bool mShowContext;						//true if it should show itself
			bool mShowHighlight;					//true if it should show the highlight
			int mIdTrack;							//used to keep track of the ids available
			int mSelectedItem;						//id of the item currently selected
			WeakPtr<MenuBar> mHostMenuBar;			//only used when this menu is docked in a menu bar. should call _SelectedMenuClosed(this) on the menu bar when this closes
			Vector2D<int> mSavedSize;				//since the ContextMenu becomes 0 by 0 in size when hidden, the size needs to be remembered for when it is shown
			Rect<int> mRightClickRegion;			//the area that the context menu will occupy relative to the parent when it is hidden. any right clicks here will unhide it
			bool mWaitForMBRToRelease;				//if true, the context menu wont pop up when the right click region is clicked until this is set to false in the update

		protected:
			bool _PostInit();
			void _Update();
			void _Draw();
			void _SendMessage(Message message);
			void _UpdateAssets();
			void _SetHostMenuBar(WeakPtr<MenuBar> hostMenuBar) { mHostMenuBar = hostMenuBar; }

		public:
			friend class Root;
			friend class MenuBar;

			ContextMenu();

			void SetCaption(String caption) { mCaption = caption; }
			String GetCaption() { return mCaption; }
			void SetShowContext(bool showContext);
			WeakPtr<ContextMenu> GetItem(int n);
			WeakPtr<ContextMenu> GetSelectedItem() { return GetItem(mSelectedItem); }
			String GetSelectedCaption();
			void ClearSelectedCaption() { mSelectedItem = -1; }
			int GetItemCount();
			WeakPtr<ContextMenu> AddItem(String name);
			int GetItemWidth();
			int GetItemHeight();
			int GetItemAtPoint(Vector2D<int> point);
			bool GetShowContext() { return mShowContext; }
			void SetRightClickRegion(Rect<int> region) { mRightClickRegion = region; }
			Rect<int> GetRightClickRegion() { return mRightClickRegion; }
		};
	};
};


#endif

