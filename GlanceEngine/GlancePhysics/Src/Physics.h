/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Physics.h
=================================================================================================*/
#include "../../GlanceCore/Src/Glance.h"

#ifndef _ge_PHYSICS_H
#define _ge_PHYSICS_H


namespace ge
{
    namespace physics
    {
		//forward declarations
		class Body;
		class Box;
		class Circle;
		class SweepBox;
		
		const double MARGIN_OF_ERROR = 0.000000000001;
		const double ACCELERATION_DUE_TO_GRAVITY_IN_PPS = 60.0;
	};
};



//inlcudes
#include "Body.h"
#include "Body/SweepBox.h"
#include "Body/Box.h"
#include "Body/Cylinder.h"
#include "Collision.h"
#include "Environment.h"


#endif

