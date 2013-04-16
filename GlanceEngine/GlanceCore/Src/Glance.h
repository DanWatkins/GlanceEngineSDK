/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Glance.h
=================================================================================================*/

#ifndef _ge_GLANCE_H
#define _ge_GLANCE_H


#include "Constants.h"

#include "Diagnostic.h"
#include "Debug.h"

namespace ge
{
	//forward declarations
	class Window;
	class DebugStream;

	//global debug stream
	extern DebugStream gDebug;

	//forward declarations
	class Sprite;
	class SpriteFrame;

	/*=============================================================================
	-- Version infromation.
	=============================================================================*/
	volatile const int ge_VERSION_MAJOR = 0;
	volatile const int ge_VERSION_MINOR = 27;
	volatile const int ge_VERSION_REVISION = 0;
	String GetVersionText();
};


//regular includes
#include "VideoSettings.h"
#include "Timer.h"
#include "Input.h"
#include "Window.h"

#endif

