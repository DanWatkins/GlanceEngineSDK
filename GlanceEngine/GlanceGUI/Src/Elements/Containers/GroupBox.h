/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: GroupBox.h
=================================================================================================*/

#include "../../GUI.h"

#ifndef _ge_GROUPBOX_H
#define _ge_GROUPBOX_H

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- GroupBox element that groups elements with an optional caption.
		=============================================================================*/
		class GroupBox : public Element
		{
		private:
			SpriteFrame mImgBorder;		//border that represents the groupbox
			String mCaption;			//caption in the upper left corner (auto aligned)

		protected:
			bool _PostInit();
			void _Draw();
			void _UpdateAssets();


		public:
			friend class Root;

			GroupBox();
			~GroupBox() {}

			//accessors and mutators
			void SetCaption(String caption) { mCaption = caption; }
		};
	};
};

#endif

