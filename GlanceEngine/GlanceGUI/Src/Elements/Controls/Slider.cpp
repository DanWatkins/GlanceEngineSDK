/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Slider.cpp
=================================================================================================*/

#include "../../GUI.h"

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- Initializes the Slider after it has been added to the Root. Automatically
		   called from its corresponding Root::Create() method.
		=============================================================================*/
		bool Slider::_PostInit()
		{
			mKnobButton = GetRoot()->CreateButtonCaption(-31, GetId(), Vector2D<int>(-(KNOB_WIDTH/2),-(KNOB_HEIGHT/2)), KNOB_WIDTH, KNOB_HEIGHT, "");
			mKnobDrager = GetRoot()->CreateDrager(-32, mKnobButton, Vector2D<int>(0,0), KNOB_WIDTH, KNOB_HEIGHT, true, false);
			mKnobDrager.lock()->SetUseHorizontalBounding(true);

			//load the sprite frames
			mImgTrack.Init(GetWindow(), GetRoot()->GetAssetManager()->GetImage("track.png")->GetImage());

			return true;
		}


		/*=============================================================================
		-- Draws the button depending on the state and the caption overlay.
		=============================================================================*/
		void Slider::_Draw()
		{
			mImgTrack.Draw();
		}


		/*=============================================================================
		-- Updates the position and size of each asset.
		=============================================================================*/
		void Slider::_UpdateAssets()
		{
			if (!mKnobDrager.expired())
			{
				mKnobDrager.lock()->SetParentRelPosLowerBound(Vector2D<int>(-KNOB_WIDTH/2, 0));
				mKnobDrager.lock()->SetParentRelPosUpperBound(Vector2D<int>(GetWidth()-(KNOB_WIDTH/2), 0));
			}

			mImgTrack.SetSize(GetWidth(), TRACK_HEIGHT);
			mImgTrack.SetPos(GetScreenPos());

			if (!mKnobDrager.expired())
			{
				if (GetPercentOffset() != mOldPercentOffset)
					_SendElementMessageToListeners(ElementEvent::VALUE_CHANGED, ToString(GetPercentOffset()));

				mOldPercentOffset = GetPercentOffset();
			}
		}
	};
};