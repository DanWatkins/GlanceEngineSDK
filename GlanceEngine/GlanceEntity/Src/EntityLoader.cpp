/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: EntityLoader.cpp
=================================================================================================*/
#include "Entity.h"

namespace ge
{
    namespace world
    {
        /*=============================================================================
        -- Loads every entity in the entity file directory "../Data/Entity/"
        =============================================================================*/
        bool EntityLoader::LoadEntityDatabase(World *world)
        {
            mWorld = world;

			if (mWorld == NULL)
			{
				gDebug.Error("Cannot load entity database: Bad world object"); //TODO error clean this up
				return false;
			}

            //generate a list of every template entity file
            io::DirectoryListing listing;
            listing = io::GetFilesInDirectory(DIR_ASSET_DATA, ".te");

            //serilize each file in the list
            io::DirectoryListing::iterator iter = listing.begin();
            while (iter != listing.end())
            {
                LoadTemplateEntityFromFile(*iter);
                iter++;
            }

            return true;
        }


        /*=============================================================================
        -- Loads a template entity from @filepath and adds it to the template entity
           list. Note that it will make sub-classed entity types where appropriate.
        =============================================================================*/
        bool EntityLoader::LoadTemplateEntityFromFile(String filepath)
        {
            HMLFile file;
            file.Open(filepath);
            EntityType type = Parse_EntityType(file);

            //Actor
            if (type == ACTOR)
            {
                SharedPtr<Actor> actor(new Actor);
                actor->_SetWorld(mWorld);
                Parse_Entity_HeaderData(file, (SharedPtr<Entity>)actor);
				Parse_Entity_WorldData(file, (SharedPtr<Entity>)actor);
                Parse_Entity_GraphicsData(file, actor);
				mWorld->GetEntityManager()->_AddTemplateEntity(actor);
            }
			//Object
			else if (type == OBJECT)
			{
				ObjectType objectType = Parse_ObjectType(file);

				//regular Object
				if (objectType == STATIC)
				{
					SharedPtr<Object> object(new Object);
					object->_SetWorld(mWorld);
					Parse_Entity_HeaderData(file, (SharedPtr<Entity>)object);
					Parse_Entity_WorldData(file, (SharedPtr<Entity>)object);
					Parse_Object_ObjectData(file, object);
					Parse_Entity_GraphicsData(file, object);
					mWorld->GetEntityManager()->_AddTemplateEntity(object);
				}
				//Ammo
				else if (objectType == AMMO)
				{
					SharedPtr<Ammo> ammo(new Ammo);
					ammo->_SetWorld(mWorld);
					Parse_Entity_HeaderData(file, (SharedPtr<Entity>)ammo);
					Parse_Entity_WorldData(file, (SharedPtr<Entity>)ammo);
					Parse_Object_ObjectData(file, ammo);
					Parse_Ammo_AmmoData(file, ammo);
					Parse_Entity_GraphicsData(file, ammo);
					mWorld->GetEntityManager()->_AddTemplateEntity(ammo);
				}
				//Weapon
				else if (objectType == WEAPON)
				{
					SharedPtr<Weapon> weapon(new Weapon);
					weapon->_SetWorld(mWorld);
					Parse_Entity_HeaderData(file, (SharedPtr<Entity>)weapon);
					Parse_Entity_WorldData(file, (SharedPtr<Entity>)weapon);
					Parse_Object_ObjectData(file, weapon);
					Parse_Weapon_WeaponData(file, weapon);
					Parse_Entity_GraphicsData(file, weapon);
					mWorld->GetEntityManager()->_AddTemplateEntity(weapon);
				}
			}

            return true;
        }


        /*=============================================================================
        -- Parses a template entity file to determine the type of entity.
        =============================================================================*/
        EntityType EntityLoader::Parse_EntityType(HMLFile& file)
        {
            file.Reset();
            if (file.SeekElementOpen("<header>"))
            {
                while (!file.AtElementClose("</header>"))
                {
                    if (file.TokenIsDeclaration("#type"))
                    {
						file.Seek();
						if (file.GetToken() == "ACTOR")
                            return ACTOR;
                        else if (file.GetToken() == "OBJECT")
                            return OBJECT;
                    }

                    file.Seek();
                }
            }
            return UNDEFINED;
        }


