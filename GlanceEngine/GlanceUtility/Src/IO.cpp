/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: IO.h
=================================================================================================*/
#include "Utility.h"

namespace ge
{
    namespace io
    {
        /*=============================================================================
        -- Returns true if the path exists. If it can't find the path as is, it will
           convert it to an absolute path (regardless if it already is), and retry.
        =============================================================================*/
        bool PathExists(String path)
        {
			if (boost::filesystem::exists( AbsolutePath(path).GetStd() ))
                return true;

            return false;
        }


        /*=============================================================================
        -- Returns true if the file exists and is not a directory.
        =============================================================================*/
        bool IsFile(String path)
        {
			boost::filesystem::path p(path.GetStd());
            return boost::filesystem::is_regular_file(p);
        }


        /*=============================================================================
        -- Returns the size in bytes of the specified file.
        =============================================================================*/
        uint64 FileSize(String path)
        {
            if (PathExists(path))
				return (uint64)boost::filesystem::file_size( AbsolutePath(path).GetStd() );

            return 0;
        }


        /*=============================================================================
        -- Removes everything before and including the last '/' or '\'.
        =============================================================================*/
        String ShortenPath(String path)
        {
			//TODO I think it is actually linux and Mac that would complain about \, not windows caring about /
            #ifdef BUILD_WINDOWS
            //ensure that directory deliminators are all "/" and not "\"
            path.Replace("\\", "/", PROCESS_ALL);  //TODO shouldn't this be done for all systems?
            #endif

            path.RemoveUntil("/", PROCESS_ALL, INCLUDE_DELIMINATOR);

            return path;
        }


        /*=============================================================================
        -- Returns the size of all the files in the directory and its sub-directories.
        =============================================================================*/
        uint64 DirectorySize(String path)
        {
            //create directory object
            Directory mainDir;
            mainDir.SetPath(AbsolutePath(path));

			return mainDir.CalculateSize();
        }


        /*=============================================================================
        -- Returns the file extension of the path. #DIR if the path is a directory.
        =============================================================================*/
        String GetExtension(String path)
        {
			boost::filesystem::path p(path.GetStd());
            return String(boost::filesystem::extension(p));
        }


        /*=============================================================================
        -- Removes the extension so it is just the name/path.
        =============================================================================*/
        String RemoveExtension(String path)
        {
			String p1(path);
            p1.RemoveUntil(".");

            String p2(path);
            p2.Remove(p1, 1);

            return p2;
        }


        /*=============================================================================
        -- Returns the absolute path of the specified path (even if it is already
          and absoulte path).
        =============================================================================*/
        String AbsolutePath(String partial)
        {
			#ifdef BUILD_LINUX
            char *absolutePath = realpath(partial.GetStd().c_str(), NULL);
            return String(absolutePath);
			#endif


			#ifdef BUILD_WINDOWS
			char full[8192];
			if (_fullpath(full, partial.GetStd().c_str(), 4096) != NULL)
				return full;
			else
				return "";
			#endif
        }


        /*=============================================================================
        == Returns the type of path (directory, file, undefined).
        =============================================================================*/
        PathType GetPathType(String path)
        {
            PathType type = UNDEFINED;
            path = AbsolutePath(path);

            if (PathExists(path))
            {
				if (boost::filesystem::is_regular_file(path.GetStd()))
                    type = FILE;
				else if (boost::filesystem::is_directory(path.GetStd()))
                    type = DIRECTORY;
            }

            return type;
        }


        /*=============================================================================
        -- Directory class for finding the contents of a directory.
        =============================================================================*/
		uint64 Directory::CalculateSize()
        {
            mPath = AbsolutePath(mPath);

			//make sure this is a valid path
			if (!PathExists(mPath))
				return 0;

            //reset the size
            mSize = 0;

			boost::filesystem::path p(mPath.GetChar());
            typedef std::vector<boost::filesystem::path> vec;
            vec storahx;

            copy(boost::filesystem::directory_iterator(p), boost::filesystem::directory_iterator(), back_inserter(storahx));

            for (vec::iterator it(storahx.begin()); it != storahx.end(); ++it)
            {
                //if it is a file
                if (io::GetPathType((*it).string()) == FILE)
                {
                    DirectoryElement *element = new DirectoryElement();
                    element->SetIsFile(true);
                    element->SetPath((*it).string());
                    mSize += (int)(element->GetSize());
                    mElements.push_back(element);

                }
                //or if it is a directory
                else if (io::GetPathType((*it).string()) == DIRECTORY)
                {
                    Directory *directory = new Directory();
                    directory->SetIsFile(false);
                    directory->SetPath((*it).string());
                    mSize += (int)directory->GetSize();
                    mElements.push_back(directory);

                }
            }

			return mSize;
        }


