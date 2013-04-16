/*=================================================================================================
  GlanceEngine - (C) 2010-2013 Daniel L. Watkins

  Filename: Extensions.cpp
=================================================================================================*/

#include "Utility.h"

namespace ge
{
    /*=============================================================================
    -- Combines all of the passed strings into a single String.
    =============================================================================*/
    String Combine(String s1, String s2, String s3, String s4, String s5, String s6)
    {
		s5.PushBack(s6);
		s4.PushBack(s5);
		s3.PushBack(s4);
		s2.PushBack(s3);
		s1.PushBack(s2);
        return s1;
    }


    /*=============================================================================
    -- Converts the string into a double.
    =============================================================================*/
    double ToDouble(String text)
    {
        double n;
		std::stringstream(text.GetStd()) >> n;
        return n;
    }


    /*=============================================================================
    -- Converts the string into a float.
    =============================================================================*/
    float ToFloat(String text)
    {
        float n;
		std::stringstream(text.GetStd()) >> n;
        return n;
    }


    /*=============================================================================
    -- Converts the string into an int.
    =============================================================================*/
    int ToInt(String text)
    {
        int n;
		std::stringstream(text.GetStd()) >> n;
        return n;
    }


    /*=============================================================================
    -- Converts the string into a bool.
    =============================================================================*/
    bool ToBool(String text)
    {
		if (text == "0")
			return false;

        if (text == "true" || text == "True" || text == "TRUE")
            return true;

		if (text.IsNumeric())
			return true;

        return false;
    }


    /*=============================================================================
    -- Converts the string into a Vector2D. Format is (x,y). The parenthesis are
	   actually optional, but are prefered.
    =============================================================================*/
    Vector2D<double> ToVector2D(String text)
    {
        Vector2D<double> vect;
        text.Remove("(");
        text.Remove(")");

        String tA(text);
        tA.RemoveUntil(",", 1, true);
        vect.y = ToDouble(tA.GetStd());
        
        String tB(text);
        tB.Remove(tA);
        tB.Remove(",");
        vect.x = ToDouble(tB.GetStd());

        return vect;
    }


	/*=============================================================================
    -- Converts the string into a Vector2D. Format is (x,y,z). The parenthesis are
	   actually optional, but are prefered.
    =============================================================================*/
    Vector3D<double> ToVector3D(String text)
    {
        Vector3D<double> vect;
        text.Remove("(");
        text.Remove(")");

        String tA(text);
        tA.RemoveUntil(",", 2, true);
        vect.z = ToDouble(tA.GetStd());
        
		String tB(text);
        tB.RemoveUntil(",", 1, true);
		//TODO convert this into String::RemoveFrom(...)
		while (tB[tB.Size()-1] != ","  &&  tB.Size() > 0)
			tB.RemoveLast(1);
		tB.RemoveLast(1);

        vect.y = ToDouble(tB.GetStd());

		// 700,920,0

		String tC(text);
        
		while (tC.Find(","))
		{
			while (tC[tC.Size()-1] != ","  &&  tC.Size() > 0)
				tC.RemoveLast(1);
			tC.RemoveLast(1);
		}

        vect.x = ToDouble(tC.GetStd());

        return vect;
    }

    /*=============================================================================
    -- Divides two doubles and checks for DIV/0.
    =============================================================================*/
    double DivideD(double a, double b)
    {
        if (b != 0.0)
            return a/b;

        return 0.0;
    }


    /*=============================================================================
    -- Divides two floats and checks for DIV/0.
    =============================================================================*/
    float DivideF(float a, float b)
    {
        if (b != 0.0f)
            return a/b;

        return 0.0f;
    }


    /*=============================================================================
    -- Divides two integers and checks for DIV/0.
    =============================================================================*/
    int DivideI(int a, int b)
    {
        if (b == 0)
            return 0;

        return a/b;
    }


    /*=============================================================================
    -- Rounds to the nearest integer.
    =============================================================================*/
    int Round(double n)
    {
        double result = 0.0;

        //get the decimal part
        double n2 = n - (int)n;

        //see if the decimal part is closer to 0 or 1
        if (1.0-n2 <= 0.0+n2)
            result = n+(1.0-n2);
        else if (1.0-n2 > 0.0+n2)
            result = n-n2;

        return (int)result;
    }

    int Round(float n)
    {
        return (int)Round((double)n);
    }


    /*=============================================================================
    -- Rounds up to the nearest integer.
    =============================================================================*/
    int RoundUp(double n)
    {
        return (int)std::ceil(n);
    }

    int RoundUp(float n)
    {
        return (int)std::ceil(n);
    }


    /*=============================================================================
    -- Rounds down to the nearest integer.
    =============================================================================*/
    int RoundDown(double n)
    {
        return (int)std::floor(n);
    }

    int RoundDown(float n)
    {
        return (int)std::floor(n);
    }


    /*=============================================================================
    -- Converts from degrees to radians.
    =============================================================================*/
    double DegToRad(double deg)
    {
        return deg*PI_OVER_180;
    }

    float DegToRad(float deg)
    {
        return deg*(float)PI_OVER_180;
    }

    int DegToRad(int deg)
    {
        return Round((double)deg*PI_OVER_180);
    }


    /*=============================================================================
    -- Converts from radians to degrees.
    =============================================================================*/
    double RadToDeg(double rad)
    {
        return rad*PI_UNDER_180;
    }

    float RadToDeg(float rad)
    {
        return rad*(float)PI_UNDER_180;
    }

    int RadToDeg(int rad)
    {
        return Round((double)rad*PI_UNDER_180);
    }

	
	/*=============================================================================
    -- Functions to compare two doubles within a margin of error (accounting for
	   loss of precision).
    =============================================================================*/
	bool LessThan(double number, double numberToBeLessThan, double marginOfError)
	{
		return (number < (numberToBeLessThan-marginOfError));
	}

	bool GreaterThan(double number, double numberToBeGreaterThan, double marginOfError)
	{
		return (number > (numberToBeGreaterThan+marginOfError));
	}

	bool EqualTo(double number, double numberToBeEqual, double marginOfError)
	{
		//TODO not sure if this needs to be inclusive of the outter bounds of the margin of error?
		return ((number > (numberToBeEqual-marginOfError)) && (number < (numberToBeEqual+marginOfError)));
	}
};