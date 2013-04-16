/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: ProgressBar.h
=================================================================================================*/

#include "../../GUI.h"

#ifndef _ge_PROGRESSBAR_H
#define _ge_PROGRESSBAR_H

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- Displays progress graphically.
		=============================================================================*/
		class ProgressBar : public Element
		{
		private:
			Sprite mImgBar;					//the bar itself
			SpriteFrame mImgBackground;		//background of the bar
			WeakPtr<Text> mText;			//text displayed in the middle (usually shows the percent, but can be customized)
			double mPercent;				//0.0 to 100.0

		protected:
			bool _PostInit();
			void _Draw();
			void _SendMessage() {}
			void _UpdateAssets();


		public:
			friend class Root;

			ProgressBar();
			~ProgressBar() {}

			//accessor and mutator
			void SetText(String caption);
			void SetPercent(double percent, int precision=1);
			double GetPercent() { return mPercent; }
		};
	};
};

#endif