        /*=============================================================================
        -- Takes all of the sub-directories and files inside the directory and puts
           there path's into the std::vector string container DirectoryListing.
        =============================================================================*/
        DirectoryListing Directory::ExportDirectoryListing()
        {
            DirectoryListing listing;

            CalculateSize();

            for (unsigned n=0; n<mElements.size(); n++)
            {
                listing.push_back(mElements.at(n)->GetPath());
            }

            return listing;
        }


        /*=============================================================================
        -- Returns the size of the element in the specified unit.
        =============================================================================*/
        void Directory::PrintTree(int indent)
        {
            //apply indent amount
            std::string indentExtra = "";
            for (int n=0; n<indent; n++)
            {
                indentExtra.append("   ");
            }
            indent++;

            //print header
			std::cout << indentExtra << mPath.GetStd() << " is " << GetSize(KILOBYTE) << "kb" << std::endl;

            indentExtra.append("   ");

            //print out all sub paths
            std::vector<DirectoryElement*>::iterator iter = mElements.begin();
            while (iter != mElements.end())
            {
                if ( (*iter)->GetIsFile() )
					std::cout << indentExtra << (*iter)->GetPath().GetStd() << " is " << (*iter)->GetSize(KILOBYTE) << "kb" << std::endl;
                else
                    ((Directory*)*iter)->PrintTree(indent);

                iter++;
            }
        }


        /*=============================================================================
        -- Returns the size of the element in the specified unit.
        =============================================================================*/
        uint64 DirectoryElement::GetSize(bool calculate)
        {
			if (calculate)
				return CalculateSize();

            return 0;
        }


        /*=============================================================================
        -- Calculates the size of itself if it's a file, and everything in it if a dir.
        =============================================================================*/
        uint64 DirectoryElement::CalculateSize()
        {
            //if this is a file
            if (mIsFile)
                mSize = FileSize(mPath);

			return mSize;
        }


        /*=============================================================================
        -- Converts every "\" to "/".
        =============================================================================*/
        String Standardize(String path)
        {
            path.Replace("\\", "/", PROCESS_ALL);
			return path;
        }


        /*=============================================================================
        -- Adds a trailing slash if there isn't already one.
        =============================================================================*/
        String TrailingSlash(String path)
        {
            path = Standardize(path);
            path.Remove(0, path.Size()-1);

            if (path != "/")
				path.PushBack( Combine("/", "") );

			return path;
        }


        /*=============================================================================
        -- Returns a DirectoryListing containing all of the files in the immediate
           @directory that have extensions matching @extensionFilter.
        =============================================================================*/
        DirectoryListing GetFilesInDirectory(String directory, String extensionFilter)
        {
            Directory dir;
            DirectoryListing listing, listingFiltered;

            //populate the list with every file in the immediate directory
            dir.SetPath(io::AbsolutePath(directory));
            listing = dir.ExportDirectoryListing();

            //filter out files without the right extension
            DirectoryListing::iterator iter = listing.begin();
            while (iter != listing.end())
            {
                if ( GetExtension(*iter) == extensionFilter )
                    listingFiltered.push_back(*iter);
                else if ( extensionFilter == "#any" )
                {
                    if (IsFile(*iter))
                        listingFiltered.push_back(*iter);
                }

                iter++;
            }

            return listingFiltered;
        }


		/*=============================================================================
		-- Converts from bytes to the data unit.
		=============================================================================*/
		double ConvertBytesTo(uint64 size, DataUnit dataUnit)
		{
			if (dataUnit == BYTE)
                return (double)size;
            else if (dataUnit == KILOBYTE)
                return size/1024.0;				// size/2^10
            else if (dataUnit == MEGABYTE)
                return size/1048576.0;			// size/2^20
            else if (dataUnit == GIGABYTE)
                return size / 1073741824.0;		// size/2^30
            else if (dataUnit == TERABYTE)
                return size/(1099511627776.0);	// size/2^40
            else
                return 0.0;
		}
    };

};