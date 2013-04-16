/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Body.h
=================================================================================================*/
#include "Physics.h"

#ifndef _ge_BODY_H
#define _ge_BODY_H

namespace ge
{
    namespace physics
    {
		/*=============================================================================
		-- Describes the type of Body. This is used to decide how to handle certain
		   Body ojects (as a Box, Circle, or whatever).
		=============================================================================*/
		enum BodyType
		{
			UNDEFINED,
			BOX,
			CYLINDER,
			SWEEPBOX
		};


		/*=============================================================================
		-- Abstract base class to represents bodies in an Environment.
		=============================================================================*/
		class Body
		{
		private:
			int mId;										//unique id of the Body
			int mHostId;									//id of the host (not used by the physics system)
			BodyType mType;									//type the Body is

			Vector3D<double> mPos;							//absoluet position of the body represented by the upper right of the Body in an Environment
			Vector3D<double> mVelocity;						//velocity of the Body. Call _ApplyVelocity() to actually move the Body the amount specified by @mVelocity

			bool mFoundSupportingBodyDuringUpdate;
			bool mHasSupportingBody;						//false if gravity is taking noticable effect on the body (falling)
			Timer mFallTimer;								//timer that resets whenever a body begins falling

			bool mRespondsToGravity;						//true if gravity can effect this body

			bool mActive;									//false if the environment should pretend like it doesn't exist
			bool mSaveVelocity;								//true if the velocity should not be reset to zero every frame

			std::vector< WeakPtr<Body> > mConnectedBodies;	//list of other bodies that move with this body


		protected:
			double mHeight;	//this dimension is used by every body (to extrued whatever shape it is)

			void _ApplyVelocity();
			void _SetId(int id) { mId = id; }
			void _SetHostId(int id);
			void _SetType(BodyType type) { mType = type; }
			void _SetPos(Vector3D<double> pos);
			
			void _SetX(double x);
			void _AddX(double x);
			void _SetY(double y);
			void _AddY(double y);
			void _SetZ(double z);
			void _AddZ(double z);
			void _SetPos(double x, double y, double z);
			void _SetVelocity(Vector3D<double> velocity);
			void _SetHeight(double height) { mHeight = height; }

			void _SetFoundSupportingBodyDuringUpdate(bool foundSupportingBodyDuringUpdate) { mFoundSupportingBodyDuringUpdate = foundSupportingBodyDuringUpdate; }
			bool _GetFoundSupportingBodyDuringUpdate() { return mFoundSupportingBodyDuringUpdate; }
			void _SetHasSupportingBody(bool hasSupportingBody);
			void _ResetFallDuration() { mFallTimer.Start(); }

			void _ResetVelocity();

		public:
			friend class Environment;
			friend class Box; //TODO that is just so it can set the id for the sweep box

			Body();
			virtual ~Body() {}
			virtual SharedPtr<SweepBox> GetSweepBox() = 0;
			virtual Vector3D<double> GetCenter() = 0;
			
			//general
			void AddVelocity(Vector3D<double> vect);
			void AddVelocity(double x, double y, double z);
			void SubtractVelocity();
			void AdjustVelocityByMultiplier(double multiplier) { mVelocity *= multiplier; }
			
			void RequestPos(double x, double y, double z);
			void ConnectWithBody(WeakPtr<Body> body);
			int GetConnectedBodiesCount() { return mConnectedBodies.size(); }
			WeakPtr<Body> GetConnectedBody(int n) { return mConnectedBodies.at(n); }

			//accessor and mutator
			int GetId() { return mId; }
			int GetHostId() { return mHostId; }
			BodyType GetType() { return mType; }
			Vector3D<double> GetPos() { return mPos; }
			Vector3D<double> GetVelocity() { return mVelocity; }
			void SetActive(bool active) { mActive = active; }
			bool GetActive() { return mActive; }
			void SetSaveVelocity(bool saveVelocity) { mSaveVelocity = saveVelocity; }	//newB connected support?
			bool GetSaveVelocity() { return mSaveVelocity; }
			double GetHeight() { return mHeight; }

			bool GetHasSupportingBody() { return mHasSupportingBody; }
			double GetFallDuration() { return mFallTimer.GetTimeAndEnd(); }
			void SetRespondsToGravity(bool respondsToGravity) { mRespondsToGravity = respondsToGravity; }
			bool GetRespondsToGravity() { return mRespondsToGravity; }
		};
	};
};

#endif

