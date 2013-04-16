/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Camera.h
=================================================================================================*/
#include "World.h"

#ifndef _ge_CAMERA_H
#define _ge_CAMERA_H

namespace ge
{
    namespace world
    {
		/*=============================================================================
        -- Camera to visualize a world to the screen. It can "follow" an Entity, or
		   just free roam a world. The camera centers itself on its position.
        =============================================================================*/
        class Camera
        {
        private:
            int mScreenX, mScreenY;             //TODO: these values aren't even used  //position (pixels) on the screen
            unsigned mViewWidth, mViewHeight;   //TODO: these values aren't used correctly //dimensions (pixels) of the camera's screen area
            Vector3D<double> mPos;              //position in the world that the upper left corner represents in the world

            World *mWorld;						//pointer to the world that the camera visualizes
            WeakPtr<Entity> mTarget;            //target entity to "follow" by centering the camera on it's world position
            Window  *mWindow;					//window to render everything to


        public:
            Camera();
            ~Camera() {}

			//general
            void Draw(); //TODO the z needs to be implemented in Draw()
            void Move(double x, double y, double z);
			void MoveTarget(double x, double y, double z);
            void GoTo(double x, double y, double z);
			void GoTo(Vector3D<double> pos) { GoTo(pos.x, pos.y, pos.z); }

			//mutators
			void SetScreenPos(int posX, int posY) { mScreenX = posX; mScreenY = posY; }
			void SetViewSize(unsigned width, unsigned height) { mViewWidth = width; mViewHeight = height; }
            void SetWorld(World *world) { mWorld = world; }
			void SetTarget(SharedPtr<Entity> target) { mTarget = target; }
			void SetTarget(WeakPtr<Entity> target) { mTarget = target; }
            void SetWindow(Window *window) { mWindow = window; }
        };
    };
};

#endif

