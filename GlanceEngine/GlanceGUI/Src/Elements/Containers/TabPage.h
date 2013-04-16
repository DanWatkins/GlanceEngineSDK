/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: TabPage.h
=================================================================================================*/

#include "../../GUI.h"

#ifndef _ge_TABPAGE_H
#define _ge_TABPAGE_H

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- TabPage element that groups elements with an optional caption.
		=============================================================================*/
		class TabPage : public Element
		{
		private:
			String mCaption;			//caption in the upper left corner (auto aligned)

		public:
			friend class TabBar;

			//accessors and mutators
			String GetCaption() { return mCaption; }
			void SetCaption(String caption) { mCaption = caption; }
		};
	};
};

#endif

