/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: InventoryGroup.h
=================================================================================================*/
#include "../Entity.h"

#ifndef _ge_INVENTORY_GROUP_H
#define _ge_INVENTORY_GROUP_H

namespace ge
{
    namespace world
    {
		/*=============================================================================
        -- Group that contains a bunch of the same objects (same template object).
        =============================================================================*/
		class InventoryGroup
		{
		private:
			std::vector< WeakPtr<Object> > mObjects;		//container for the objects
			String mName;									//name of the objects allowed in this group (only one name)

		public:
			InventoryGroup() {}

			//general
			void AddObject(WeakPtr<Object> object);
			bool RemoveObject(int id);
			WeakPtr<Object> GetObject(int id);
			WeakPtr<Object> GetFirstObject();

			//accessor and mutator
			void SetName(String name) { mName = name; }
			String GetName() { return mName; }
			int GetObjectCount() { return mObjects.size(); }
		};
	};
};

#endif

