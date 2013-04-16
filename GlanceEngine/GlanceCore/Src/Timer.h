/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Timer.h
=================================================================================================*/
#include "Glance.h"

#ifndef _ge_TIMER_H
#define _ge_TIMER_H

namespace ge
{
    /*=============================================================================
      Class to keep track of time. 
    =============================================================================*/
    class Timer
    {
    private:
        sf::Clock mClock;
        float mEnd;


    public:
        Timer()
        {
            Start();
            mEnd = 0.0f;
        }

        void Start()
        {
            mClock.Reset();
        }

        void End()
        {
            mEnd = mClock.GetElapsedTime();
        }

		//returns time in milli-seconds
        float GetTime()
        {
            return mEnd*1000;
        }

        float GetTimeAndEnd()
        {
            End();
            return GetTime();
        }
    };

};

#endif

