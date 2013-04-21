/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Glance.cpp
=================================================================================================*/

#include "Glance.h"

namespace ge
{
	DebugStream gDebug(true, "debug.txt");


	/*=============================================================================
	  Returns the version of the engine in the form of a String
	=============================================================================*/
	String GetVersionText()
	{
		if (ge_VERSION_MINOR > 9)
			return String("v")+ge_VERSION_MAJOR+"."+ge_VERSION_MINOR+"."+ge_VERSION_REVISION;


		return String("v")+ge_VERSION_MAJOR+".0"+ge_VERSION_MINOR+"."+ge_VERSION_REVISION;
	}
};