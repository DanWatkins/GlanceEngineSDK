/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: HML.cpp
=================================================================================================*/

#include "Utility.h"

namespace ge
{
    /*=============================================================================
    -- Opens the file at @mFilepath.
    =============================================================================*/
    bool HMLFile::Open()
    {
        if (!mFile.is_open())
        {
            mFile.open(mFilepath.GetChar());
            if (!mFile.is_open())
                return false;
        }

        return true;
    }


    /*=============================================================================
    -- Sets @mFilepath to @filepath and then opens the file.
    =============================================================================*/
    bool HMLFile::Open(String filepath)
    {
        mFilepath = filepath;
        io::Standardize(mFilepath);
        return Open();
    }


    /*=============================================================================
    -- Closes the file.
    =============================================================================*/
    void HMLFile::Close()
    {
        mFile.close();
    }


    /*=============================================================================
    -- Closes and then opens the file to effectively set the seek position to the
       beginning of the file.
    =============================================================================*/
    bool HMLFile::Reset()
    {
        Close();
        return Open();
    }


    /*=============================================================================
    -- Seeks through the file until it reaches the opening element (thing in tags
       "<>") or the end.
    =============================================================================*/
    bool HMLFile::SeekElementOpen(String element)
    {
        //add the "<>" tags if they aren't already there
        if (element[0] != "<")
            element.Insert("<", 0);
        if (element[element.Size()-1] != ">")
            element.Insert(">", element.Size());

        if (!mFile.is_open())
            return false;

        //seek to the element tag
        while (GetToken() != element.GetStd())
        {
			//return false because the file couldn't seek any further (at end of file)
            if (!Seek())
                return false;
        }


        return true;
    }


	/*=============================================================================
    -- Returns true if the open element (thing in tags "<>" but with a "/") is
	   equivalent to the token.
    =============================================================================*/
	bool HMLFile::AtElementOpen(String element)
	{
		//add the "<>" tags if they aren't already there
        if (element[0] != "<")
            element.Insert("<", 0);
        if (element[element.Size()-1] != ">")
            element.Insert(">", element.Size());

        if (GetToken() == element)
            return true;

		if (mFile.eof())
			return true;

        return false;
	}


    /*=============================================================================
    -- Returns true if the end element (thing in tags "<>" but with a "/") is
	   equivalent to the token.
    =============================================================================*/
    bool HMLFile::AtElementClose(String element)
    {
        //add the "</>" tags if they aren't already there
        if (element[0] != "<")
            element.Insert("<", 0);
        if (element[element.Size()-1] != ">")
            element.Insert(">", element.Size());
        if (element[1] != "/")
            element.Insert("/", 1);

        if (GetToken() == element)
            return true;

		if (mFile.eof())
			return true;

        return false;
    }


    /*=============================================================================
    -- Advances 1 time in the file and puts the read string into @mToken.
    =============================================================================*/
    bool HMLFile::Seek()
    {
        if (mFile.is_open())
        {
            if (mFile.eof())
                return false;

            mFile >> mToken;
        }

        return true;
    }


	/*=============================================================================
    -- Seeks to the next declaration (just any declaration).
    =============================================================================*/
    void HMLFile::SeekDeclaration()
    {
		//TODO: this doesn't check for end of file, but TokenIsDeclaration probably returns true if eof is found
        while (!TokenIsDeclaration(this->GetToken()))
            Seek();
    }


    /*=============================================================================
    -- Seeks and returns @mToken. Sort of handy when parsing large files with lots
       of data.
    =============================================================================*/
    String HMLFile::SeekGetToken()
    {
        Seek();
        return GetToken();
    }


    /*=============================================================================
    -- Returns true if the token is a declaration (#declaration).
    =============================================================================*/
    bool HMLFile::TokenIsDeclaration(String dec)
    {
        //add the # if it isn't already there
        String declaration(dec);
        if (declaration[0] != "#")
            declaration.Insert("#", 0);

        if (GetToken() == declaration.GetStd())
            return true;

        return false;
    }
};