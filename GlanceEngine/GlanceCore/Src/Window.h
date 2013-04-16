/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Window.h
=================================================================================================*/

#ifndef _ge_WINDOW_H
#define _ge_WINDOW_H

#include "Glance.h"

namespace ge
{
    /*=============================================================================
    -- Window to render stuff too.
    =============================================================================*/
    class Window
    {
    private:
		VideoSettings mVideoSettings, mLastVideoSettings;
		bool mWasResized;

		bool mQuit;											//if true, the window will close next start of the frame
        sf::RenderWindow mWindow;							//SFML render window pointer
        Color mBackgroundColor;								//color to clear the back buffer to
        Input mInput;										//input manager

		Timer mClock;										//keeps the update rate
		double mUpdateRate;									//minimum length for a clock cycle (int milli-seconds)
		double mLastUpdateTime;								//time it took for the last update to complete (in milli-seconds)

		sf::Sprite  mCursor;
		bool mUseCustomCursor;

    public:
        Window();
        ~Window() {}

		//general
        bool Init(String uniqueIdentifier);
        void Update();
        void Shutdown();

		bool LoadConfigurationFile(String uniqueIdentifier);
		void WriteConfigurationFile(String uniqueIdentifier);

		void UseCustomCursor(bool useCustomCursor) { mUseCustomCursor = useCustomCursor;  mWindow.ShowMouseCursor(!useCustomCursor); }

		//video mode
		VideoSettings *GetVideoSettings() { return &mVideoSettings; }
		bool ApplyVideoSettings();

		unsigned GetWidth() { return mVideoSettings.width; }
		unsigned GetHeight() { return mVideoSettings.height; }

        //accessor/mutators
        bool GetQuit() { return mQuit; }
        void SetQuit(bool quit) { mQuit = quit; }
        sf::RenderWindow *GetWindow() { return &mWindow; }
        void SetBackgroundColor(Color color) { mBackgroundColor = color; }
        Color GetBackgroundColor() { return mBackgroundColor; }
		Input *GetInput() { return &mInput; }
		bool GetWasResized() { return mWasResized; }

		//timing
		double GetUpdateFPS();
		double GetLastUpdateTime() { return mLastUpdateTime; }
    };

};

#endif

