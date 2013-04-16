/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Entity.h
=================================================================================================*/
#include "../../GlancePhysics/Src/Physics.h"
#include "../../GlanceAsset/Src/Asset.h"

//use the define to break out a cyclic dependency between world and entity
//but also let world include the entity declaration
#ifndef _ge_ENTITY_INC
#define _ge_ENTITY_INC
#include "../../GlanceWorld/Src/World.h"
#endif


#ifndef _ge_ENTITY_CONSTANTS_H
#define _ge_ENTITY_CONSTANTS_H

namespace ge
{
	namespace world
	{
		const int UNDEFINED_ENTITY_ID = -1;
		const int TEMPLATE_ENTITY_ID = -2;

		//forward declarations
		class Entity;
		class Actor;
		class Inventory;
		class Player;

		class Object;
		class Ammo;
		class Weapon;
		
		class EntityLoader;
		class EntityManager;

		class Animation;
		class AnimationManager;

		class World;

        /*=============================================================================
        -- Entity type enumeration.
        =============================================================================*/
        enum EntityType
        {
            UNDEFINED = 0,
            ACTOR = 1,
            OBJECT = 2
        };
	};
};



//inlcudes
#include "Animation.h"
#include "AnimationManager.h"
#include "Inventory/InventoryGroup.h"
#include "Inventory/Inventory.h"
#include "Entities/Actor.h"
#include "Player.h"

#include "Entities/Object.h"
#include "Entities/Objects/Ammo.h"
#include "Entities/Objects/Weapon.h"

#include "EntityLoader.h"
#include "EntityManager.h"


#endif

