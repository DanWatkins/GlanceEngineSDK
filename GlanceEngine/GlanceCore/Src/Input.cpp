/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Input.cpp
=================================================================================================*/
#include "Glance.h"

namespace ge
{
	/*=============================================================================
    -- Constructor for Input.
    =============================================================================*/
	Input::Input()
	{
		mWindow = NULL;
		std::memset(&mKeysReleased, false, sizeof(mKeysReleased));
		std::memset(&mKeysPressed, false, sizeof(mKeysPressed));
		std::memset(&mKeysOld, false, sizeof(mKeysOld));
	}


    /*=============================================================================
    -- Initializes Input
    =============================================================================*/
    bool Input::Init(sf::Window &window)
    {
        //assign the window
        mWindow = &window;

        return true;
    }


    /*=============================================================================
    -- Updates Input
    =============================================================================*/
    void Input::Update()
    {
        //update every key's state
        for (int n=0; n<512; n++)
        {
            mKeysReleased[n] = false;
            mKeysPressed[n] = false;

            //was the key just released?
            if (mKeysOld[n])
            {
                if (!GetKeyState(n, KEY_DOWN))
                    mKeysReleased[n] = true;
            }

            //or was the key just pressed?
            else if (!mKeysOld[n])
            {
                if (GetKeyState(n, KEY_DOWN))
                    mKeysPressed[n] = true;
            }

            //save the current state
            mKeysOld[n] = GetKeyState(n, KEY_DOWN);
        }


		//get the text entered this frame
		mTextEntered = "";
		while (mWindow->GetEvent(mEvent))
		{
			if (mEvent.Type == sf::Event::TextEntered)
			{
				sf::Uint32 unicode = mEvent.Text.Unicode;
				char u1 = (char)unicode;
				std::string s1 = "";
				s1.push_back(u1);
				mTextEntered.PushBack(s1);
			}
		}
    }


    /*=============================================================================
    -- Returns true if the specified key is in the specified state.
    =============================================================================*/
    bool Input::GetKeyState(int key, KeyState state)
    {
        switch (state)
        {
        case KEY_DOWN:
			return mWindow->GetInput().IsKeyDown(static_cast<sf::Key::Code>(key));

        case KEY_RELEASED:
            return mKeysReleased[key];

        case KEY_UP:
            return !mWindow->GetInput().IsKeyDown(static_cast<sf::Key::Code>(key));

        case KEY_PRESSED:
            return mKeysPressed[key];

        default:
            return false;
        }
    }


    /*=============================================================================
    -- Returns true if the specified mouse button is down.
    =============================================================================*/
    bool Input::GetButtonState(int button)
    {
        return mWindow->GetInput().IsMouseButtonDown(static_cast<BUTTON>(button)) != 0;
    }

};