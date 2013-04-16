/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: AnimationManager.h
=================================================================================================*/
#include "Constants.h"

#ifndef _ge_ANIMATION_MANAGER_H
#define _ge_ANIMATION_MANAGER_H

namespace ge
{
	namespace world
	{
		/*=============================================================================
		-- Handles the logistics of animations (loading animations and switching
		   between them).
		=============================================================================*/
		class AnimationManager
		{
		private:
			std::vector< SharedPtr<Animation> > mAnimations;
			WeakPtr<Animation> mActiveAnimation;

		protected:
			AnimationManager& _Copy(const AnimationManager& animationManager);

		public:
			AnimationManager() {}
			AnimationManager(const AnimationManager& animationManager);
			~AnimationManager() {}
			AnimationManager& operator= (const AnimationManager& animationManager);

			void AddAnimation(SharedPtr<Animation> animation);					
			void SwitchToAnimation(String type);
			WeakPtr<Animation> GetActiveAnimation() { return mActiveAnimation; }
		};
  };
};

#endif

