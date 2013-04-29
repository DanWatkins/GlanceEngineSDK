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
		-- Adds the object to the inventory systems. It also notifies the object that
		   it is being "held" and should not be active on the map anymore.
		=============================================================================*/
		void Inventory::AddObject(WeakPtr<Object> object)
		{
			if (object.expired())
				return;

			//check if the object is being held by anything
			if (object.lock()->GetHeld())
				return;

			//the object can logically be held by this inventory system, proceed with adding it
			object.lock()->SetHeld(true);
			bool added = false;

			//try adding it to an existing group if one exists and the object can be combined
			if (object.lock()->GetCombined())
			{
				std::vector<InventoryGroup>::iterator iter = mGroups.begin();
				while (iter != mGroups.end())
				{
					if ( (*iter).GetName() == object.lock()->GetName() )
					{
						(*iter).AddObject(object);
						added = true;
					}

					iter++;
				}
			}

			//make a new group and add the object if a group was not found for the object
			if (!added)
			{
				mGroups.push_back(InventoryGroup());
				mGroups.back().SetName(object.lock()->GetName());
				mGroups.back().AddObject(object);
			}

			//make this object the active object if this is the only object in the whole system
			if (mGroups.back().GetObjectCount() == 1  &&  mGroups.size() == 1)
				mActiveObject = object;
		}


		/*=============================================================================
		-- Removes an object in the inventory system with the id if it exists.
		=============================================================================*/
		void Inventory::RemoveObject(int id)
		{
			//try removing it from every group until it is successful
			std::vector<InventoryGroup>::iterator iter = mGroups.begin();
			while (iter != mGroups.end())
			{
				//is this object in this group?
				if ( !(*iter).GetObject(id).expired() )
				{
					if (!mActiveObject.expired())
					{
						//if the active object was just removed it needs to be reassigned to a new object
						if (mActiveObject.lock()->GetId() == id )
						{
							if ( (*iter).GetObjectCount() > 1 )
							{
								(*iter).RemoveObject(id);
								mActiveObject = (*iter).GetFirstObject();
								return;
							}
							else
							{
								CycleActiveObject();
								(*iter).RemoveObject(id);//does not need to happen because the next line will do effectively the same task
								mGroups.erase(iter);

								//clear the active object when everything is empty (hacky but hey)
								if (mGroups.size() == 0)
									mActiveObject.reset();

								return;
							}
						}
					}

					break;
				}

				iter++;
			}
		}


		/*=============================================================================
		-- Returns a WeakPtr to an object with the id if it is in the inventory system.
		=============================================================================*/
		WeakPtr<Object> Inventory::GetObject(int id)
		{
			//search every group
			std::vector<InventoryGroup>::iterator iter = mGroups.begin();
			while (iter != mGroups.end())
			{
				WeakPtr<Object> object = (*iter).GetObject(id);
				if (!object.expired())
					return object;

				iter++;
			}

			return WeakPtr<Object>();
		}


		/*=============================================================================
		-- Sets the active object to an object in the system with the same id if it
		   exists. If it does not, the method returns false.
		=============================================================================*/
		bool Inventory::SetActiveObject(int id)
		{
			WeakPtr<Object> object = GetObject(id);

			if (!object.expired())
			{
				mActiveObject = object;
				return true;
			}

			return false;
		}


		/*=============================================================================
		-- Changes the active object to the next object in the inventory list.
		=============================================================================*/
		void Inventory::CycleActiveObject()
		{
			if (mGroups.size() > 0)
			{
				std::vector<InventoryGroup>::iterator iter = mGroups.begin();
				while (iter != mGroups.end())
				{
					if ( (*iter).GetFirstObject().lock() == mActiveObject.lock() )
					{
						iter++;

						if (iter != mGroups.end())
							mActiveObject = (*iter).GetFirstObject();
						else
							mActiveObject = (*mGroups.begin()).GetFirstObject();

						return;
					}

					iter++;
				}
			}

			//set the active object to NULL because there is nothing for it to point to
			mActiveObject.reset();
		}


		/*=============================================================================
		-- Returns the active group.
		=============================================================================*/
		InventoryGroup Inventory::GetActiveGroup()
		{
			std::vector<InventoryGroup>::iterator iter = mGroups.begin();
			while (iter != mGroups.end())
			{
				if ( (*iter).GetName() == mActiveObject.lock()->GetName() )
					return *iter;

				iter++;
			}

			return InventoryGroup();
		}


		/*=============================================================================
		-- Returns the total number of objects in every group.
		=============================================================================*/
		int Inventory::GetTotalObjectCount()
		{
			int count = 0;
			std::vector<InventoryGroup>::iterator iter = mGroups.begin();

			while (iter != mGroups.end())
			{
				count += (*iter).GetObjectCount();
				iter++;
			}

			return count;
		}
	};
};