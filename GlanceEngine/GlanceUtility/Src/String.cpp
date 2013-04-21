/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: String.cpp
=================================================================================================*/

#include "Utility.h"

namespace ge
{
    /*=============================================================================
      Constructor for String.
    =============================================================================*/
    String::String(std::string str) { mString = str; }
	String::String(const char *c) { mString = c; }


    /*=============================================================================
      Returns the numeric equivalent of the string in the form of a double.  If
      the string contains non numeric symbol (- and . are no problem), the return
      value will be 0.0f.
    =============================================================================*/
    double String::GetNumeric()
    {
        return (GetNumeric(mString));
    }

    double String::GetNumeric(std::string str)
    {
        double n;
        std::stringstream(str) >> n;
        return n;
    }


    /*=============================================================================
      Returns true if cstr is identical to mString.
    =============================================================================*/
    bool String::operator ==(String str)
    {
		if (str.GetStd() == mString)
            return true;
        else
            return false;
    }


    /*=============================================================================
      Returns true if cstr is not identical to mString.
    =============================================================================*/
    bool String::operator !=(String str)
    {
		if (str.GetStd() != mString)
            return true;
        else
            return false;
    }


    /*=============================================================================
      Puts cstr at the end of the class's string.
    =============================================================================*/
    void String::operator +=(String str)
    {
		mString.append(str.GetStd());
    }


    /*=============================================================================
      Attempts to remove the string 1 time.
    =============================================================================*/
    void String::operator -=(String str)
    {
        Remove(str, 1);
    }


    /*=============================================================================
      Returns the nth character.  Zero based!
    =============================================================================*/
    String String::operator [](int n)
    {
        if (n < 0)
            return String("");

        std::string temp2 = mString;
        temp2.erase(0,n);
        temp2.erase(1, temp2.size());
        String temp(temp2);

        return temp;
    }


    /*=============================================================================
      Returns true if cstr has been found in mString at least n times.
    =============================================================================*/
    bool String::Find(String str, int times)
    {
        //is it possible for this to even exist in the string class?
        if (! (mString.size() >= str.Size()) )
            return false;

        int foundTimes = 0;

        //save the first character of the string
		std::string firstChar = str.GetStd();

        if (firstChar.size() > 1)
            firstChar.erase(1, firstChar.size()-1);

        for (unsigned pos=0; pos<mString.size(); pos++)
        {
            std::string test = mString;
            test.erase(0, pos);//TODO  use [] access
            test.erase(1, test.size()-1);

            if (test == firstChar)
            {
                //test the following characters to see if this is the string
                test = mString;
                test.erase(0, pos);

                //is the remaining part of the string long enouph?
                if (test.size() >= str.Size())
                {
                    pos += str.Size();
                    test.erase(str.Size(), test.size()-1);

                    if (str == test)
                        foundTimes++;
                }
                else
                    return false;
            }

            if (foundTimes >= times)
                return true;
        }

        return false;
    }


	/*=============================================================================
	-- Returns true if the string is numeric between the two positions. Numeric is
	   defined as having only characters "0" to "9", ".", and ",".
	=============================================================================*/
	bool String::IsNumeric(int p1, int p2)
	{
		//bounds check
		if (p2 < 0)
			p2 = Size();
		if (p1 > p2 || p1 < 0)
			p1 = 0;

		for (int n=p1; n<p2; n++)
		{
			String s = (*this)[n];

			switch ( *s[0].GetChar() )
			{
			case 49:		//1
				break;
			case 50:		//2
				break;
			case 51:		//3
				break;
			case 52:		//4
				break;
			case 53:		//5
				break;
			case 54:		//6
				break;
			case 55:		//7
				break;
			case 56:		//8
				break;
			case 57:		//9
				break;
			case 48:		//0
				break;
			case 45:		//minus symbol
				break;
			case 46:		//period
				break;
			//it isn't a number, period, or nminus symbol
			default:
				return false;
			}
		}

		return true;
	}


	/*=============================================================================
      Returns a vector containing the parts separated by the deliminator.
    =============================================================================*/
	std::vector<String> String::GetSectionsDeliminatedBy(String deliminator)
	{
		std::vector<String> sections;
		String currentSection;

		for (unsigned n=0; n<=Size(); n++)
		{
			if (n != Size())
			{
				if ( (*this)[n] != deliminator.GetChar() )
					currentSection.PushBack( (*this)[n] );
				else
				{
					sections.push_back(currentSection);
					currentSection.Clear();
				}
			}
			else if (currentSection.Size() > 0)
				sections.push_back(currentSection);
		}

		return sections;
	}



    /*=============================================================================
      Inserts the passed string after the number of characters in the class's
      string specified with the location parameter.
    =============================================================================*/
    void String::Insert(String str, int location)
    {
        //make sure the location is within the size of the class's string
        if ((int)mString.size() < location)
            location = mString.size();

        //insert the string using the std::string::insert() method
		mString.insert(location, str.GetStd());
    }


    /*=============================================================================
      Erases everything between the two positions in @mString.
    =============================================================================*/
    void String::Remove(unsigned p1, unsigned p2)
    {
		if (p2 > p1)
		{
			p2 = p2-p1;
			mString.erase(p1, p2);
		}
    }


