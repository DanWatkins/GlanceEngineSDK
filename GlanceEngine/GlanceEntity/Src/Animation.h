/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Animation.h
=================================================================================================*/
#include "Constants.h"

#ifndef _ge_ANIMATION_H
#define _ge_ANIMATION_H

namespace ge
{
	namespace world
	{
		namespace stance
		{
			const String Standing = "standing";
			const String Walking = "walking";
		};


		/*=============================================================================
		-- Handles animation for 2d sprite sheets.
		=============================================================================*/
		class Animation
		{
		private:
			String mType;											//type that specifies what the animation resembles. It is used to switch to a certain animation in an AnimationAnimation
			Timer mTimer;											//timer that is reset every time a new frame is shown
			Sprite mSpriteSheet;									//sprite that actually renders the frames
			double mFrameTime;										//number of seconds each frame is shown in real time

			WeakPtr< Vector2D<int> > mCurrentOffset;					//pointer to the offset for the current rotation
			std::map< int, SharedPtr< Vector2D<int> > > mOffsets;	//contains vectors that represent the offsets for each rotation's images. first vector is for first rotation

			void _AdvanceFrame();


		public:
			Animation(String type);
			~Animation() {}

			void Draw();
			void Draw(int x, int y);

			bool LoadAnimationFromFile(String filename, AssetManager *assetManager);
			void SetWindow(Window *window) { mSpriteSheet.SetWindow(window); }
			

			String GetType() { return mType; }
			void SetFrameTime(double frameTime) { mFrameTime = frameTime; }
			double GetFrameTime() { return mFrameTime; }

			void SetCurrentRotationInDegrees(double rotation);
			void SetCurrentRotation(int currentRotation);
			int GetCurrentRotation();

			void SetX(int x) { mSpriteSheet.SetX(x); }
			void SetY(int y) { mSpriteSheet.SetY(y); }
			void SetPos(int x, int y) { SetX(x); SetY(y); }
		};
	};
};

#endif

