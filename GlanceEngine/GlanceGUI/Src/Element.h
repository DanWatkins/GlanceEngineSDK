/*=================================================================================================
Glance Engine - (C) 2010-2013 Daniel L. Watkins

Filename: Element.h
=================================================================================================*/

#include "GUI.h"

#ifndef _ge_ELEMENT_H
#define _ge_ELEMENT_H

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- The core functionality for all GUI elements.
		=============================================================================*/
		class Element
		{
		private:
			int mId;											//unique id of the Element
			Root *mRoot;										//pointer to the Root manager (can't use weak reference because the Root can't instantiate itself as a smart pointer)
			WeakPtr<Element> mParent;							//weak reference to the parent
			Window *mWindow;									//pointer to the window where everything is rendered

			State mState;										//state the element is in
			bool mDisabled;										//if true, this element is not usuable, and usually has a different set of images to show it is disabled
			bool mHasFocus;										//true if this Element is in focus
			bool mDraws;										//if false, this elements and its children will not be drawn
			bool mSolid;										//if false, clicking events go right through this element to the one below
			bool mCanFocus;

			bool mScheduledToBringToFocus;						//if true, the Root will call BringToFocus on this, but will only have priority based on its position in the array storage
			bool mScheduledToBeRemoved;

			//TODO can't the Root just store a pointer to the window and every element just references it? instead of every Element having a Window pointer
			Vector2D<int> mScreenPos;							//absolute screen position (updated whenever @mRelPos changes or the parent changes)
			Vector2D<int> mRelPos;								//releative position to the parent
			unsigned mWidth, mHeight;							//width and height of the element (not necessarily visually)

		protected:
			std::vector< SharedPtr<Element> > mChildren;		//direct children of this Element. Children are only stored in there parents

			bool _Init(int id, WeakPtr<Element> parent, Root *root, Vector2D<int> pos, unsigned width, unsigned height, Window *window);

			//virtual core methods
			virtual bool _PostInit() { return true; }
			virtual void _Update() {}
			virtual void _Draw() {}
			virtual void _SendMessage(Message) {}
			virtual void _UpdateAssets() {}

			void _UpdateScreenPos(bool goUptree=false);
			void _AddChild(WeakPtr<Element> child);
			void _RemoveChild(int id);
			void _SetAllBelow_HasFocus(bool hasFocus);
			void _SetHasFocus(bool hasFocus) { mHasFocus = hasFocus; }

			void _SetScheduledToBringToFocus(bool scheduledBringToFocus) { mScheduledToBringToFocus = scheduledBringToFocus; }
			bool _GetScheduledToBringToFocus() { return mScheduledToBringToFocus; }
			void _SetScheduledToBeRemoved(bool scheduledToBeRemoved) { mScheduledToBeRemoved = scheduledToBeRemoved; }
			bool _GetScheduledToBeRemoved() { return mScheduledToBeRemoved; }

			void _UpdateChildren();
			void _DrawChildren();
			WeakPtr<Element> _GetElementSearch(unsigned id);
			WeakPtr<Element> _GetAsWeakPtr();
			void _SetWindow(Window *window) { mWindow = window; }
			void _SetState(State state) { mState = state; }
			bool _ContainsPoint(Vector2D<int> point);
			std::vector< SharedPtr<Element> > _GetAllDerivedElementsWithIntersectingPoint(Vector2D<int> point);
			bool _BringAllChildrenScheduledToBeBroughtToFocusToFocus();
			void _RemoveAllChildrenScheduledToBeRemoved();
			void _SetSize(unsigned width, unsigned height);

			void _BringToLocalFront();
			void _BringParentsToLocalFront();
			void _ScheduleToBringToFocus() { _SetScheduledToBringToFocus(true); }

		public:
			friend class Root;

			Element();
			~Element() {}

			WeakPtr<Element> GetChild(int id);
			void BringToFocus();
			void RemoveChild(WeakPtr<Element> element) { _RemoveChild(element.lock()->GetId()); }
			void ScheduleToBeRemoved() { _SetScheduledToBeRemoved(true); }

			//accessor and mutator
			int GetId() { return mId; }
			State GetState() { return mState; }
			WeakPtr<Element> GetParent() { return mParent; }
			Root *GetRoot() { return mRoot; }
			void SetDisabled(bool disabled) { mDisabled = disabled; }
			bool GetDisabled() { return mDisabled; }
			int GetChildCount() { return mChildren.size(); }

			void SetRelPos(Vector2D<int> pos);
			Vector2D<int> GetRelPos() { return mRelPos; }
			void SetScreenPos(Vector2D<int> pos);
			Vector2D<int> GetScreenPos() { return mScreenPos; }
			unsigned GetWidth() { return mWidth; }
			unsigned GetHeight() { return mHeight; }
			void SetSize(unsigned width, unsigned height) { mWidth = width; mHeight = height; }
			Window *GetWindow() { return mWindow; }

			bool GetHasFocus() { return mHasFocus; }
			void SetDraws(bool draws) { mDraws = draws; }
			bool GetDraws() { return mDraws; }
			void SetSolid(bool solid) { mSolid = solid; }
			bool GetSolid() { return mSolid; }
			void SetCanFocus(bool canFocus) { mCanFocus = canFocus; }
			bool GetCanFocus() { return mCanFocus; }
		};
	};
};

#endif

