/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Map.h
=================================================================================================*/
#include "../World.h"

#ifndef _ge_MAP_H
#define _ge_MAP_H

namespace ge
{
    namespace world
    {
        /*=============================================================================
        -- Represents all layers of the map for a single tile. Each layer is an integer
           that refers to a certain tile image from the map's tileset.
        =============================================================================*/
        class Tile
        {
		private:
			long mId;
            short mLayer1;
            short mLayer2;
			bool mCollidable;
			bool mAddedToGroup;			//only used by map. if false, this tile still needs to be added to a tile group for optimization


		public:
            Tile();
			~Tile() {}

			void SetId(long id) { mId = id; }
			long GetId() { return mId; }
			void SetLayer(short layer, TileTemplate tileTemplate);
			bool GetCollidable() { return mCollidable; }
			short GetLayer1() { return mLayer1; }
			short GetLayer2() { return mLayer2; }
			void SetAddedToGroup(bool added) { mAddedToGroup = added; }
			bool GetAddedToGroup() { return mAddedToGroup; }
        };


		struct TileGroup
		{
			short layer1;
			Rect<double> rect;
		};


        /*=============================================================================
        -- 
        =============================================================================*/
        class Map
        {
        private:
            int mWidth, mHeight;
            std::vector<Tile> mTileData;
            Tileset mTileset;
			World *mWorld;
			std::vector<TileGroup> mCachedTileGroups;

			std::vector<TileGroup> _GenerateTileGroups();
			void _CreatePhysicBodies();

        public:
            Map(World *world);
            bool LoadMapDataFromWorldFile(String filepath);
			
            void AddTile(Tile tile);
			Tileset *GetTileset() { return &mTileset; }
			Tile operator[](unsigned tile);
			Vector3D<double> GetTilePos(long id);

            int GetWidth() { return mWidth; }
            int GetHeight() { return mHeight; }

			void SetWorld(World *world) { mWorld = world; }
			World *GetWorld() { return mWorld; }

			std::vector<TileGroup> GetCachedTileGroups() { return mCachedTileGroups; }

        };

    };
};

#endif

