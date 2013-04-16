/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Picture.h
=================================================================================================*/

#include "../../GUI.h"

#ifndef _ge_PICTURE_H
#define _ge_PICTURE_H

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- Image element to display images.
		=============================================================================*/
		class Picture : public Element
		{
		private:
			Sprite mImg;			//image
			String mFilepath;		//path to the image


		protected:
			bool _PostInit();
			void _Draw();
			void _UpdateAssets();

		public:
			friend class Root;

			Picture();
			~Picture() {}

			void SetImage(Image *image);

			//accessors and mutators
			void SetFilePath(String filepath) { mFilepath = filepath; }
		};
	};
};

#endif

