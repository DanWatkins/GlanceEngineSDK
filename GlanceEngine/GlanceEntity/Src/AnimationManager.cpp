/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: AnimationManager.cpp
=================================================================================================*/
#include "Constants.h"

namespace ge
{
	namespace world
	{
		/*=============================================================================
		-- Performs a deep copy on every animation contained in the manager. Without
		   doing a deep copy, copies of this animation manager would be controlling the
		   same animations, which is not desired.
		=============================================================================*/
		AnimationManager& AnimationManager::_Copy(const AnimationManager& animationManager)
		{
			//deep copy every Animation
			std::vector< SharedPtr<Animation> >::const_iterator iter = animationManager.mAnimations.begin();
			while (iter != animationManager.mAnimations.end())
			{
				mAnimations.push_back( SharedPtr<Animation>(new Animation(*iter->get())) );
				iter++;
			}

			if (mAnimations.size() > 0)
				mActiveAnimation = mAnimations.front();

			return *this;
		}


		/*=============================================================================
		-- Constructor for AnimationManager.
		=============================================================================*/
		AnimationManager::AnimationManager(const AnimationManager& animationManager)
		{
			_Copy(animationManager);
		}

		AnimationManager& AnimationManager::operator= (const AnimationManager& animationManager)
		{
			return _Copy(animationManager);
		}


		/*=============================================================================
		-- Adds the animation to the manager, but only if there is not another
		   animation with the same type identifier.
		=============================================================================*/
		void AnimationManager::AddAnimation(SharedPtr<Animation> animation)
		{
			//make sure there is not already an animation with the same type
			bool uniqueType = true;
			std::vector< SharedPtr<Animation> >::iterator iter = mAnimations.begin();
			while (iter != mAnimations.end())
			{
				if ((*iter)->GetType() == animation->GetType())
				{
					uniqueType = true;
					break;
				}

				iter++;
			}

			if (uniqueType)
			{
				mAnimations.push_back(animation);
			
				//set the active animation to this new animation if there is not a active one
				if (mActiveAnimation.expired())
					mActiveAnimation = animation;
			}
		}


		/*=============================================================================
		-- Switches the active animation to an animation with type identifier @type if
		   there is an animation with that type identifier.
		=============================================================================*/
		void AnimationManager::SwitchToAnimation(String type)
		{
			std::vector< SharedPtr<Animation> >::iterator iter = mAnimations.begin();
			while (iter != mAnimations.end())
			{
				if ((*iter)->GetType() == type)
				{
					int oldRotation = mActiveAnimation.lock()->GetCurrentRotation();
					mActiveAnimation = *iter;
					mActiveAnimation.lock()->SetCurrentRotation(oldRotation);
				}

				iter++;
			}
		}
	};
};