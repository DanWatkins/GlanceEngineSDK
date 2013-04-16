/*=================================================================================================
  Glance Resource Editor - (C) 2013 Daniel L. Watkins

  Filename: Main.h
=================================================================================================*/
#ifndef _gre_MAIN_H
#define _gre_MAIN_H

#include "../../../GlanceEngine/GlanceGUI/Src/GUI.h"
#include "../../../GlanceEngine/GlanceWorld/Src/World.h"
#include "../../../GlanceEngine/GlanceAudio/Src/Audio.h"

using namespace ge::gui;

namespace ge
{
	namespace gre
	{
		volatile const int VERSION_MAJOR		= 0;
		volatile const int VERSION_MINOR		= 0;
		volatile const int VERSION_MICRO		= 4;

		inline String GetVersionText()
		{
			return String()+"v"+ToString(VERSION_MAJOR)+"."+ToString(VERSION_MINOR)+"."+ToString(VERSION_MICRO);
		}


		class ResourceEditor;
	};
};

#include "eid.h"

#include "Editor.h"
#include "SubEditor.h"

#include "EntityEditor.h"
#include "ResourceEditor.h"

#endif

