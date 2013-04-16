/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: HML.h
=================================================================================================*/

#include "Utility.h"

#ifndef _ge_HML_H
#define _ge_HML_H

namespace ge
{
    /*=============================================================================
    -- Hailex Markup Language File class. This markup language is used for
       serializing data using a text formatted file format on disc.
    =============================================================================*/
    class HMLFile
    {
    private:
        std::ifstream mFile;			//actual std::ifstream file object
        String mFilepath;				//path to the file
        std::string mToken;				//token string used for parsing the file (represents the current piece of text parsed in the file)


    public:
        HMLFile() {}
        ~HMLFile() {}

        //general
        bool Open();
        bool Open(String filepath);
        void Close();
        bool Reset();
		bool IsOpen() { return mFile.is_open(); }

        //read mechanics
		bool EndOfFile() { return mFile.eof(); }
        bool SeekElementOpen(String element);
        bool SeekElementClose(String element);
		bool AtElementOpen(String element);
        bool AtElementClose(String element);
        bool Seek();
        void SeekDeclaration();
        String SeekGetToken();
        bool TokenIsDeclaration(String dec);
        String GetToken()  { return mToken; }
    };

};

#endif

