/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Inventory.h
=================================================================================================*/
#include "../Entity.h"

#ifndef _ge_INVENTORY_H
#define _ge_INVENTORY_H

namespace ge
{
    namespace world
    {
		/*----  INVENTORY SYSTEM NOTES ----

		1) When an object is added to the system, it will be added to an existing group that has the same name.
		   OR it will be added to a new group named after the object. If the object is not supposed to be grouped,
		   it will automatically be added to its own group regardless if another group with the same name already
		   is in the system.

		2) Objects cannot be added to the system multiple times. Whenever a client attempts to add an object to the
		   system, the method adding the object will check if it is already in the system. If the object is, then
		   the adding procedure will be aborted since it does not make sense to have to instances of the same object
		   in the inventory. Two objects are considered to be the same if their id values are the same, not thier name.
		*/

		/*=============================================================================
		-- Manages a list of objects an Actor is holding.
		=============================================================================*/
		class Inventory
		{
		private:
			std::vector< InventoryGroup > mGroups;			//contains inventroy groups
			WeakPtr<Object> mActiveObject;					//current object "in the hosts' hand" essentially

		public:
			Inventory() {}
			~Inventory() {}

			//general
			void AddObject(WeakPtr<Object> object);
			void RemoveObject(int id);
			WeakPtr<Object> GetObject(int id);

			//active object management
			bool SetActiveObject(int id);
			WeakPtr<Object> GetActiveObject() { return mActiveObject; }
			void CycleActiveObject();
			InventoryGroup GetActiveGroup();
			int GetTotalObjectCount();
		};
	};
};

#endif

