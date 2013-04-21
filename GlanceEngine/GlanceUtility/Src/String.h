/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: String.h
=================================================================================================*/

#include "Utility.h"

#ifndef _ge_STRING_H
#define _ge_STRING_H

#define PROCESS_ALL -1 //used for making functions do everything. Such as Replace("A", "a", PROCESS_ALL) would replace all occurences of A with a
#define INCLUDE_DELIMINATOR true

namespace ge
{
    /*=============================================================================
      String class for doing special operations on text.
    =============================================================================*/
    class String
    {
    private:
        std::string mString;				//actual content of the String

    public:
        //constructor and destructor
        String() { mString = ""; }
        String(std::string str);
		String(const char *c);
        ~String() {}

        //accessor/mutator
        std::string GetStd() { return (this->mString); }
        void Set(String str) { this->mString = str.GetStd(); }

        //conversion methods
        const char *GetChar() { return (mString.c_str()); }
        double GetNumeric();
        double GetNumeric(std::string str);

        //operators
        bool operator ==(String cstr);
        bool operator !=(String cstr);
        void operator +=(String cstr);
        void operator -=(String cstr);
        String operator [](int n);

        //general
        unsigned Size() { return mString.size(); }
        void Clear() { mString.clear(); }
        bool Find(String cstr, int times=1);
		bool IsNumeric(int p1=0, int p2=-1);
		void Print() { std::cout << mString << std::endl; }
		std::vector<String> GetSectionsDeliminatedBy(String deliminator);

		//removal
        void Remove(unsigned p1, unsigned p2);
        void Remove(String cstr, int times=1);
        void RemoveAll(String cstr) { RemoveAll(cstr.GetStd()); }
        void RemoveAll(std::string str) { Remove(str, -1); }
        void RemoveUntil(String delim, int times=1, bool includeDelim=false);
		void RemoveLast(int numerOfCharacters=1);

		//arrangment
		void Insert(String cstr, int location=0);
        void Replace(String text, std::string replacement, int times=1);
        void TransferFront(unsigned p1, unsigned p2, String& client);
        void TransferBack(unsigned p1, unsigned p2, String& client);
        void PushFront(String text) { Insert(text, 0); }
        void PushBack(String text) { Insert(text, Size()); }
		int ApplyOperations();
    };


	/*=============================================================================
      Converts most primitive data types to a string.
    =============================================================================*/
    template <class T>
    String ToString(const T & t, int places=3)
    {
        std::ostringstream oss;
        oss.precision(places);
        oss.setf(std::ios_base::fixed);
        oss << t;

        return String(oss.str());
    }


	/*=============================================================================
	-- Combines all of the specified strings into one big string.
	=============================================================================*/
    //String Combine(String s1, String s2, String s3="", String s4="", String s5="", String s6="");


	//global String + operator
	String operator+(String lhs, String rhs);


	template <class A>
	String operator +(String lhs, A number) { return lhs+ToString(number); }
};

#endif