        /*=============================================================================
        -- Parses a template entity file's header portion and derializes the data
           to @entity.
        =============================================================================*/
        void EntityLoader::Parse_Entity_HeaderData(HMLFile& file, SharedPtr<Entity> entity)
        {
            file.Reset();
            if (file.SeekElementOpen("<header>"))
            {
                while (!file.AtElementClose("</header>"))
                {
                    if (file.TokenIsDeclaration("#name"))
                        entity->_SetName(file.SeekGetToken());

                    else if (file.TokenIsDeclaration("#type"))
                    {
                        if (file.SeekGetToken() == "ACTOR")
                            entity->_SetType(ACTOR);
                        else if (file.GetToken() == "OBJECT")
                            entity->_SetType(OBJECT);
                    }

                    else if (file.TokenIsDeclaration("#displayName"))
                        entity->SetDisplayName(file.SeekGetToken());

                    //else if (file.TokenIsDeclaration("#displayIcon"))

                    file.Seek();
                }
            }
        }


		/*=============================================================================
        -- Parses a template entity file's world portion and derializes the data
           to @aentity.
        =============================================================================*/
		void EntityLoader::Parse_Entity_WorldData(HMLFile& file, SharedPtr<Entity> entity)
		{
			file.Reset();
            if (file.SeekElementOpen("<world>"))
            {
                while (!file.AtElementClose("</world>"))
                {
					bool respondsToGravity = true;

					if (file.TokenIsDeclaration("#body"))
					{
						String type = file.SeekGetToken();

						//make a box body type
						if (type == "BOX")
						{
							//parse the position the Body is relative to the Entity
							Vector3D<double> anchor = ToVector3D(file.SeekGetToken());

							//parse the dimensions of the box
							double width = ToDouble(file.SeekGetToken());
							double length = ToDouble(file.SeekGetToken());
							double height = ToDouble(file.SeekGetToken());



							//make box for a connected body
							if (!entity->_GetBody().expired())
							{
								//the anchor for connected bodies is the offset from the primary body
								WeakPtr<physics::Body> box = mWorld->GetReservedPhysicsEnvironment()->CreateBox(width, length, height, anchor);

								box.lock()->SetRespondsToGravity(respondsToGravity);
								entity->_GetBody().lock()->ConnectWithBody(box);
							}
							//make body for the primary body
							else
							{
								//there is no offset for the primary body
								WeakPtr<physics::Body> box = mWorld->GetReservedPhysicsEnvironment()->CreateBox(width, length, height);

								box.lock()->SetRespondsToGravity(respondsToGravity);
								entity->_SetBody(box);

								//set the anchor for offsetting the image
								//this can only be for the primary body
								entity->_SetBodyAnchor(anchor);
							}


						}
						//make a circle body type
						else if (type == "CYLINDER")
						{
							//next parse the position the Body is relative to the Entity
							Vector3D<double> anchor = ToVector3D(file.SeekGetToken());

							//parse the dimensions of the box
							double radius = ToDouble(file.SeekGetToken());

							//create a reserved physics::Body object for other Entities to template off
							WeakPtr<physics::Body> circle = mWorld->GetReservedPhysicsEnvironment()->CreateCircle(radius, Vector3D<double>());

							circle.lock()->SetRespondsToGravity(respondsToGravity);

							//connect this with the primary body for this entity if there is one
							if (!entity->_GetBody().expired())
								entity->_GetBody().lock()->ConnectWithBody(circle);
							//or just set it at the primary body since there is not already one
							else
								entity->_SetBody(circle);

							//set the anchor for offsetting the image
							entity->_SetBodyAnchor(anchor);
						}

						type = "";
					}
					else if (file.TokenIsDeclaration("#respondsToGravity"))
					{
						respondsToGravity = ToBool(file.SeekGetToken());

						//set the entity's body's respondsToGravity boolean if the body is already created
						if (!entity->_GetBody().expired())
							entity->_GetBody().lock()->SetRespondsToGravity(respondsToGravity);
					}

                    file.Seek();
                }
            }
		}


