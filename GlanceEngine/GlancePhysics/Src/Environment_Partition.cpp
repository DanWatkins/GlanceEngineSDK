/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Environment_Partition.cpp
  Created: 1/9/13 by Dan Watkins
  Notes: -Moved partition related code over from Environment.cpp (1/9/13)
=================================================================================================*/
#include "Physics.h"

namespace ge
{
    namespace physics
    {
		/*=============================================================================
		-- Returns true if every body in @group1 has the same id as another body in
		   @group2 and vice versa.
		=============================================================================*/
		bool _BodiesAreIdentical(std::vector< SharedPtr<Body> > group1, std::vector< SharedPtr<Body> > group2)
		{
			//make sure both groups have the same size
			if (group1.size() != group2.size())
				return false;

			//try and find a match for every body in the other list
			bool foundConflict = false;
			std::vector< SharedPtr<Body> >::iterator iter1 = group1.begin();

			while (iter1 != group1.end())
			{
				bool foundMatch = false;

				std::vector< SharedPtr<Body> >::iterator iter2 = group2.begin();
				while (iter2 != group2.end())
				{
					if ((*iter1)->GetId() == (*iter2)->GetId())
						foundMatch = true;

					iter2++;
				}

				foundConflict = !foundMatch;
				iter1++;
			}

			return !foundConflict;
		}


		/*=============================================================================
		-- Divides a Partition into two halves. It is split vertically or horizontally
		   depending on how the partition was split from its parent partition. In some
		   cases, only one have will be returned because the other contained no bodies.
		=============================================================================*/
		std::vector< SharedPtr<Partition> > Environment::_DividePartition(SharedPtr<Partition> partition)
		{
			//determine the center value to split the bodies across
			double centerValue = 0.0;
			if (partition->splitVerticallyLast)
				centerValue = partition->height / 2.0;
			else
				centerValue = partition->width / 2.0;

			//setup the two new partitions (they are identical initially)
			SharedPtr<Partition> partitionA(new Partition);
			if (partition->splitVerticallyLast)
			{
				partitionA->height = partition->height / 2.0;
				partitionA->width = partition->width;
			}
			else
			{
				partitionA->height = partition->height;
				partitionA->width = partition->width / 2.0;
			}

			partitionA->splitVerticallyLast = !partition->splitVerticallyLast;
			partitionA->pos = partition->pos;
			SharedPtr<Partition> partitionB(new Partition(*partitionA.get()));

			//add @centerValue to the B partition because it needs to
			//be offset from the position it got from the A partition
			if (partition->splitVerticallyLast)
				partitionB->pos.y += centerValue;
			else
				partitionB->pos.x += centerValue;

			//determine which half each body belongs in
			std::vector< SharedPtr<Body> > borderBodies;	//bodies in both partitions
			std::vector< SharedPtr<Body> >::iterator iter = partition->bodies.begin();
			bool allAtRestPartitionA=true, allAtRestPartitionB=true;

			while (iter != partition->bodies.end())
			{
				Vector3D<double> bodyPos = (*iter)->GetPos();
				Box bodyAsBox = *((Box*)(*iter).get());
				int partitionsAddedTo = 0;

				if (partition->splitVerticallyLast)
				{
					if (bodyPos.y < partitionB->pos.y)
					{
						partitionA->bodies.push_back(*iter);
						partitionsAddedTo++;
					}
					
					if (bodyPos.y >= partitionB->pos.y)
					{
						partitionB->bodies.push_back(*iter);
						partitionsAddedTo++;
					}
					else if (bodyPos.y+bodyAsBox.GetLength() >= partitionB->pos.y)
					{
						partitionB->bodies.push_back(*iter);
						partitionsAddedTo++;
					}
				}
				else
				{
					if (bodyPos.x < partitionB->pos.x)
					{
						partitionA->bodies.push_back(*iter);
						partitionsAddedTo++;
					}
					
					if (bodyPos.x >= partitionB->pos.x)
					{
						partitionB->bodies.push_back(*iter);
						partitionsAddedTo++;
					}
					else if (bodyPos.x+bodyAsBox.GetWidth() >= partitionB->pos.x)
					{
						partitionB->bodies.push_back(*iter);
						partitionsAddedTo++;
					}
				}

				//was this a border body?
				if (partitionsAddedTo == 2)
					borderBodies.push_back(*iter);

				//was this body moving?
				if (allAtRestPartitionA || allAtRestPartitionB)
				{
					if ((*iter)->GetVelocity() != Vector3D<double>())
					{
						if (partitionA->bodies.back()->GetId() == (*iter)->GetId())
							partitionA->allBodiesAtRest = allAtRestPartitionA = false;
						if (partitionB->bodies.back()->GetId() == (*iter)->GetId())
							partitionB->allBodiesAtRest = allAtRestPartitionB = false;
					}
				}

				iter++;
			}


			//check if the initial partition was as optimal as possible
			//meaning it cannot be borken down any further even though
			//it has more bodies than @mMaxBodiesPerPartition
			if (partitionA->bodies.size() == partition->bodies.size() &&
				partitionB->bodies.size() == partition->bodies.size() )
			{
				//it was optimal, so set A to be optimal and toss B
				//since they have the same bodies
				partitionA->optimal = true;
				partitionB->bodies.clear();
			}


			//does either partition consist entirely of border bodies?
			if (_BodiesAreIdentical(partitionA->bodies, borderBodies))
				partitionA->bodies.clear();
			else if (_BodiesAreIdentical(partitionB->bodies, borderBodies))
				partitionB->bodies.clear();


			//check if either partition is optimal because it has
			//a body count not exceeding @mMaxBodiesPerPartition
			partitionA->optimal = (partitionA->bodies.size() <= mMaxBodiesPerPartition);
			partitionB->optimal = (partitionB->bodies.size() <= mMaxBodiesPerPartition);				

			//create a vector of both new partitions and return it
			//the partitions have to have bodies in them
			std::vector< SharedPtr<Partition> > partitions;
			if (partitionA->bodies.size() > 0)
				partitions.push_back(partitionA);
			if (partitionB->bodies.size() > 0)
				partitions.push_back(partitionB);

			return partitions;
		}


