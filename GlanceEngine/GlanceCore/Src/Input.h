/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Input.h
=================================================================================================*/
#include "Glance.h"

#ifndef _ge_INPUT_H
#define _ge_INPUT_H

namespace ge
{
    /*=============================================================================
      Key and button macros
    =============================================================================*/
	#define GKEY sf::Key;
	typedef sf::Mouse::Button BUTTON;

    //mainboard letters
	const int GK_A = sf::Key::A;
	const int GK_B = sf::Key::B;
	const int GK_C = sf::Key::C;
	const int GK_D = sf::Key::D;
	const int GK_E = sf::Key::E;
	const int GK_F = sf::Key::F;
	const int GK_G = sf::Key::G;
	const int GK_H = sf::Key::H;
	const int GK_I = sf::Key::I;
	const int GK_J = sf::Key::J;
	const int GK_K = sf::Key::K;
	const int GK_L = sf::Key::L;
	const int GK_M = sf::Key::M;
	const int GK_N = sf::Key::N;
	const int GK_O = sf::Key::O;
	const int GK_P = sf::Key::P;
	const int GK_Q = sf::Key::Q;
	const int GK_R = sf::Key::R;
	const int GK_S = sf::Key::S;
	const int GK_T = sf::Key::T;
	const int GK_U = sf::Key::U;
	const int GK_V = sf::Key::V;
	const int GK_W = sf::Key::W;
	const int GK_X = sf::Key::X;
	const int GK_Y = sf::Key::Y;
	const int GK_Z = sf::Key::Z;

	//mainboard numbers
	const int GK_Num0 = sf::Key::Num0;
	const int GK_Num1 = sf::Key::Num1;
	const int GK_Num2 = sf::Key::Num2;
	const int GK_Num3 = sf::Key::Num3;
	const int GK_Num4 = sf::Key::Num4;
	const int GK_Num5 = sf::Key::Num5;
	const int GK_Num6 = sf::Key::Num6;
	const int GK_Num7 = sf::Key::Num7;
	const int GK_Num8 = sf::Key::Num8;
	const int GK_Num9 = sf::Key::Num9;

	//keypad  numbers
	const int GK_Numpad0 = sf::Key::Numpad0;
	const int GK_Numpad1 = sf::Key::Numpad1;
	const int GK_Numpad2 = sf::Key::Numpad2;
	const int GK_Numpad3 = sf::Key::Numpad3;
	const int GK_Numpad4 = sf::Key::Numpad4;
	const int GK_Numpad5 = sf::Key::Numpad5;
	const int GK_Numpad6 = sf::Key::Numpad6;
	const int GK_Numpad7 = sf::Key::Numpad7;
	const int GK_Numpad8 = sf::Key::Numpad8;
	const int GK_Numpad9 = sf::Key::Numpad9;

	//mainboard other symbols
	const int GK_Grave = sf::Key::Tilde;
	const int GK_Backspace = sf::Key::Back;
	const int GK_Space = sf::Key::Space;
	const int GK_Add = sf::Key::Add;
	const int GK_Subtract = sf::Key::Subtract;
	const int GK_Multiply = sf::Key::Multiply;
	const int GK_Divide = sf::Key::Divide;
	const int GK_Dash = sf::Key::Dash;
	const int GK_Equals = sf::Key::Equal;
	const int GK_LBracket = sf::Key::LBracket;
	const int GK_RBracket = sf::Key::RBracket;
	const int GK_SemiColon = sf::Key::SemiColon;
	const int GK_Quote = sf::Key::Quote;
	const int GK_Comma = sf::Key::Comma;
	const int GK_Period = sf::Key::Period;
	const int GK_Slash = sf::Key::Slash;
	const int GK_Backslash = sf::Key::BackSlash;

	//mainboard functionality
	const int GK_Tab = sf::Key::Tab;
	const int GK_LShift = sf::Key::LShift;
	const int GK_RShift = sf::Key::RShift;
	const int GK_LCtrl = sf::Key::LControl;
	const int GK_RCtrl = sf::Key::RControl;
	const int GK_LWindows = sf::Key::LSystem;
	const int GK_RWindows = sf::Key::RSystem;
	const int GK_LAlt = sf::Key::LAlt;
	const int GK_RAlt = sf::Key::RAlt;
	const int GK_Context = sf::Key::Menu;
	const int GK_Enter = sf::Key::Return;
	const int GK_Escape = sf::Key::Escape;
	const int GK_Insert = sf::Key::Insert;
	const int GK_Delete = sf::Key::Delete;
	const int GK_Left = sf::Key::Left;
	const int GK_Up = sf::Key::Up;
	const int GK_Right = sf::Key::Right;
	const int GK_Down = sf::Key::Down;

	//function keys
	const int GK_F1 = sf::Key::F1;
	const int GK_F2 = sf::Key::F2;
	const int GK_F3 = sf::Key::F3;
	const int GK_F4 = sf::Key::F4;
	const int GK_F5 = sf::Key::F5;
	const int GK_F6 = sf::Key::F6;
	const int GK_F7 = sf::Key::F7;
	const int GK_F8 = sf::Key::F8;
	const int GK_F9 = sf::Key::F9;
	const int GK_F10 = sf::Key::F10;
	const int GK_F11 = sf::Key::F11;
	const int GK_F12 = sf::Key::F12;
	const int GK_F13 = sf::Key::F13;
	const int GK_F14 = sf::Key::F14;
	const int GK_F15 = sf::Key::F15;

    //mouse buttons
	const int GMB_Left = sf::Mouse::Left;
	const int GMB_Right  = sf::Mouse::Right;
	const int GMB_Center = sf::Mouse::Middle;
	const int GMB_LeftSide = sf::Mouse::XButton1;
	const int GMB_RightSide = sf::Mouse::XButton2;


    /*=============================================================================
      Key state enumeration
    =============================================================================*/
    enum KeyState
    {
        KEY_DOWN,
        KEY_PRESSED,
        KEY_UP,
        KEY_RELEASED
    };


    /*=============================================================================
      Input class for managing keyboard and mouse events.
    =============================================================================*/
    class Input
    {
        sf::Event mEvent;                       //SFML event object
        sf::Window *mWindow;                    //SFML window pointer

        //keyboard keys
        bool mKeysReleased[512];                //record of keys released (down previous frame, up now)
        bool mKeysPressed[512];                 //record of keys pressed (up previous frame, down now)
        bool mKeysOld[512];                     //record of keys from the last frame

		//text entered
		String mTextEntered;					//text entered for the current frame


      public:
        Input();
        ~Input() {}

		//general
        bool Init(sf::Window &window);
        void Update();

		//accessor and mutator
        bool GetKeyState(int key, KeyState state=KEY_RELEASED);
        bool GetButtonState(int button);
        int GetMouseX() { return mWindow->GetInput().GetMouseX(); }
        int GetMouseY() { return mWindow->GetInput().GetMouseY(); }
		Vector2D<int> GetMousePos() { return Vector2D<int>(GetMouseX(), GetMouseY()); }

        sf::Event *GetEvent() { return &mEvent; }
		String GetTextEntered() { return mTextEntered; }
    };
};

#endif

