/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Environment_CollisionCheck.cpp
=================================================================================================*/
#include "Physics.h"

namespace ge
{
    namespace physics
    {
		/*=============================================================================
		-- Determines which overloaded _CheckCollision() method to call based on the
		   type of the two Body objects.
		=============================================================================*/
		Collision Environment::_CheckCollision(SharedPtr<Body> b1, SharedPtr<Body> b2)
		{
			if ( b1->GetType() == BOX )
			{
				//Box to Box
				if (b2->GetType() == BOX)
					return _CheckCollision(DynamicPtrCast<Box>(b1), DynamicPtrCast<Box>(b2) );
			}
			else if (b1->GetType() == SWEEPBOX)
			{
				SharedPtr<SweepBox> sweepBox1 = DynamicPtrCast<SweepBox>(b1);
				SharedPtr<Box> tempBox1( new Box(sweepBox1) );

				//SweepBox to Box
				if (b2->GetType() == BOX)
					return _CheckCollision(tempBox1, DynamicPtrCast<Box>(b2));

				//SweepBox to SweepBox
				else if (b2->GetType() == SWEEPBOX)
				{
					SharedPtr<SweepBox> sweepBox2 = DynamicPtrCast<SweepBox>(b2);
					SharedPtr<Box> tempBox2( new Box(sweepBox2) );
					return _CheckCollision(tempBox1, tempBox2);
				}
			}

			return false;
		}


		/*=============================================================================
		-- Returns true if the two boxes are intersecting on the x and y axes.
		=============================================================================*/
		bool Environment::_CheckCollisionAxisXY(Box &b1, Box &b2)
		{
			if (b1.GetPos().x+b1.GetWidth() < b2.GetPos().x)//left of @b2
				return false;
			if (b1.GetPos().x > b2.GetPos().x+b2.GetWidth())//right of @b2
				return false;
			if (b1.GetPos().y+b1.GetLength() < b2.GetPos().y)//top of @b2
				return false;
			if (b1.GetPos().y > b2.GetPos().y+b2.GetLength())//bottom of @b2
				return false;

			return true;
		}


		/*=============================================================================
		-- Returns true if the two boxes are intersecting on the z-axis.
		=============================================================================*/
		bool _CheckCollisionAxisZ(SharedPtr<Box> b1, SharedPtr<Box> b2)
		{
			if (b1->GetPos().z+b1->GetHeight() <= b2->GetPos().z)//TODO may need to include ==?
				return false;
			if (b1->GetPos().z >= b2->GetPos().z+b2->GetHeight())//TODO is inclusive appropriate here? it makes objects at ground level movable
				return false;

			return true;
		}


		/*=============================================================================
		-- Determines how a collision between the two bodies should be resolved, and
		   saves the resolution information in @collision. Note that it is assumed that
		   b1 and b2 are colliding, and that b1 is moving.
		=============================================================================*/
		void Environment::_DetermineCollisionResponse(SharedPtr<Box> b1, SharedPtr<Box> b2, Collision &collision)
		{
			//was b1 intersecting b2 on the z-axis before its velocity was applied?
			b1->_AddZ(-b1->GetVelocity().z);
			bool wereIntersectingOnZ = _CheckCollisionAxisZ(b1, b2);
			b1->_AddZ(b1->GetVelocity().z);

			//if they were intersecting on the z, only perform a XY resolution (if applicable)
			if (wereIntersectingOnZ)
			{
				Vector2D<double> result(0.0, 0.0);
				double difference = 0.0;
				double minimumTranslation = 0.0;
				int axis = 0;
				int side = -1;
				bool didX = false;

				//which face of @b2 did @b1 overlap the most?
				//x negative
				if (b1->GetVelocity().x != 0.0)
				{
					minimumTranslation = b1->GetPos().x+b1->GetWidth() - b2->GetPos().x;
					axis = 0;
					
					//x positive
					difference = b2->GetPos().x+b2->GetWidth() - b1->GetPos().x;
					if (LessThan(difference, minimumTranslation, MARGIN_OF_ERROR))
					{
						minimumTranslation = difference;
						axis = 0;
						side = 1;
					}

					didX = true;
				}

				//Y-Axis
				if (b1->GetVelocity().y != 0.0)
				{
					//y negative
					difference = b1->GetPos().y+b1->GetLength() - b2->GetPos().y;

					if (!didX)
					{
						minimumTranslation = difference;
						axis = 1;
					}

					if (LessThan(difference, minimumTranslation, MARGIN_OF_ERROR))
					{
						minimumTranslation = difference;
						axis = 1;
						side = -1;
					}
					//y positive
					difference = b2->GetPos().y+b2->GetLength() - b1->GetPos().y;
					if (LessThan(difference, minimumTranslation, MARGIN_OF_ERROR))
					{
						minimumTranslation = difference;
						axis = 1;
						side = 1;
					}
				}

				//there was a collision
				if (axis == 0)
					collision.SetResolveX( (minimumTranslation+MARGIN_OF_ERROR) * side );
				else
					collision.SetResolveY( (minimumTranslation+MARGIN_OF_ERROR) * side );
			}
			//if not, just perform a z resolution
			else
			{
				if (b1->GetVelocity().z > 0.0)
					collision.SetResolveZ( -(b1->GetPos().z+b1->GetHeight() - b2->GetPos().z) );
				else
					collision.SetResolveZ( -(b1->GetPos().z - (b2->GetPos().z+b2->GetHeight())) );
			}
		}


		/*=============================================================================
		-- Checks for a collision between the two Box objects. Only @b1 is considered
		   to be moving: even if @b2 is in fact moving.
		=============================================================================*/
		Collision Environment::_CheckCollision(SharedPtr<Box> b1, SharedPtr<Box> b2)
		{
			//make sure @b1 isn't @b2
			if (b1 == b2  ||  b1->GetId() == b2->GetId())
				return Collision(false);


			//b1 and b2 will only be colliding if they have collisions on the x, y, and z axis

			//check if b1 and b2 are too far away to be colliding on either the x or y axis from either bound
			if (b1->GetPos().x+b1->GetWidth() < b2->GetPos().x)//left of @b2
				return Collision(false);
			if (b1->GetPos().x > b2->GetPos().x+b2->GetWidth())//right of @b2
				return Collision(false);
			if (b1->GetPos().y+b1->GetLength() < b2->GetPos().y)//top of @b2
				return Collision(false);
			if (b1->GetPos().y > b2->GetPos().y+b2->GetLength())//bottom of @b2
				return Collision(false);

			//b1 and b2 are colliding on the x and y axis, so see if b1 is resting on top of b2
			//it will be considered resting if the bottom of b1 has a position 1 pixel above
			//the top of b2
			if (b1->GetPos().z+b1->GetHeight() <= b2->GetPos().z)//TODO may need to include ==?
				return Collision(false);
			if (b1->GetPos().z >= b2->GetPos().z+b2->GetHeight())//TODO is inclusive appropriate here? it makes objects at ground level movable
				return Collision(false);

			//is b2 supporting b1?
			if (b1->GetPos().z <= b2->GetPos().z+b2->GetHeight())
				b1->_SetFoundSupportingBodyDuringUpdate(true);

			//there are no cases where they cannot be colliding, so they must be colliding
			Collision collision(true, b1, b2);

			return collision;
		}
	};
};