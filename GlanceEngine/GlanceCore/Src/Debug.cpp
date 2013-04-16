/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Debug.cpp
=================================================================================================*/

#include "Glance.h"

namespace ge
{
	DebugStream::DebugStream()
	{
		Init(true, "");
	}


	DebugStream::DebugStream(bool useCerrStream, String logFile)
	{
		Init(useCerrStream, logFile);
	}

	DebugStream::~DebugStream()
	{
		mLogFile.close();
	}


	bool DebugStream::Init(bool useCerrStream, String logFile)
	{
		mUseCerrStream = useCerrStream;
		SetLogFile(logFile);

		return true;
	}


	bool DebugStream::SetLogFile(String logFile)
	{
		mLogFileName = logFile;

		if (logFile != "")
		{
			mLogFile.open(logFile.GetStd().c_str(), std::ios::app);

			if (!mLogFile.is_open())
			{
				if (mUseCerrStream)
					std::cerr << "[error] Could not open log file \""+logFile.GetStd()+"\"" << std::endl;

				return false;
			}
		}
		else
			return false;

		return true;
	}

	bool DebugStream::Open()
	{
		if (mLogFile.is_open())
			return true;

		mLogFile.open(mLogFileName.GetStd().c_str(), std::ios::trunc);

		return mLogFile.is_open();
	}



	void DebugStream::AddDebugProtocol(WeakPtr<DebugProtocol> debugProtocol)
	{
		//ensure this debugProtocol is not already added
		std::vector< WeakPtr<DebugProtocol> >::iterator iter = mDebugProtocols.begin();
		while (iter != mDebugProtocols.end())
		{
			if ((*iter).lock() == debugProtocol.lock())
				return;

			iter++;
		}

		mDebugProtocols.push_back(debugProtocol);
	}


	void DebugStream::RemoveDebugProtocol(WeakPtr<DebugProtocol> debugProtocol)
	{
		std::vector< WeakPtr<DebugProtocol> >::iterator iter = mDebugProtocols.begin();
		while (iter != mDebugProtocols.end())
		{
			if ((*iter).lock() == debugProtocol.lock())
			{
				mDebugProtocols.erase(iter);
				return;
			}

			iter++;
		}
	}


	void DebugStream::Print(String text, Color color)
	{
		if (mUseCerrStream)
			std::cerr << text.GetStd() << std::endl;

		if (Open())
			mLogFile << text.GetStd() << std::endl;

		std::vector< WeakPtr<DebugProtocol> >::iterator iter = mDebugProtocols.begin();
		while (iter != mDebugProtocols.end())
		{
			(*iter).lock()->Print(text, color);

			iter++;
		}
	}


	void DebugStream::Error(String text, String functionAddress)
	{
		if (functionAddress == "")
			Print("[error] "+text.GetStd(), gRed);
		else
			Print("[error] ["+functionAddress.GetStd()+"] "+text.GetStd(), gRed);
	}


	void DebugStream::Warning(String text, String functionAddress)
	{
		if (functionAddress == "")
			Print("[warning] "+text.GetStd(), gYellow);
		else
			Print("[warning] ["+functionAddress.GetStd()+"] "+text.GetStd(), gYellow);
	}
};
