/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: ScrollBar.cpp
  Created: 1/23/13 by Dan Watkins
=================================================================================================*/

#include "../../GUI.h"

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- Constructor for ScrollBar.
		=============================================================================*/
		ScrollBar::ScrollBar()
		{
			mEnableSliding		= true;
			mScrollPosition		= 0.0;
		}


		/*=============================================================================
		-- Initializes the ScrollBar after it has been added to the Root. Automatically
		   called from its corresponding Root::Create() method.
		=============================================================================*/
		bool ScrollBar::_PostInit()
		{
			mKnobButton = GetRoot()->CreateButtonImage(-31, _GetAsWeakPtr(), Vector2D<int>(), 0, 0, "scroll_knob.png");
			mKnobButton.lock()->SetSize(SCROLL_BAR_BUTTON_SIZE, SCROLL_BAR_BUTTON_SIZE);

			mKnobDrager = GetRoot()->CreateDrager(-32, mKnobButton, Vector2D<int>(), 0, 0, false, false);
			mKnobDrager.lock()->SetUseHorizontalBounding(true);
			mKnobDrager.lock()->SetUseVerticalBounding(true);
			mKnobDrager.lock()->SetSize(SCROLL_BAR_BUTTON_SIZE, SCROLL_BAR_BUTTON_SIZE);

			mButtonDecrease = GetRoot()->CreateButtonImage(-33, _GetAsWeakPtr(), Vector2D<int>(), SCROLL_BAR_BUTTON_SIZE, SCROLL_BAR_BUTTON_SIZE, "scroll_button_decrease.png");
			mButtonIncrease = GetRoot()->CreateButtonImage(-34, _GetAsWeakPtr(), Vector2D<int>(), SCROLL_BAR_BUTTON_SIZE, SCROLL_BAR_BUTTON_SIZE, "scroll_button_increase.png");

			return true;
		}


		/*=============================================================================
		-- Updates the ScrollBar.
		=============================================================================*/
		void ScrollBar::_Update()
		{
			//update the scroll position
			int knobLength = (mSpanDirection == SpanDirection::HORIZONTAL) ? mKnobButton.lock()->GetWidth() : mKnobButton.lock()->GetHeight();
			int knobMin = SCROLL_BAR_BUTTON_SIZE;
			int knobMax = ((mSpanDirection == SpanDirection::HORIZONTAL) ? GetWidth() : GetHeight()) - SCROLL_BAR_BUTTON_SIZE - knobLength;
			int innerRange = knobMax - knobMin;

			Vector2D<int> knobRelPos = mKnobButton.lock()->GetRelPos();
			int knobOffset = ((mSpanDirection == SpanDirection::HORIZONTAL) ? knobRelPos.x : knobRelPos.y) - SCROLL_BAR_BUTTON_SIZE;

			mScrollPosition = DivideD((double)knobOffset,(double)innerRange);

			//check if either increase/decrease button were pressed
			if (mAdjustTimer.GetTimeAndEnd() > SCROLL_BAR_ADJUST_DELAY)
			{
				if (mButtonDecrease.lock()->GetState() == State::DOWN)
					SetScrollPosition(GetScrollPosition() + 0.01);
				else if (mButtonIncrease.lock()->GetState() == State::DOWN)
					SetScrollPosition(GetScrollPosition() - 0.01);

				mAdjustTimer.Start();
			}
		}

		
		/*=============================================================================
		-- Updates the position and size of every element.
		=============================================================================*/
		void ScrollBar::_UpdateAssets()
		{
			if (!mButtonDecrease.expired()  &&  !mButtonIncrease.expired()  &&  !mKnobButton.expired())
			{
				if (mSpanDirection == SpanDirection::VERTICAL)
				{
					mButtonIncrease.lock()->SetRelPos(Vector2D<int>(0,0));
					mButtonDecrease.lock()->SetRelPos(Vector2D<int>(0, GetHeight()-SCROLL_BAR_BUTTON_SIZE));

					mKnobDrager.lock()->SetDragsHorizontal(false);
					mKnobDrager.lock()->SetDragsVertical(true);
					mKnobDrager.lock()->SetParentRelPosLowerBound(Vector2D<int>(0, SCROLL_BAR_BUTTON_SIZE));
					mKnobDrager.lock()->SetParentRelPosUpperBound(Vector2D<int>(0, GetHeight()-SCROLL_BAR_BUTTON_SIZE-mKnobButton.lock()->GetHeight()));
				}
				else
				{
					mButtonIncrease.lock()->SetRelPos(Vector2D<int>(0,0));
					mButtonDecrease.lock()->SetRelPos(Vector2D<int>(GetWidth()-SCROLL_BAR_BUTTON_SIZE, 0));

					mKnobDrager.lock()->SetDragsHorizontal(true);
					mKnobDrager.lock()->SetDragsVertical(false);
					mKnobDrager.lock()->SetParentRelPosLowerBound(Vector2D<int>(SCROLL_BAR_BUTTON_SIZE, 0));
					mKnobDrager.lock()->SetParentRelPosUpperBound(Vector2D<int>(GetWidth()-SCROLL_BAR_BUTTON_SIZE-mKnobButton.lock()->GetWidth(), 0));
				}
			}
		}


		/*=============================================================================
		-- Properly adjusts the ScrollBar to span vertically or horizontally.
		=============================================================================*/
		void ScrollBar::SetSpanDirection(SpanDirection spanDirection)
		{
			//is this span direction differnt than the current?
			if (spanDirection != mSpanDirection)
			{
				mSpanDirection = spanDirection;
				_UpdateAssets();
			}

		}


		/*=============================================================================
		-- Snaps the scroll knob to a specific position. The scroll position is a value
		   between 0.0 and 1.0, where 0.0 represents the knob being the farthest left
		   or up. 1.0 represensts the knob being all the way to the right or down.
		=============================================================================*/
		void ScrollBar::SetScrollPosition(double scrollPosition)
		{
			mScrollPosition = scrollPosition;

			if (mScrollPosition < 0.0)
				mScrollPosition = 0.0;
			else if (mScrollPosition > 1.0)
				mScrollPosition = 1.0;

			int knobLength = (mSpanDirection == SpanDirection::HORIZONTAL) ? mKnobButton.lock()->GetWidth() : mKnobButton.lock()->GetHeight();
			int knobMin = SCROLL_BAR_BUTTON_SIZE;
			int knobMax = ((mSpanDirection == SpanDirection::HORIZONTAL) ? GetWidth() : GetHeight()) - SCROLL_BAR_BUTTON_SIZE - knobLength;
			int innerRange = knobMax - knobMin;

			int knobOffset = innerRange*scrollPosition + SCROLL_BAR_BUTTON_SIZE;

			if (mSpanDirection == SpanDirection::HORIZONTAL)
				mKnobButton.lock()->SetRelPos(Vector2D<int>(knobOffset,0));
			else
				mKnobButton.lock()->SetRelPos(Vector2D<int>(0,knobOffset));
		}


		/*=============================================================================
		-- Properly sizes the knob button and drager.
		=============================================================================*/
		void ScrollBar::_SetKnobSize(int width, int height)
		{
			mKnobButton.lock()->SetSize(width, height);
			mKnobDrager.lock()->SetSize(width, height);

			_UpdateAssets();
		}


		/*=============================================================================
		-- Sets the virtual scroll size and adjusts the knob size to represent it.
		=============================================================================*/
		void ScrollBar::SetVirtualScrollSize(int virtualScrollSize)
		{
			if (mVirtualScrollSize != virtualScrollSize)
			{
				mVirtualScrollSize = virtualScrollSize;

				//calculate the appropriate height of the knob and visible size (length)
				int maxSize, visibleSize, knobSize;
				if (mSpanDirection == SpanDirection::HORIZONTAL)
				{
					maxSize = GetWidth()-SCROLL_BAR_BUTTON_SIZE*2;
					visibleSize = GetWidth();

					if (visibleSize > mVirtualScrollSize) knobSize = maxSize;
					else knobSize = maxSize * visibleSize / mVirtualScrollSize;

					if (knobSize < SCROLL_BAR_MIN_KNOB_SIZE) knobSize = SCROLL_BAR_MIN_KNOB_SIZE;
					_SetKnobSize(knobSize, mKnobButton.lock()->GetHeight());
				}
				else  //if vertical span direction
				{
					maxSize = GetHeight()-SCROLL_BAR_BUTTON_SIZE*2;
					visibleSize = GetHeight();

					if (visibleSize > mVirtualScrollSize) knobSize = maxSize;
					else knobSize = maxSize * visibleSize / mVirtualScrollSize;

					if (knobSize < SCROLL_BAR_MIN_KNOB_SIZE) knobSize = SCROLL_BAR_MIN_KNOB_SIZE;
					_SetKnobSize(mKnobButton.lock()->GetWidth(), knobSize);
				}
			}
		}
	};
};