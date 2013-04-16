/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Environment.h
=================================================================================================*/
#include "Physics.h"

#ifndef _ge_ENVIRONMENT_H
#define _ge_ENVIRONMENT_H

namespace ge
{
    namespace physics
    {
		/*=============================================================================
		-- Represents a partitioned chunk of the world. It holds a list of the bodies
		   in its region.
		=============================================================================*/
		struct Partition
		{
			bool optimal;
			std::vector< SharedPtr<Body> > bodies;   //use shared pointer because it is faster to copy from original list
			double width, height;
			bool splitVerticallyLast;
			bool allBodiesAtRest;
			Vector2D<double> pos;

			Partition()
			{
				optimal				= false;
				width				= 0.0;
				height				= 0.0;
				splitVerticallyLast	= false;
				allBodiesAtRest		= true;
			}
		};


		/*=============================================================================
		-- Environment to simulate interactions between different Body objects. Another
		   entity system, such as world::EntityManager can assign references from
		   Body objects to its own entities.
		=============================================================================*/
		class Environment
		{
		private:
			double mMaxVelocityStep;								//maximum number of units (usually pixels) to move any Body along the x or y axis before checking for collision and handleing it
			double mMinBodySize;									//minimum dimension for any Body added to the Environment. Body objects failing this will be adjusted to meet the standards
			std::vector< SharedPtr<Body> > mBodies;					//list of every Body in the Environment
			std::vector< SharedPtr<Body> > *mCurrentBodyPartition;	//current partition of bodies being updated
			int mIdTrack;											//used to make every Body have a unique id
			double mWidth, mLength, mHeight;						//boundaries of environment, used for partitioning the collision testing
			double mBlockSize;										//size of partition cubes (measured in pixels). end blocks may vary in size
			unsigned mMaxBodiesPerPartition;						//maximum number of bodies per partition

			//body factory
			WeakPtr<Body> _CreateBoxFromBox(WeakPtr<Box> box);

			std::vector< std::vector< SharedPtr<Body> > > _GenerateBlocks();
			std::vector< SharedPtr<Partition> > _DividePartition(SharedPtr<Partition> partition);

			//broad collision management
			void _ManageCollisions_Std(WeakPtr<Body> body, std::vector<Collision>& collisions);   //standard collision management for a body
			void _ManageCollisions_Inc(WeakPtr<Body> body, std::vector<Collision>& collisions);   //incremental collisions managment for a body (it is moving too fast)
			std::vector<Collision> _CheckAndHandleAllCollisions(SharedPtr<Body> body);

			//specific collision test
			bool _CheckCollisionAxisXY(Box &b1, Box &b2);
			Collision _CheckCollision(SharedPtr<Body> b1, SharedPtr<Body> b2);
			Collision _CheckCollision(SharedPtr<Box> b1, SharedPtr<Box> b2);
			void _DetermineCollisionResponse(SharedPtr<Box> b1, SharedPtr<Box> b2, Collision &collision);
			void _HandleCollision(Collision collision);

			//gravity related
			void _UpdateGravitationalVelocity(WeakPtr<Body> body);


		public:
			Environment();
			~Environment() {}

			//general
			std::vector<Collision> Update();
			void ClearBodies() { mBodies.clear(); }

			//Body access
			WeakPtr<Body> GetBody(int id);
			WeakPtr<Box> GetBox(int id);

			//Body factories
			WeakPtr<Body> CreateBody(WeakPtr<Body> body, Vector3D<double> pos = Vector3D<double>(0.0, 0.0, 0.0), int hostId=-1);
			WeakPtr<Body> CreateBox(double width, double length, double height, Vector3D<double> pos = Vector3D<double>(0.0, 0.0, 0.0), int hostId=-1, Vector3D<double> offset=Vector3D<double>(0.0, 0.0, 0.0));
			WeakPtr<Body> CreateCircle(double radius, Vector3D<double> pos = Vector3D<double>(0.0, 0.0, 0.0), int hostId=-1);
			

			//accessor and mutator
			void SetMaxVelocityStep(double mvs) { mMaxVelocityStep = mvs; }

			void SetWidth(double width) { mWidth = width; }
			double GetWidth() { return mWidth; }
			void SetLength(double length) { mLength = length; }
			double GetLength() { return mLength; }
			void SetHeight(double height) { mHeight = height; }
			double GetHeight() { return mHeight; }
			void SetSize(double width, double height) { SetWidth(width); SetHeight(height); }

			void SetBlockSize(double blockSize) { mBlockSize = blockSize; }
			double GetBlockSize() { return mBlockSize; }
		};
	};
};

#endif

