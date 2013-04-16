/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: IO.h
=================================================================================================*/

#include "Utility.h"

#ifndef _ge_IO_H
#define _ge_IO_H

#ifndef uint64
#define uint64 unsigned long long
#endif

namespace ge
{
    namespace io
    {
        /*=============================================================================
        -- Types of paths enumeration.
        =============================================================================*/
        enum PathType
        {
            UNDEFINED = 0,
            FILE = 1,
            DIRECTORY = 2
        };


        /*=============================================================================
        -- Data units for defining the unit.
        =============================================================================*/
        enum DataUnit
        {
            BYTE = 0,		//byte
            KILOBYTE = 1,	//kilobyte
            MEGABYTE = 2,	//megabyte
            GIGABYTE = 3,	//gigabyte
            TERABYTE = 4,	//terabyte
			PENTABYTE = 5,	//pentabyte
			EXABYTE = 6		//exabyte
        };

        //simplified std::vector for listing directories
        typedef std::vector<String> DirectoryListing;


        /*=============================================================================
        -- Path class for representing paths.
        =============================================================================*/
        class DirectoryElement
        {
        protected:
            String mPath;					//path to the element
            uint64 mSize;					//calculated size of the element. Call CalculateSize() to update this value. This is always in bytes.
            bool mIsFile;					//true if single element, false if directory


        public:
            DirectoryElement() { mPath = "#NULL"; mIsFile = true; mSize = 0;}
            ~DirectoryElement() {}

            virtual uint64 CalculateSize();

            uint64 GetSize(bool calculate=true);
            void SetSize(int size) { mSize = size; }
            String GetPath() { return mPath; }
            void SetPath(String path) { mPath = path; }
            bool GetIsFile() { return mIsFile; }
            void SetIsFile(bool isFile) { mIsFile = isFile; }
        };


        /*=============================================================================
        -- Directory class for finding the contents of a directory.
        =============================================================================*/
        class Directory : public DirectoryElement
        {
        private:
            std::vector<DirectoryElement*> mElements;       //list of every element inside the directory. Call CalculateTotalSize() to update the list.

        public:
            Directory() { mIsFile = false; }
            ~Directory() {}

            uint64 CalculateSize();
            void PrintTree(int indent=0);
            DirectoryListing ExportDirectoryListing();
        };


        /*=============================================================================
        -- Helper functions to navigate and manage filesystems.
        =============================================================================*/
        String ShortenPath(String path);
        bool PathExists(String path);
        bool IsFile(String path);
        uint64 FileSize(String path);
        uint64 DirectorySize(String path);
        String GetExtension(String path);
        String RemoveExtension(String path);
        String AbsolutePath(String partial);
        PathType GetPathType(String path);
        String Standardize(String path);
        String TrailingSlash(String path);
        DirectoryListing GetFilesInDirectory(String directory, String extensionFilter="#any");
		double ConvertBytesTo(uint64 size, DataUnit dataUnit);
    };

};

#endif