		/*=============================================================================
        -- Parses a template entity file's world portion and derializes the data
           to @aentity.
        =============================================================================*/
		void EntityLoader::Parse_Entity_GraphicsData(HMLFile& file, SharedPtr<Entity> entity)
		{
			file.Reset();
            if (file.SeekElementOpen("<graphics>"))
            {
                while (!file.AtElementClose("</graphics>"))
                {
					if (file.SeekElementOpen("<animations>"))
					{
						while (!file.AtElementClose("</animations>"))
						{
							if (file.TokenIsDeclaration("#add"))
							{
								String name = file.SeekGetToken();
								String filename = file.SeekGetToken();
								
								//load the animation and add it to the manager
								SharedPtr<Animation> newAnimation(new Animation(name));
								//only add the animation if it was loaded corectly

								if (newAnimation->LoadAnimationFromFile(filename, mWorld->GetAssetManager()))
									entity->GetAnimationManager().lock()->AddAnimation(newAnimation);
							}

							file.Seek();
						}
					}
                    file.Seek();
                }
            }
		}


		/*=============================================================================
        -- Parses a template object file to determine the object type.
        =============================================================================*/
		ObjectType EntityLoader::Parse_ObjectType(HMLFile& file)
		{
			file.Reset();
            if (file.SeekElementOpen("<object>"))
            {
                while (!file.AtElementClose("</object>"))
                {
                    if (file.TokenIsDeclaration("#objectType"))
                    {
						file.Seek();
						if (file.GetToken() == "STATIC")
                            return STATIC;
                        else if (file.GetToken() == "AMMO")
                            return AMMO;
						else if (file.GetToken() == "WEAPON")
							return WEAPON;
                    }

                    file.Seek();
                }
            }
            return STATIC;
		}


		/*=============================================================================
        -- Parses a template object file's date portion and derializes the data
           to @object. Note that this is only for the Object subclass of Entity.
        =============================================================================*/
		void EntityLoader::Parse_Object_ObjectData(HMLFile& file, SharedPtr<Object> object)
		{
			file.Reset();
            if (file.SeekElementOpen("<object>"))
            {
                while (!file.AtElementClose("</object>"))
                {
					//see what type of physics body the unit uses
                    if (file.TokenIsDeclaration("#combined"))
						object->SetCombined( ToBool(file.SeekGetToken()) );
					else if (file.TokenIsDeclaration("#obtainable"))
						object->SetObtainable( ToBool(file.SeekGetToken()) ) ;

                    file.Seek();
                }
            }
		}


		/*=============================================================================
        -- Parses a template ammo file's data portion and derializes the data
           to @ammo. Note that this is only for the Ammo subclass of Object.
        =============================================================================*/
		void EntityLoader::Parse_Ammo_AmmoData(HMLFile& file, SharedPtr<Ammo> ammo)
		{
			file.Reset();
            if (file.SeekElementOpen("<ammo>"))
            {
                while (!file.AtElementClose("</ammo>"))
                {
					//see what type of physics body the unit uses
                    if (file.TokenIsDeclaration("#roundCount"))
                    {
						ammo->FillRoundsTo( ToInt(file.SeekGetToken()) );
                    }
                    file.Seek();
                }
            }
		}


		/*=============================================================================
        -- Parses a template weapon file's data portion and derializes the data
           to @weapon. Note that this is only for the Weapon subclass of Object.
        =============================================================================*/
		void EntityLoader::Parse_Weapon_WeaponData(HMLFile& file, SharedPtr<Weapon> weapon)
		{
			file.Reset();
            /*if (file.SeekElementOpen("<weapon>"))
            {
                while (!file.AtElementClose("</weapon>"))
                {
					//see what type of physics body the unit uses
                    if (file.TokenIsDeclaration("#combined"))
                    {
						object->SetCombined( ToBool(file.SeekGetToken()) );
                    }
                    file.Seek();
                }
            }*/
		}
    };
};
