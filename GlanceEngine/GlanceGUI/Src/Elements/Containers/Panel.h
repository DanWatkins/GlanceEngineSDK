/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Panel.h
  Created: 4/21/2013 @ 19:38 UTC-6
=================================================================================================*/

#include "../../GUI.h"

#ifndef _ge_PANEL_H
#define _ge_PANEL_H

namespace ge
{
	namespace gui
	{
		class Panel : public Element
		{
		private:
			SpriteFrame mImgPanel;		//border that represents the groupbox

		protected:
			bool _PostInit();
			void _Draw();
			void _UpdateAssets();


		public:
			friend class Root;

			Panel();		
		};
	};
};

#endif

