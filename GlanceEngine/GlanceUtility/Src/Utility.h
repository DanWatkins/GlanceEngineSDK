/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Utility.h
=================================================================================================*/

#ifndef _ge_UTILITY_H
#define _ge_UTILITY_H

//standard includes
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <list>
#include <vector>
#include <map>
#include <exception>
#include <stdio.h>
#include <cstdlib>

#include <boost/filesystem.hpp>

//project includes
#include "String.h"
#include "Extensions.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Rect.h"
#include "IO.h"
#include "HML.h"


//Windows specific includes
#ifdef BUILD_WINDOWS
#include <memory>
#endif


//Linux specific includes
#ifdef BUILD_LINUX
#include <tr1/memory>
#endif


//abbreviations
#define SharedPtr std::tr1::shared_ptr
#define WeakPtr std::tr1::weak_ptr


template<class T1, class T2>
inline SharedPtr<T1> DynamicPtrCast(const SharedPtr<T2>& sourcePtr)
{
	return std::tr1::dynamic_pointer_cast<T1>(sourcePtr);
}

typedef unsigned char uint8;
typedef char int8;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned long uint32;
typedef long int32;

#endif

