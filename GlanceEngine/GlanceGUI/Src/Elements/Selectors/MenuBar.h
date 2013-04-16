/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: MenuBar.h
=================================================================================================*/

#include "../../GUI.h"

#ifndef _ge_MENUBAR_H
#define _ge_MENUBAR_H

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- 
		=============================================================================*/
		class MenuBar : public Element
		{
		private:
			SpriteFrame mImgBackground;
			Sprite mImgHighlight;
			int mIdTrack;
			WeakPtr<ContextMenu> mSelectedMenu;
			bool mShowHighlight;
			short mMBLWasPressedTicks;										//set to 2 when the MBL_DOWN_OVER message is sent. decrements every frame.
			bool mWaitForMBLToReleaseToStopSkippingMouseMessages;			//if true, the context menu that was picked in the bar will not be brought to fo

		protected:
			bool _PostInit();
			void _Update();
			void _Draw();
			void _SendMessage(Message message);
			void _UpdateAssets();
			unsigned _GetWidthForText(String text);
			WeakPtr<ContextMenu> _GetMenuAtPoint(Vector2D<int> point);
			void _SelectedMenuClosed(WeakPtr<ContextMenu> contextMenuClosed);

		public:
			friend class Root;
			friend class ContextMenu;

			MenuBar();
			WeakPtr<ContextMenu> GetMenu(String name);
			WeakPtr<ContextMenu> GetSelectedMenu() { return mSelectedMenu; }
			WeakPtr<ContextMenu> AddMenu(String name);
		};
	};
};


#endif

