/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Drager.h
=================================================================================================*/

#include "../../GUI.h"

#ifndef _ge_DRAGER_H
#define _ge_DRAGER_H

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- Drager to house elements that can be moved around together.
		=============================================================================*/
		class Drager : public Element
		{
		private:
			bool mStrayClick, mFirstClick;
			bool mXOutOfBounds, mYOutOfBounds;
			bool mDragsHorizontal, mDragsVertical;
			Vector2D<int> mOldPos;

			bool mUseHorizontalBounding, mUseVerticalBounding;
			Vector2D<int> mParentRelPosLowerBound;	//lowest x/y value the parent's relative position can be
			Vector2D<int> mParentRelPosUpperBound;	//highest x/y value the parent's relative position can be

		protected:
			void _Update();

		public:
			friend class Root;
			Drager();

			void SetDragsHorizontal(bool dh) { mDragsHorizontal = dh; }
			void SetDragsVertical(bool dv) { mDragsVertical = dv; }

			void SetParentRelPosLowerBound(Vector2D<int> lowerBound) { mParentRelPosLowerBound = lowerBound; }
			void SetParentRelPosUpperBound(Vector2D<int> upperBound) { mParentRelPosUpperBound = upperBound; }
			void SetUseHorizontalBounding(bool useHorizontalBounding) { mUseHorizontalBounding = useHorizontalBounding; }
			void SetUseVerticalBounding(bool useVerticalBounding) { mUseVerticalBounding = useVerticalBounding; }
		};
	};
};


#endif

