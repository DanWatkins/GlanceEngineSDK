/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Window.cpp
=================================================================================================*/
#include "Glance.h"

namespace ge
{
    /*=============================================================================
      Constructor for Window
    =============================================================================*/
    Window::Window()
    {
		mVideoSettings.width		= 800;
		mVideoSettings.height		= 600;
		mVideoSettings.fullscreen	= false;
		mVideoSettings.verticalSync	= true;
		mVideoSettings.title		= Combine("GlanceEngine ", GetVersionText(), " - (C) 2011-2012 Daniel L. Watkins");
		mLastVideoSettings			= mVideoSettings;

		mQuit						= false;
        mBackgroundColor			= gWhite;
		mUpdateRate					= 16;
		mWasResized					= false;
		mUseCustomCursor			= false;	//if set to true, you need to call UseCustomCursor(bool)
    }


    /*=============================================================================
      Creates the main window and loads the rest of the engine.
    =============================================================================*/
	bool Window::Init(String uniqueIdentifier)
	{
		//load settings from configuration file
		if (LoadConfigurationFile(uniqueIdentifier) == false)
			WriteConfigurationFile(uniqueIdentifier);

        ApplyVideoSettings();

		sf::Image *image = new sf::Image;
		image->LoadFromFile("cursor.png");
		mCursor.SetImage(*image);
		UseCustomCursor(true);

        return true;
    }


	/*=============================================================================
      Loads the video mode file associated with the uniqe identifier.
    =============================================================================*/
	bool Window::LoadConfigurationFile(String uniqueIdentifier)
	{
		HMLFile file;
		file.Open(uniqueIdentifier+".vmode");
		if (file.IsOpen())
		{
			if (file.SeekElementOpen("<metrics>"))
			{
				while (!file.AtElementClose("</metrics>"))
				{
					if (file.TokenIsDeclaration("#screenWidth"))
						mVideoSettings.width = ToInt(file.SeekGetToken());
					else if (file.TokenIsDeclaration("#screenHeight"))
						mVideoSettings.height = ToInt(file.SeekGetToken());
					else if (file.TokenIsDeclaration("#fullscreen"))
						mVideoSettings.fullscreen = ToBool(file.SeekGetToken());
					else if (file.TokenIsDeclaration("#verticalSync"))
						mVideoSettings.verticalSync = ToBool(file.SeekGetToken());

					file.Seek();
				}
			}
		}
		else
			return false;

		return true;
	}


	/*=============================================================================
      Writes out the current video settings to the video mode file associated with
	  the current unique identifier.
    =============================================================================*/
	void Window::WriteConfigurationFile(String uniqueIdentifier)
	{
		std::ofstream file((uniqueIdentifier+".vmode").GetChar());
		if (file.is_open())
		{
			file << "<metrics>" << std::endl;
			file << "	#screenWidth " << mVideoSettings.width << std::endl;
			file << "	#screenHeight " << mVideoSettings.height << std::endl;
			file << "	#fullscreen " << mVideoSettings.fullscreen << std::endl;
			file << "	#verticalSync " << mVideoSettings.verticalSync << std::endl;
			file << "</metrics>" << std::endl;
			file.flush();
			file.close();
		}
	}


    /*=============================================================================
      Updates the engine.
    =============================================================================*/
    void Window::Update()
    {
        //update input system
        mInput.Update();

        while (mWindow.GetEvent(*mInput.GetEvent()))
        {
            if (mInput.GetEvent()->Type == sf::Event::Closed)
                SetQuit(true);
        }

		ClientLoop();

		//draw custom cursor?
		if (mUseCustomCursor)
		{
			int mouseX = mInput.GetMouseX();
			int mouseY = mInput.GetMouseY();

			//make sure the position of the cursor is within the window
			if (mouseX >= 0  &&  mouseX < (int)GetWidth()  &&  mouseY >= 0  &&  (int)GetHeight())
			{
				mCursor.SetPosition((float)mouseX, (float)mouseY);
				mWindow.Draw(mCursor);
			}
		}

		//display everything and then clear it out
		mWindow.Display();
		mWindow.Clear(mBackgroundColor.Get());

		mLastUpdateTime = mClock.GetTimeAndEnd();
		mClock.Start();

		//check if @mVideoSetting has changed, but not been applied
		if (mVideoSettings != mLastVideoSettings)
			mVideoSettings = mLastVideoSettings;

		//check if the window was resized
		mWasResized = false;
		if (mWindow.GetWidth() != mVideoSettings.width  ||  mWindow.GetHeight() != mVideoSettings.height)
		{
			mVideoSettings.width = mWindow.GetWidth();
			mVideoSettings.height = mWindow.GetHeight();
			ApplyVideoSettings();
			mWasResized = true;
		}
    }


    /*=============================================================================
      Ends the engine.
    =============================================================================*/
    void Window::Shutdown()
    {
		delete mCursor.GetImage();
        mWindow.Close();
    }


	double Window::GetUpdateFPS()
	{
		return 1000.0/mLastUpdateTime;
	}


	/*=============================================================================
      Recreates the window using the specified video settings.
    =============================================================================*/
	bool Window::ApplyVideoSettings()
	{
		unsigned long windowStyle;
		windowStyle = mVideoSettings.fullscreen ? sf::Style::Fullscreen : sf::Style::Titlebar;
		windowStyle = mVideoSettings.allowCloseButton ? (sf::Style::Close | windowStyle) : windowStyle;
		windowStyle = mVideoSettings.allowResize ? (sf::Style::Resize | windowStyle) : windowStyle;

		mWindow.Create(sf::VideoMode(mVideoSettings.width, mVideoSettings.height, 32), mVideoSettings.title.GetStd(), windowStyle);

        //initialize the input system
        if (!mInput.Init(mWindow))
            return false;

		mWindow.UseVerticalSync(mVideoSettings.verticalSync);

		mLastVideoSettings = mVideoSettings;

		UseCustomCursor(mUseCustomCursor);

		return true;
	}
};
