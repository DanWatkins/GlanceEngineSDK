/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: ScrollBar.h
=================================================================================================*/

#include "../../GUI.h"

#ifndef _ge_SCROLL_BAR_H
#define _ge_SCROLL_BAR_H

namespace ge
{
	namespace gui
	{
		enum SpanDirection
		{
			VERTICAL = 0,
			HORIZONTAL = 1
		};

		//TODO move to better location
		const int SCROLL_BAR_BUTTON_SIZE = 16;
		const int SCROLL_BAR_ADJUST_DELAY = 20;	//milliseconds between adjustments while holding down
		const int SCROLL_BAR_MIN_KNOB_SIZE = 8;

		/*=============================================================================
		-- A scroll bar control that can span vertically or horizontally.
		=============================================================================*/
		class ScrollBar : public Element
		{
		private:
			SpanDirection mSpanDirection;
			WeakPtr<ButtonImage> mKnobButton;
			WeakPtr<ButtonImage> mButtonDecrease, mButtonIncrease;
			WeakPtr<Drager> mKnobDrager;
			bool mEnableSliding;
			double mScrollPosition;
			Timer mAdjustTimer;
			int mVirtualScrollSize;		//height if vertical span, width if horizontal span

		protected:
			bool _PostInit();
			void _Update();
			void _UpdateAssets();

			void _SetKnobSize(int width, int height);

		public:
			friend class Root;
			ScrollBar();

			void SetSpanDirection(SpanDirection spanDirection);
			SpanDirection GetSpanDirection() { return mSpanDirection; }
			void SetScrollPosition(double scrollPosition);  //TODO rename to just Pos
			double GetScrollPosition() { return mScrollPosition; }
			void SetVirtualScrollSize(int virtualScrollSize);
			int GetVirtualScrollSize() { return mVirtualScrollSize; }
		};
	};
};


#endif

