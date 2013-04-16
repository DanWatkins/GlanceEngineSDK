/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Drager.cpp
=================================================================================================*/

#include "../../GUI.h"

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- Constructor for Drager.
		=============================================================================*/
		Drager::Drager()
		{
			mStrayClick				= false;
			mFirstClick				= false;
			mXOutOfBounds			= false;
			mYOutOfBounds			= false;
			mDragsHorizontal		= true;
			mDragsVertical			= true;
			mOldPos.Set(0, 0);
			mUseHorizontalBounding	= false;
			mUseVerticalBounding	= false;
			SetSolid(false); //this way, the drager wont come into focus and cover other elements
			SetCanFocus(false);
		}


		/*=============================================================================
		-- Checks if draging is occuring, then moves the parent accordingly.
		=============================================================================*/
		void Drager::_Update()
		{
			int mouseX = GetWindow()->GetInput()->GetMouseX();
			int mouseY = GetWindow()->GetInput()->GetMouseY();
			bool mbl_isDown = GetWindow()->GetInput()->GetButtonState(GMB_Left);

			//determine the draging state
			if (mbl_isDown && !mFirstClick)
			{
				if (GetRoot()->GetHighestElementAtPoint(Vector2D<int>(mouseX, mouseY)).lock().get() == this)
				{
					mFirstClick = true;
					mStrayClick = false;
					mOldPos.Set(mouseX, mouseY);
				}
				else
				{
					mStrayClick = true;
					mFirstClick = true;
				}
			}
			else if (!mbl_isDown)
			{
				mFirstClick = false;
				mStrayClick = false;
			}
			
			//is draging possible?
			if (mFirstClick && !mStrayClick)
			{
				//move the parent's position
				Vector2D<int> newRelPos = GetParent().lock()->GetRelPos();
				if (mDragsHorizontal)
					newRelPos.x += mouseX-mOldPos.x;
				if (mDragsVertical)
					newRelPos.y += mouseY-mOldPos.y;

				GetParent().lock()->SetRelPos(newRelPos);

				//save current mouse pos
				mOldPos.Set(mouseX, mouseY);
			}

			//bounds check the relative position of the parent
			Vector2D<int> pRelPos = GetParent().lock()->GetRelPos();//TODO make all other elements use GetParent(), not mParent directly
			Vector2D<int> newRelPos = pRelPos;

			if (mUseHorizontalBounding)
			{
				if (pRelPos.x < mParentRelPosLowerBound.x)
					newRelPos.x = mParentRelPosLowerBound.x;
				else if (pRelPos.x > mParentRelPosUpperBound.x)
					newRelPos.x = mParentRelPosUpperBound.x;
			}
			if (mUseVerticalBounding)
			{
				if (pRelPos.y < mParentRelPosLowerBound.y)
					newRelPos.y = mParentRelPosLowerBound.y;
				else if (pRelPos.y > mParentRelPosUpperBound.y)
					newRelPos.y = mParentRelPosUpperBound.y;
			}

			GetParent().lock()->SetRelPos(newRelPos);
		}
	};
};