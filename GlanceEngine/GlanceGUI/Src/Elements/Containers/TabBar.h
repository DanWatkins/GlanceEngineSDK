/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: TabBar.h
=================================================================================================*/

#include "../../GUI.h"

#ifndef _ge_TABBAR_H
#define _ge_TABBAR_H

namespace ge
{
	namespace gui
	{
		const int TABBAR_HEADER_HEIGHT = 22;
		const int TABBAR_HEADER_EXPANDED_HEIGHT = 24;

		/*=============================================================================
		-- TabBar element that docks TabPages.
		=============================================================================*/
		class TabBar : public Element
		{
		private:
			SpriteFrame mImgBackground;				//border that represents the TabBar
			SpriteFrame mImgHeader;					//header section for displaying tab pages
			SpriteFrame mImgSelectedHeader;			//selected header section

			int mIdTrack;							//incremented as pages are added. Pages are given a negative version of this number for their id
			std::vector< WeakPtr<TabPage> > mPages;	//list of pointers to pages associated with this tab bar. all pages are also children
			WeakPtr<TabPage> mActivePage;

		protected:
			bool _PostInit();
			void _Draw();
			void _UpdateAssets();
			void _SendMessage(Message message);


		public:
			friend class Root;

			TabBar() { SetCanFocus(false); }

			WeakPtr<TabPage> AddPage(String caption);
			WeakPtr<TabPage> GetActivePage() { return mActivePage; }
		};
	};
};

#endif

