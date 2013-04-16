/*=================================================================================================
  GlanceEngine - (C) 2010-2013 Daniel L. Watkins

  Filename: Extensions.h
=================================================================================================*/

#include "Utility.h"

#ifndef _ge_EXTENSIONS_H
#define _ge_EXTENSIONS_H

namespace ge
{
	//forward declarations
	template <class T>
    class Vector2D;

	template <class V>
	class Vector3D;
	class String;

    //pre-calculated constants
    const double PI = 3.141592654;
	const double TWO_PI = PI*2;
    const double PI_OVER_180 = 0.0174532925;
    const double PI_UNDER_180 = 57.29577951;



    /*=============================================================================
    -- Math helper functions.
    =============================================================================*/
    double ToDouble(String text);
    float ToFloat(String text);
    int ToInt(String text);
    bool ToBool(String text);
    Vector2D<double> ToVector2D(String text);
	Vector3D<double> ToVector3D(String text);

    double DivideD(double a, double b);
    float DivideF(float a, float b);
    int DivideI(int a, int b);

    int Round(double n);
    int Round(float n);
    int RoundUp(double n);
    int RoundUp(float n);
    int RoundDown(double n);
    int RoundDown(float n);

    double DegToRad(double deg);
    float DegToRad(float deg);
    int DegToRad(int deg);
    double RadToDeg(double rad);
    float RadToDeg(float rad);
    int RadToDeg(int rad);


	bool LessThan(double number, double numberToBeLessThan, double marginOfError);
	bool GreaterThan(double number, double numberToBeGreaterThan, double marginOfError);
	bool EqualTo(double number, double numberToBeEqual, double marginOfError);

    /*=============================================================================
    -- Text reference to identify some data type by a name identifier.
    =============================================================================*/
    template<class T>
    struct TextReference
    {
        String name;
        T ref;
    };
};

#endif

