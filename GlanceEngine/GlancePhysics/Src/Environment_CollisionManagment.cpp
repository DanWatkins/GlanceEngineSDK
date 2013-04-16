/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Environment_CollisionManagment.cpp
=================================================================================================*/
#include "Physics.h"

namespace ge
{
    namespace physics
    {
		/*=============================================================================
		-- Moves @body the amount its velocity demands. Then checks for collision
		   appropriately. It appends the collision data to @collisions, then resets
		   the velocity of @body back to zero.
		=============================================================================*/
		void Environment::_ManageCollisions_Std(WeakPtr<Body> body, std::vector<Collision>& collisions)
		{
			body.lock()->_ApplyVelocity();
			std::vector<Collision> col = _CheckAndHandleAllCollisions(body.lock());
			collisions.insert(collisions.end(), col.begin(), col.end());
		}


		/*=============================================================================
		-- Moves @body a calculated distance less than @body's velocity demands. This
		   is because the normal velocity is too fast for collision checks to catch
		   it moving "through" other Body objects. It then checks for collision
		   appropriately. It appends the collision data to @collisions, then resets
		   the velocity of @body back to zero. This whole process is repeated until
		   @body has moved as much as its velocity demanded it move. Do note that
		   @collisions will contain Collision objects that have innaccurate distance
		   values. This is because duplicate collision records are deleted, and the
		   distance is not added to the remaining instance.
		=============================================================================*/
		void Environment::_ManageCollisions_Inc(WeakPtr<Body> body, std::vector<Collision>& collisions)
		{
			SweepBox sweepBox;
			bool increment = true;

			//see if any objects are in the path of @iter
			if (body.lock()->GetType() == BOX)
			{
				//if the sweep box collided with anything, we have to move the Body in increments
				if (_CheckAndHandleAllCollisions( DynamicPtrCast<Box>(body.lock())->GetSweepBox() ).size() > 0)
				{
					int cycles = 0;
					double base = 0.0;
					Vector3D<double> vel( body.lock()->GetVelocity() );

					//get the base (the largest 
					if (std::abs(vel.x) > std::abs(vel.y))
					{
						if (std::abs(vel.x) > std::abs(vel.z))
							base = std::abs(vel.x);
						else
							base = std::abs(vel.z);
					}
					else
					{
						if (std::abs(vel.y) > std::abs(vel.z))
							base = std::abs(vel.y);
						else
							base = std::abs(vel.z);
					}

					//calculate the number of times to apply the incremental velocity
					cycles = RoundUp( DivideD(base, mMaxVelocityStep) );

					//calculate the incremental velocity
					vel.x = DivideD(vel.x, cycles);
					vel.y = DivideD(vel.y, cycles);
					vel.z = DivideD(vel.z, cycles);
					body.lock()->_SetVelocity(vel);

					//move the Body the incremental velocity n times equivalent to @cycles and check and handle all collisions each time
					for (int n=0; n<cycles; n++)
					{
						body.lock()->_ApplyVelocity();
						std::vector<Collision> col = _CheckAndHandleAllCollisions(body.lock());

						//filter any duplicate collision records that may have appeared
						std::vector<Collision>::iterator iter1, iter2;
						iter1 = collisions.begin();
						while (iter1 != collisions.end())
						{
							bool passed = false;
							while (!passed)
							{
								passed = true;
								iter2 = col.begin();

								while (iter2 != col.end())
								{
									if ( iter1->GetB1() == iter2->GetB1()  &&  iter1->GetB2() == iter2->GetB2() )
									{
										col.erase(iter2);
										passed = false;
										break;
									}

									iter2++;
								}
							}

							iter1++;
						}

						collisions.insert(collisions.end(), col.begin(), col.end());
					}

					//reset the Body's velocity so it doesn't accumulate
					body.lock()->_ResetVelocity();
				}
				else
					increment = false;
			}

			//if the Body was not moved in increments
			if (!increment)
			{
				body.lock()->_ApplyVelocity();
				std::vector<Collision> col = _CheckAndHandleAllCollisions(body.lock());
				collisions.insert(collisions.end(), col.begin(), col.end());
				body.lock()->_ResetVelocity();
			}
		}


		/*=============================================================================
		-- Checks and handles all collisions for @body against every other Body. It
		   returns true if at least one collision is found.
		=============================================================================*/
		std::vector<Collision> Environment::_CheckAndHandleAllCollisions(SharedPtr<Body> body)
		{
			std::vector< Collision > collisions;

			//check for collision with every other entity
			std::vector< SharedPtr<Body> >::iterator test = mCurrentBodyPartition->begin();
			while (test != mCurrentBodyPartition->end())
			{
				//make sure the test Body is active
				if ( (*test)->GetActive() )
				{
					//check for collision between @body and @test
					Collision collision = _CheckCollision(body, *test);
					if (collision.AreColliding())
						collisions.push_back(collision);  //add the collision result to the list of collisions to be handled later
				}

				test++;
			}

			//order the list of collisions from the closest Body to the farthest body using a bubblesort
			if (collisions.size() > 1)
			{
				std::vector< Collision >::iterator c1 = collisions.begin();
				std::vector< Collision >::iterator c2 = collisions.begin();
				c2++;
				int n = 1;

				//if it is 1 or greater, that means there was at least one rearrangment in the pass
				while (n > 0)
				{
					n = 0;
					while (c2 != collisions.end())
					{
						if (c2->GetDistance() < c1->GetDistance())
						{
							c1->Swap(*c2);
							n++;
						}

						c1++;
						c2++;
					}
				}
			}

			//handle collisions that have been ordered from priority (distance from iter)
			std::vector< Collision >::iterator handle = collisions.begin();
			while (handle != collisions.end())
			{
				if (body->GetId() == 0)
					int n = 99;

				_HandleCollision(*handle);
				handle++;
			}

			return collisions;
		}
	};
};