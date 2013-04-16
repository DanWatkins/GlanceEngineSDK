/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Slider.h
=================================================================================================*/

#include "../../GUI.h"

#ifndef _ge_SLIDER_H
#define _ge_SLIDER_H

#define KNOB_WIDTH 6
#define KNOB_HEIGHT 16
#define TRACK_HEIGHT 2

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- Slider to allow regulating to a range of values.
		=============================================================================*/
		class Slider : public Element
		{
		private:
			Sprite mImgTrack;
			WeakPtr<ButtonCaption> mKnobButton;
			WeakPtr<Drager> mKnobDrager;

		protected:
			bool _PostInit();
			void _Draw();
			void _UpdateAssets();


		public:
			friend class Root;

			double GetPercentOffset() { return (double)(mKnobButton.lock()->GetRelPos().x+(KNOB_WIDTH/2)) / (double)GetWidth(); }
		};
	};
};


#endif

