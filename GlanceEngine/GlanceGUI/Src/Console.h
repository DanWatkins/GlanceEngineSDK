/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Console.h
=================================================================================================*/
#include "GUI.h"

#ifndef _ge_CONSOLE_H
#define _ge_CONSOLE_H

namespace ge
{
	/*=============================================================================
	-- Line structure to contain the text and its color for a line.
	=============================================================================*/
	struct ConsoleLine
	{
		String text;
		Color color;
	};

	struct ConsoleCommand
	{
		String command;
		std::vector<String> parameters;	
	};


	/*=============================================================================
	-- Drop-down console for debugging and user input.
	=============================================================================*/
	class Console : public DebugProtocol
	{
	private:
		unsigned mWidth, mHeight;						//size of the console when visible
		unsigned mPadding, mBoxPadding;					//spacing between text and the bounds of the console. Spacing between input text and the bounds of the input box
		bool mVisible;									//false if it is hidden and does not take user input
		std::list<ConsoleLine> mLines;					//lines of text
		String mInputLine;								//text on the input line the user has entered
		Font *mFont;									//font to render text with
		Sprite mBackground;								//background sprite (not loaded from disk, just a flat color)
		SpriteFrame mInputBox;							//frame to outline the input box
		std::list<ConsoleCommand> mQueuedCommands;
		gui::Root mEmbeddedRoot;

		void _QueueCommand(ConsoleCommand command) { mQueuedCommands.push_back(command); }


	public:
		Console();
		~Console() {}

		void Init(unsigned width, unsigned height, Window *window);
		void Update();
		void Draw();
		
		bool Visible() { return mVisible; }
		void Print(String text, Color color=gWhite);
		void ToggleVisible() { mVisible = !mVisible; }
		bool CommandsQueued() { return (mQueuedCommands.size() > 0); }
		ConsoleCommand PumpCommand();
	};
};

#endif