		/*=============================================================================
		-- Generates a list of lists of bodies for each partitioned block that will
		   have collisions tested independently of the other lists. SharedPtr<> is used
		   for the storage of bodeis because it would not be efficient to convert from
		   SharedPtr<> (what every body is stored as in the main body list) to
		   WeakPtr<>.
		=============================================================================*/
		std::vector< std::vector< SharedPtr<Body> > > Environment::_GenerateBlocks()
		{
			std::vector< std::vector< SharedPtr<Body> > > blocks;
			std::vector< SharedPtr<Partition> > partitions;

			//make the initial partition that contains the entire environment
			SharedPtr<Partition> partition(new Partition);
			partition->bodies = mBodies;
			partition->width = GetWidth();
			partition->height = GetHeight();

			partitions.push_back(partition);

			bool keepChecking = true;
			while (keepChecking)
			{
				//try and find a reason in the following loop 
				//to need to check everything over again
				keepChecking = false;

				//iterate through every partition to see if it should be
				//divided or added to the list of blocks
				std::vector< SharedPtr<Partition> >::iterator iter = partitions.begin();
				while (iter != partitions.end())
				{
					if ((*iter)->optimal == false  &&  partition->allBodiesAtRest == false)
					{
						//divide the partition, and add the two new partitions to
						//@partitions, and remove the current @iter from @partitions
						std::vector< SharedPtr<Partition> > dividedPartition = _DividePartition(*iter);
						partitions.erase(iter);
						partitions.insert(partitions.end(), dividedPartition.begin(), dividedPartition.end());
						iter = partitions.begin();
						keepChecking = true;
					}
					else
					{
						//this partition is optimal, so add the body list to the blocks list
						//also remove the partition from the list of partitions
						blocks.push_back((*iter)->bodies);
						partitions.erase(iter);
						iter = partitions.begin();
					}

				}
			}

			return blocks;
		}
	};
};