    /*=============================================================================
      Searches mString for str and removes it @times times if it exists at all,
      or that many times.
    =============================================================================*/
    void String::Remove(String str, int times)
    {
        //the source string must be of at least the same length as the string to remove
        if (str.Size() > 0  &&  mString.size() >= str.Size())
        {
            std::string test = "";
            std::string first = "";
            int nTimes = 0;

            //determine the first character of str
			test = str.GetStd();
            first = test.erase(1, test.length()-1);

            //search mString for the first character of str
            for (unsigned n=0; n<mString.size(); n++)
            {
                test = mString;
                test.erase(0,n);
                test.erase(1, test.length()-1);
                if (test == first)
                {
                    test = mString;
                    test.erase(0, n);
                    test.erase(str.Size(), test.length()-str.Size());

                    //see if all of str exists in mString, then erase it from mString
                    if (str == test  &&  (nTimes < times || times<0) )
					{
                        mString.erase(n, str.Size());
                        nTimes++;

                        if (nTimes >= times  &&  times>=0)
                            break;
                    }
                }
            }
        }
    }


    /*=============================================================================
      Removes everything in the string until the deliminator is reached. The
      deliminator is removed if @includeDelim is true. If @times is less than zero
      the process keeps going until the end of @mString is reached.
    =============================================================================*/
    void String::RemoveUntil(String delim, int times, bool includeDelim)
    {
        int timesRemoved = 0;

        for (unsigned m=0; m<mString.size(); m++)
        {
            if (timesRemoved < times || times < 0)
            {
                std::string t = mString;
                t.erase(0, m);
                t.erase(1, t.size()-1);

                if (delim == t)
                {
                    mString.erase(0, m);

                    if (includeDelim && mString.size() > 0)
                        mString.erase(0, 1);

                    timesRemoved++;
                    m = 0; //need to start at the beginning of the new length string
                }
            }
            else
                return;
        }
    }


	/*=============================================================================
	-- Removes the last @numberOfCharacters characters in the string.
	=============================================================================*/
	void String::RemoveLast(int numerOfCharacters)
	{
		int length = Size();
		int p1 = length-numerOfCharacters;

		if (p1 < 0)
			p1 = 0;

		Remove(p1, length);
	}


    /*=============================================================================
      Replaces the first occurence of @text in @mString @times.  If times is less
      than zero, then it replaces every occurence.
    =============================================================================*/
    void String::Replace(String text, std::string replacement, int times)
    {
        int timesReplaced=0; //times replaced

		std::string firstChar = text.GetStd();
        if (firstChar.size() > 0)
            firstChar.erase(1, firstChar.size()-1);

        std::string test = "";

        //check every character of @mString if it matches with the first character of @text
        for (unsigned n=0; n<mString.size(); n++)
        {
           if (timesReplaced >= times  &&  times > 0)
                return;

            test = mString;
            test.erase(0, n);
            test.erase(1, test.size()-1);

            if (test == firstChar)
            {
                std::string test2 = mString;
                test2.erase(0, n);

                //does the rest of the characters in @mString match with what is being searched for?
                if (test2.size() >= text.Size())
                {
                    test2.erase(text.Size(), test2.size()-1);

                    if (text == test2)
                    {
                        Remove(n, n+text.Size());
                        Insert(replacement, n);

                        n += replacement.size();
                        timesReplaced++;
                    }
                }
                //there is no way for anything else to occur
                else
                    return;
            }
        }
    }


    /*=============================================================================
      Removes the text between positions @p1 and @p2 in mString, and sticks it at
      the front of @client.
    =============================================================================*/
    void String::TransferFront(unsigned p1, unsigned p2, String& client)
    {
        std::string stringOld(mString);
        Remove(0, p1);
        Remove(p2-p1, Size()-1);
        String shipment(*this);
        client.PushFront(shipment);
        mString = stringOld;
        Remove(p1, p2);
    }


    /*=============================================================================
      Removes the text between positions @p1 and @p2 in mString, and appends it to
      the end of @client.
    =============================================================================*/
    void String::TransferBack(unsigned p1, unsigned p2, String& client)
    {
        std::string stringOld(mString);
        Remove(0, p1);
        Remove(p2-p1, Size()-1);
        String shipment(*this);
        client.PushBack(shipment);
        mString = stringOld;
        Remove(p1, p2);
    }


	/*=============================================================================
      Takes into account miscalculaneous key codes that are in the string.
	  Backspace, tab, enter. It returns the net size. Negative values occur if
	  there were more backspaces then regular chars.
    =============================================================================*/
	int String::ApplyOperations()
	{
		bool done = false;
		int extraBackSpaces = 0;

		while (!done)
		{
			unsigned n = 0;

			while (n < Size())
			{
				//backspace (erases itself and the previous character)
				if (mString[n] == 8)
				{
					//make sure this backspace is not the first character (bounds check)
					if (n > 0)
						Remove(n-1,n+1);
					else
					{
						Remove(n, n+1);
						extraBackSpaces++;
					}

					break;
				}
				//tab
				else if (mString[n] == 9)
				{
					Remove(n, n+1);
					break;
				}

				//enter
				else if (mString[n] == 13)
				{
					Remove(n, n+1);
					break;
				}

				n++;
			}

			if (n == Size())
				done = true;
		}

		return Size()-extraBackSpaces;
	}


	String operator+(String lhs, String rhs)
	{
		lhs.PushBack(rhs);
		return lhs;
	}
};