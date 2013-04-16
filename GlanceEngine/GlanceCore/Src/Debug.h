/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Debug.h
=================================================================================================*/

#ifndef _ge_DEBUG_H
#define _ge_DEBUG_H

#include "Glance.h"

namespace ge
{
	class DebugProtocol
	{
	public:
		virtual void Print(String text, Color color) = 0;
	};


	class DebugStream
	{
	private:
		bool mUseCerrStream;									//should debug output go through the std::cerr stream?
		String mLogFileName;									//name of output file on disk to direct output
		std::ofstream mLogFile;									//output file on disk to direct output
		std::vector< WeakPtr<DebugProtocol> > mDebugProtocols;	//DebugProtocol objects

	public:
		DebugStream();
		DebugStream(bool useCerrStream, String logFile="");
		~DebugStream();

		bool Init(bool useCerrStream, String logFile);
		bool SetLogFile(String logFile);
		bool Open();

		void AddDebugProtocol(WeakPtr<DebugProtocol> debugProtocol);
		void RemoveDebugProtocol(WeakPtr<DebugProtocol> debugProtocol);

		void Print(String text, Color color=gWhite);
		void Error(String text, String functionAddress="");
		void Warning(String text, String functionAddress="");
	};
};

#endif

