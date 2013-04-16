/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: TileSet.h
=================================================================================================*/
#include "../World.h"

#ifndef _ge_TILESET_H
#define _ge_TILESET_H

namespace ge
{
    namespace world
    {
		/*=============================================================================
        -- Template that stores properties to make a tile.
        =============================================================================*/
		struct TileTemplate
		{
			TileTemplate()
			{
				mName = "";
				mRef = -1;
				mCollidable = false;
			}

			String mName;
			short mRef;
			bool mCollidable;
		};



        /*=============================================================================
        -- Contains the tilesheet image to represent tiles n the map, as well as the
           collision data and special porperties associated with each type of tile.
        =============================================================================*/
        class Tileset
        {
        private:
            String mName;
            Sprite mImage;
            int mTileWidth, mTileHeight;
			std::vector< TileTemplate > mTileTemplates;

        public:
            Tileset();

			Sprite *GetImage() { return &mImage; }
			TileTemplate GetTileTemplate(int ref);
            bool LoadTilesetFromFile(String filename);
			void Draw(unsigned tile, int posx, int posy);

            String GetName() { return mName; }
            int GetTileWidth() { return mTileWidth; }
            int GetTileHeight() { return mTileHeight; }
        };
    };
};

#endif

