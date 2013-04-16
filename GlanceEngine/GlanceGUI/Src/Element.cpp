/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Element.cpp
=================================================================================================*/

#include "GUI.h"

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- Constructor for Element.
		=============================================================================*/
		Element::Element()
		{
			mId							= -1;
			mWidth						= 100;
			mHeight						= 50;
			mState						= UNDEFINED;
			mRoot						= NULL;
			mDisabled					= false;
			mWindow						= NULL;
			mHasFocus					= false;
			mScheduledToBringToFocus	= false;
			mScheduledToBeRemoved		= false;
			mDraws						= true;
			mSolid						= true;
			mCanFocus					= true;
		}


		/*=============================================================================
		-- Returns true if the point is contained in the element's dimesnions.
		=============================================================================*/
		bool Element::_ContainsPoint(Vector2D<int> point)
		{
			Rect<int> rect;
			rect.Set(GetScreenPos().x, GetScreenPos().y, GetWidth(), GetHeight());

			return rect.Intersect(point);
		}


		/*=============================================================================
		-- Returns a vector of every derived element that contains the point.
		=============================================================================*/
		std::vector< SharedPtr<Element> > Element::_GetAllDerivedElementsWithIntersectingPoint(Vector2D<int> point)
		{
			std::vector< SharedPtr<Element> > relevantElements;

			//add every child and derived element that contains @point
			std::vector< SharedPtr<Element> >::iterator iter = mChildren.begin();
			while (iter != mChildren.end())
			{
				if ((*iter)->GetDraws())
				{
					if ( (*iter)->_ContainsPoint(point) )
						relevantElements.insert(relevantElements.begin(),*iter);

					//get a vector<> for the child's derived elements containing the point
					std::vector< SharedPtr<Element> > childElements = (*iter)->_GetAllDerivedElementsWithIntersectingPoint(point);

					relevantElements.insert(relevantElements.begin(), childElements.begin(), childElements.end());
				}

				iter++;
			}

			return relevantElements;
		}


		/*=============================================================================
		-- Adjusts the size of the element and any assets that need to.
		=============================================================================*/
		void Element::_SetSize(unsigned width, unsigned height)
		{
			mWidth = width;
			mHeight = height;
			_UpdateAssets();
		}


		/*=============================================================================
		-- Initializes basic stuff.
		=============================================================================*/
		bool Element::_Init(int id, WeakPtr<Element> parent, Root *root, Vector2D<int> pos, unsigned width, unsigned height, Window *window)
		{
			bool error = false;

			mId		= id;
			mParent	= parent;
			mRoot	= root;

			//check that the root is valid
			if (!mRoot)
				error = true;

			SetRelPos(pos);
			mWidth		= width;
			mHeight		= height;
			mWindow		= window;

			if (!mWindow)
				error = true;

			_UpdateScreenPos();

			return !error;
		}


		/*=============================================================================
		-- Updates every child and their children.
		=============================================================================*/
		void Element::_UpdateChildren()
		{
			std::vector< SharedPtr<Element> >::iterator iter = mChildren.begin();
			while (iter != mChildren.end())
			{
				(*iter)->_Update();
				(*iter)->_UpdateChildren();

				iter++;
			}
		}


		/*=============================================================================
		-- Draws every child and their children.
		=============================================================================*/
		void Element::_DrawChildren()
		{
			std::vector< SharedPtr<Element> >::iterator iter = mChildren.begin();
			while (iter != mChildren.end())
			{
				if ((*iter)->GetDraws())
				{
					(*iter)->_Draw();
					(*iter)->_DrawChildren();
				}

				iter++;
			}
		}


		/*=============================================================================
		-- Recalculates the screen position.
		=============================================================================*/
		void Element::_UpdateScreenPos(bool goUpTree)
		{
			//update all the way up the tree
			if (goUpTree)
			{
				if (!mParent.expired())
				{
					mParent.lock()->_UpdateScreenPos(true);

					mScreenPos = mParent.lock()->GetScreenPos()+mRelPos;
				}
				//there is no parent, so it probably is a direct descendant the root
				else
					mScreenPos = mRelPos;
			}
			//simply add relPos to parent's screen position
			else if (!mParent.expired())
				mScreenPos = mRelPos+mParent.lock()->GetScreenPos();
			//it is a direct descendant of the root
			else
				mScreenPos = mRelPos;

			//update down the tree
			std::vector< SharedPtr<Element> >::iterator iter = mChildren.begin();
			while (iter != mChildren.end())
			{
				(*iter)->_UpdateScreenPos(false);
				iter++;
			}

			_UpdateAssets();
		}


		/*=============================================================================
		-- Searches only the direct children of this element
		=============================================================================*/
		WeakPtr<Element> Element::GetChild(int id)
		{
			std::vector< SharedPtr<Element> >::iterator iter = mChildren.begin();
			while (iter != mChildren.end())
			{
				if ( (*iter)->GetId() == id )
					return WeakPtr<Element>(*iter);

				iter++;
			}

			return WeakPtr<Element>();
		}


		/*=============================================================================
		-- Searches for an element with @id that is below this element, meaning it
		   searches the children of this element and all of their children and downward.
		   Does not include integrated elements with ids < 0.
		=============================================================================*/
		WeakPtr<Element> Element::_GetElementSearch(unsigned id)
		{
			std::vector< SharedPtr<Element> >::iterator iter = mChildren.begin();
			while (iter != mChildren.end())
			{
				//check iter itself (disregard negative id's because they are reserved for integrated elements
				if ( (*iter)->GetId() == (int)id  &&  (*iter)->GetId() >= 0)
					return WeakPtr<Element>(*iter);

				//check the children of iter
				WeakPtr<Element> el = (*iter)->_GetElementSearch(id);
				if (!el.expired())
				{
					if (el.lock()->GetId() == (int)id)
						return el;
				}

				iter++;
			}

			return WeakPtr<Element>();
		}


		/*=============================================================================
		-- Returns itself as a WeakPtr<Element>.
		=============================================================================*/
		WeakPtr<Element> Element::_GetAsWeakPtr()
		{
			if (!mParent.expired())
				return mParent.lock()->GetChild(GetId());
			else
				return GetRoot()->GetChild(GetId());
		}


		/*=============================================================================
		-- Adds a child element to the child list for this element.
		=============================================================================*/
		void Element::_AddChild(WeakPtr<Element> child)
		{
			mChildren.push_back(child.lock());
		}


		/*=============================================================================
		-- Directly removes the child with the corresponding id in this element's
		   children list. Use with caution while iterating through the Root's tree.
		=============================================================================*/
		void Element::_RemoveChild(int id)
		{
			std::vector< SharedPtr<Element> >::iterator iter = mChildren.begin();
			while (iter != mChildren.end())
			{
				if ( (*iter)->GetId() == id )
				{
					mChildren.erase(iter);
					return;
				}

				iter++;
			}
		}


		void Element::_SetAllBelow_HasFocus(bool hasFocus)
		{
			std::vector< SharedPtr<Element> >::iterator iter = mChildren.begin();
			while (iter != mChildren.end())
			{
				(*iter)->_SetHasFocus(hasFocus);
				(*iter)->_SetAllBelow_HasFocus(hasFocus);

				iter++;
			}
		}



		/*=============================================================================
		-- Sets the relative position and resets the screen position.
		=============================================================================*/
		void Element::SetRelPos(Vector2D<int> pos)
		{
			mRelPos = pos;
			_UpdateScreenPos();
		}


		/*=============================================================================
		-- Calculates the new relative position to get to the screen position specified
		   and then recalculates the screen position.
		=============================================================================*/
		void Element::SetScreenPos(Vector2D<int> pos)
		{
			if (!mParent.expired())
				mRelPos = pos - mParent.lock()->GetScreenPos();
			else
				mRelPos = pos;

			_UpdateScreenPos();
		}


		void Element::_BringToLocalFront()
		{
			if (!mParent.expired())
			{
				SharedPtr<Element> sharedPtrThis = _GetAsWeakPtr().lock();
				mParent.lock()->_RemoveChild(GetId());
				mParent.lock()->_AddChild(sharedPtrThis);	
				_SetAllBelow_HasFocus(false);
			}
		}


		void Element::_BringParentsToLocalFront()
		{
			if (!mParent.expired())
			{
				mParent.lock()->_BringToLocalFront();
				mParent.lock()->_BringParentsToLocalFront();
				_SetAllBelow_HasFocus(false);  //TODO this is pretty hefty on the CPU (well not really, but it is done an unessecessary number of times)
			}
		}


		//never call in any Element or derived class methods that are called during _Update() or _Draw() (will screw up element order in vector storage)
		void Element::BringToFocus()
		{
			_BringToLocalFront();
			_BringParentsToLocalFront();
			_SetHasFocus(true);
			_SendMessage(HAS_FOCUS);
		}


		/*=============================================================================
		-- Recursively brings all elements scheduled to be brought to focus, to focus.
		   It returns false if an element had been brought to focus, because the
		   children have been rearranged. This means the recursive tree has to be
		   restarted completely from wherever it orriginated.
		=============================================================================*/
		bool Element::_BringAllChildrenScheduledToBeBroughtToFocusToFocus()
		{
			std::vector< SharedPtr<Element> >::iterator iter = mChildren.begin();
			while (iter != mChildren.end())
			{
				if ((*iter)->_GetScheduledToBringToFocus())
				{
					(*iter)->_SetScheduledToBringToFocus(false);
					(*iter)->BringToFocus();
					return false;
				}

				if ((*iter)->_BringAllChildrenScheduledToBeBroughtToFocusToFocus() == false)
					return false;

				iter++;
			}

			return true;
		}


		void Element::_RemoveAllChildrenScheduledToBeRemoved()
		{
			std::vector< SharedPtr<Element> >::iterator iter = mChildren.begin();
			while (iter != mChildren.end())
			{
				if ((*iter)->_GetScheduledToBeRemoved())
				{
					RemoveChild(*iter);
					return;
				}

				(*iter)->_RemoveAllChildrenScheduledToBeRemoved();

				iter++;
			}
		}
	};
};
