/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Animation.cpp
=================================================================================================*/
#include "Constants.h"

namespace ge
{
	namespace world
	{
		/*=============================================================================
		-- Constructor for Animation.
		=============================================================================*/
		Animation::Animation(String type)
		{
			mType				= type;
			mFrameTime			= 0.05;
		}


		/*=============================================================================
		-- Sets the current frame to the next frame in the sheet. Horizontal rows
		   represent rotations, so once at the end of a row, this method will set the
		   current frame to the frame at the beginning of the row.
		=============================================================================*/
		void Animation::_AdvanceFrame()
		{
			mTimer.Start();

			int frame = mSpriteSheet.GetFrame();
			int totalColumns = mSpriteSheet.GetColumns();
			int row = (frame/totalColumns);

			//if the frame is all the way to the right of the sheet, it
			//must loop back to the start of its row
			if ((frame+1) % totalColumns == 0)
				mSpriteSheet.SetFrame(totalColumns*row);
			//otherwise just advance the frame
			else
				mSpriteSheet.SetFrame(frame+1);
		}


		/*=============================================================================
		-- Loads an image from file for the sprite sheet.
		=============================================================================*/
		bool Animation::LoadAnimationFromFile(String filename, AssetManager *assetManager)
		{
		    if (filename == "")
                return false;

			//load the properties from file
			HMLFile file;
			gDebug.Print(String()+DIR_ASSET_ANIMATIONS+filename);
			file.Open(String()+DIR_ASSET_ANIMATIONS+filename);
			bool loadImageSuccess = false;

			if (file.SeekElementOpen("<properties>"))
			{
				while (!file.AtElementClose("</properties>"))
				{
					//load the image for the sprite sheet
					if (file.TokenIsDeclaration("#spriteSheet"))
					{
						//use a shared image from the asset manager if applicable
						if (assetManager)
							loadImageSuccess = mSpriteSheet.Init(NULL, assetManager->GetImage(file.SeekGetToken())->GetImage());
						//load the image directly because there is no asset manager
						else
						{
						    String path = file.SeekGetToken();
							gDebug.Print(String()+DIR_ASSET_GRAPHICS+path);
							loadImageSuccess = mSpriteSheet.LoadImageFromFile(String()+DIR_ASSET_GRAPHICS+path);
						}
					}
					//set the frame size
					else if (file.TokenIsDeclaration("#frameSize"))
					{
						unsigned frameWidth = ToInt(file.SeekGetToken());
						unsigned frameHeight = ToInt(file.SeekGetToken());
						mSpriteSheet.SetFrameDimensions(frameWidth, frameHeight);
					}
					//set the frame time
					else if (file.TokenIsDeclaration("#frameTime"))
						SetFrameTime(ToDouble(file.SeekGetToken()));
					//set the number of columns
					else if (file.TokenIsDeclaration("#columns"))
						mSpriteSheet.SetColumns(ToInt(file.SeekGetToken()));

					//load the rotation offsets for the frames in each rotation
					//note there is one offset used for all frames in each rotation
					else if (file.AtElementOpen("<offsets>"))
					{
						while (!file.AtElementClose("</offsets>"))
						{
							if (file.TokenIsDeclaration("#set"))
							{
								int rotation = ToInt(file.SeekGetToken());
								SharedPtr< Vector2D<int> > offset(new Vector2D<int>(ToVector2D(file.SeekGetToken())));

								mOffsets.insert( std::pair< int, SharedPtr< Vector2D<int> > >(rotation, offset) );
							}

							file.Seek();
						}
					}

					file.Seek();
				}
			}
			else
				gDebug.Error(String()+"Could not load animation "+filename);

			mSpriteSheet.SetIsSheet(true);
			SetCurrentRotation(0);
			mTimer.Start();

			//if the image was not loaded, this was unsucessful because the image is so critical
			return loadImageSuccess;
		}


		/*=============================================================================
		-- Draws the current frame.
		=============================================================================*/
		void Animation::Draw()
		{
			Draw(mSpriteSheet.GetX(), mSpriteSheet.GetY());
		}


		/*=============================================================================
		-- Draws the current frame at the specified position.
		=============================================================================*/
		void Animation::Draw(int x, int y)
		{
			if (mTimer.GetTimeAndEnd() > 1000*mFrameTime)
				_AdvanceFrame();

			SetPos(x-mCurrentOffset.lock()->x, y-mCurrentOffset.lock()->y);
			mSpriteSheet.Draw();
		}


		/*=============================================================================
		-- Sets the current rotation of the animation. The current position in the
		   loop is maintained.
		=============================================================================*/
		void Animation::SetCurrentRotation(int currentRotation)
		{
			int frame = mSpriteSheet.GetFrame();
			int totalColumns = mSpriteSheet.GetColumns();
			int column = (frame+1) % totalColumns;

			//if column is 0, that means the current frame is at the end of the loop
			//so just use the first frame for the rotation. at this point, column-1
			//is -1, which will give undesired results, so we need to seperate
			//the two scenarios
			if (column == 0)
				mSpriteSheet.SetFrame(currentRotation*totalColumns);
			else
				mSpriteSheet.SetFrame(currentRotation*totalColumns + (column-1));

			mCurrentOffset = mOffsets.find(currentRotation)->second;
		}


		void Animation::SetCurrentRotationInDegrees(double rotation)
		{
			double numberOfRotationPositions = mSpriteSheet.GetRows();
			double angleBetweenRotations = 360.0/numberOfRotationPositions;
			int rotationNumber = Round(rotation/angleBetweenRotations);

			SetCurrentRotation(rotationNumber);
		}


		int Animation::GetCurrentRotation()
		{
			return mSpriteSheet.GetFrame() / mSpriteSheet.GetColumns();
		}
	};
};
