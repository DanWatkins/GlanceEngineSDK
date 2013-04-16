/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Constatns.h
=================================================================================================*/

#ifndef _ge_CONSTANTS_H
#define _ge_CONSTANTS_H

/*=============================================================================
  Includes
=============================================================================*/

//Windows specific includes
#ifdef BUILD_WINDOWS
#include <Windows.h>
#include <memory>
#endif

//Linux specific includes
#ifdef BUILD_LINUX
#include <tr1/memory>
#endif

//SFML includes
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

//boost includes
#include <boost/filesystem.hpp>

//jam includes
#include "../../GlanceUtility/Src/Utility.h"


#define DEFAULT_FONT "consolas"

//directory shortcuts
#define DIR_FONTS                       "../Resources/Fonts/"
#define DIR_GUI                         "../Resources/Interface/"
#define DIR_ASSET_DATA                  "../Resources/Assets/"
#define DIR_ASSET_GRAPHICS              "../Resources/Assets/Graphics/"
#define DIR_ASSET_ANIMATIONS			"../Resources/Assets/Animations/"
#define DIR_TILESETS                    "../Resources/Tilesets/"


namespace ge
{
	/*=============================================================================
      Color structure that separates the engine from SFML's color class.
    =============================================================================*/
    class Color
    {
	private:
        sf::Color color;


	public:
        Color() { Set(255, 255, 255, 255); }

        Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
        {
            Set(r, g, b, a);
        }

		Color(sf::Color color)
		{
			Set(color.r, color.g, color.b, color.a);
		}

        void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
        {
			color.r = r;
            color.g = g;
            color.b = b;
            color.a = a;
        }

        sf::Color Get() { return color; }
		int R() { return color.r; }
		int G() { return color.g; }
		int B() { return color.b; }
		int A() { return color.a; }
    };


    /*=============================================================================
      Client external functions.
    =============================================================================*/
    extern bool ClientInit();
    extern void ClientLoop();


    /*=============================================================================
      Pre-defined colors.
    =============================================================================*/
    const Color gWhite(255, 255, 255, 255);
    const Color gGray(128, 128, 128, 255);
    const Color gBlack(0, 0, 0, 255);
    const Color gPink (255, 128, 255, 255);

    const Color gYellow(255, 255, 0, 255);
    const Color gOrange(255, 192, 0, 255);
    const Color gRed(255, 0, 0, 255);
    const Color gBlue(0, 0, 255, 255);
    const Color gGreen(0, 255, 0, 255);
    const Color gPurple(255, 255, 0, 255);
};

#endif

