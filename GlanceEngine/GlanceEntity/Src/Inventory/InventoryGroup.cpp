/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Inventory.cpp
=================================================================================================*/
#include "../Entity.h"

namespace ge
{
    namespace world
    {
		/*=============================================================================
		-- Adds the object to the object list.
		=============================================================================*/
		void InventoryGroup::AddObject(WeakPtr<Object> object)
		{
			//it is safe to assume the object is not in here already because Inventory already checks
			mObjects.push_back(object);
		}


		/*=============================================================================
		-- Removes the object with the same id from the object list.
		=============================================================================*/
		bool InventoryGroup::RemoveObject(int id)
		{
			std::vector< WeakPtr<Object> >::iterator iter = mObjects.begin();
			while (iter != mObjects.end())
			{
				if ( (*iter).lock()->GetId() == id )
				{
					mObjects.erase(iter);
					return true;
				}

				iter++;
			}

			return false;
		}


		/*=============================================================================
		-- Returns a WeakPtr<> to the object with the id if it is in the object list.
		=============================================================================*/
		WeakPtr<Object> InventoryGroup::GetObject(int id)
		{
			std::vector< WeakPtr<Object> >::iterator iter = mObjects.begin();
			while (iter != mObjects.end())
			{
				if ( (*iter).lock()->GetId() == id )
					return *iter;

				iter++;
			}

			return WeakPtr<Object>();
		}


		/*=============================================================================
		-- Returns a WeakPtr<> to the first object in the object list.
		=============================================================================*/
		WeakPtr<Object> InventoryGroup::GetFirstObject()
		{
			if (mObjects.size() > 0)
				return mObjects.front();

			return WeakPtr<Object>();
		}
	};
};