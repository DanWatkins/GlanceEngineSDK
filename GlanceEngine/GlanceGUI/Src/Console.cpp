/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Console.cpp
=================================================================================================*/
#include "GUI.h"

namespace ge
{
	/*=============================================================================
	-- Constructor for Console.
	=============================================================================*/
	Console::Console()
	{
		mWidth		= 800;
		mHeight		= 200;
		mPadding	= 5;
		mBoxPadding	= 8;
		mVisible	= false;
		mFont		= NULL;
	}


	/*=============================================================================
	-- Initializes the console to a certain size, and sets the window and font.
	=============================================================================*/
	void Console::Init(unsigned width, unsigned height, Window *window)
	{
		mWidth = width;
		mHeight = height;

		mEmbeddedRoot.Init(window);
		mFont = mEmbeddedRoot.GetDefaultFont();
		mFont->SetWindow(window);

		//initialize the background image
		mBackground.SetWindow(window);
		mBackground.LoadImageFromFile(ToString(DIR_GUI)+"console_bkg.png");
		mBackground.SetColor(Color(0, 0, 0, 128));
		mBackground.SetSize(width, height);

		//initialize the input box image
		if (mFont)
		{
			mInputBox.Init(width, mFont->GetCharHeightPx()+mBoxPadding, ToString(DIR_GUI)+"console_box/", window, false);
			mInputBox.SetPos(0, height-(mFont->GetCharHeightPx()+mBoxPadding));
		}
	}


	/*=============================================================================
	-- Updates the text entered and the command buffer.
	=============================================================================*/
	void Console::Update()
	{
		if (mVisible)
		{
			//append any new text input to @mInputLine
			mInputLine.PushBack(mEmbeddedRoot.GetWindow()->GetInput()->GetTextEntered());
			mInputLine.ApplyOperations();

			//add the input to text lines when enter is pressed
			if (mEmbeddedRoot.GetWindow()->GetInput()->GetKeyState(GK_Enter, KEY_RELEASED))
			{
				Print(mInputLine, gWhite);

				if (mInputLine.Size() > 0  &&  mInputLine[0] != GUI_CONSOLE_NO_COMMAND_PREFIX)
				{
					//parse @mInputLine as a command
					ConsoleCommand command;
					String buffer;

					for (int n=0; n<mInputLine.Size(); n++)
					{
						if (mInputLine[n] != " ")
							buffer.PushBack(mInputLine[n]);
						else
						{
							if (command.command == "")
								command.command = buffer;
							else
								command.parameters.push_back(buffer);

							buffer.Clear();
						}
					}

					command.parameters.push_back(buffer);
					mQueuedCommands.push_back(command);
					Print(ToString("   ")+GUI_CONSOLE_NO_COMMAND_PREFIX+"Command \""+command.command+"\" was parsed",gYellow);
				}

				mInputLine = "";
			}
		}
	}


	/*=============================================================================
	-- Draws the console and all the visible text.
	=============================================================================*/
	void Console::Draw()
	{
		if (mVisible)
		{
			//make sure the window and font are valid
			if (!mEmbeddedRoot.GetWindow() || !mFont)
				return;

			mBackground.Draw();
			mInputBox.Draw();

			//draw the visible text lines
			float spaceFactor = 1.1f;
			int textHeight = mFont->GetCharHeightPx();
			float y = mHeight-(textHeight*spaceFactor)-mInputBox.GetHeight();

			std::list<ConsoleLine>::iterator iter = mLines.begin();
			while (iter != mLines.end())
			{
				//break if the next line will be drawn off screen
				if (y < -(textHeight*spaceFactor))
					break;

				mFont->Print(iter->text, mPadding, (int)y, iter->color);
			
				y -= textHeight*spaceFactor;
				iter++;
			}

			//draw the input line
			mFont->Print(mInputLine, mBoxPadding, mHeight-mBoxPadding-mFont->GetCharHeightPx(), gWhite);
		}
	}


	/*=============================================================================
	-- Prints a new line to the console (not visible until Draw() is called again).
	=============================================================================*/
	void Console::Print(String text, Color color)
	{
		ConsoleLine newLine;
		newLine.text = text;
		newLine.color = color;
		mLines.push_front(newLine);

		//process and add the printed line to the command queue
		std::vector<String> commandSections = text.GetSectionsDeliminatedBy(" ");

		if (commandSections.size() > 0)
		{
			ConsoleCommand command;
			command.command = commandSections.at(0);

			for (unsigned n=1; n<commandSections.size(); n++)
				command.parameters.push_back(commandSections.at(n));
			
			_QueueCommand(command);
		}
	}


	ConsoleCommand Console::PumpCommand()
	{
		ConsoleCommand command = mQueuedCommands.front();
		mQueuedCommands.pop_front();

		return command;
	}
};