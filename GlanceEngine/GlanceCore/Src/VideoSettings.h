/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: VideoSettings.h
  Created: 1/18/13 by Dan Watkins
=================================================================================================*/

#ifndef _ge_VIDEOSETTINGS_H
#define _ge_VIDEOSETTINGS_H

#include "Glance.h"

namespace ge
{
    /*=============================================================================
    -- Holds video setting information.
    =============================================================================*/
	struct VideoSettings
	{
		unsigned width, height;
		bool fullscreen, verticalSync;
		bool allowResize, allowCloseButton;
		String title;

		VideoSettings()
		{
			width				= 640;
			height				= 480;
			fullscreen			= false;
			verticalSync		= true;
			allowResize			= false;
			allowCloseButton	= true;
			title				= "Untitled";
		}

		bool operator == (VideoSettings &videoSettings)
		{
			if (videoSettings.width != width) return false;
			if (videoSettings.height != height) return false;
			if (videoSettings.fullscreen != fullscreen) return false;
			if (videoSettings.verticalSync != verticalSync) return false;
			if (videoSettings.allowResize != allowCloseButton) return false;
			if (videoSettings.title != title) return false;

			return false;
		}

		bool operator != (VideoSettings &videoSettings)
		{
			return !(*this == videoSettings);
		}
	};
};

#endif

