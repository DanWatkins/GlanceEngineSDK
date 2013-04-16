/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: GUI.h
=================================================================================================*/
#include "../../GlanceCore/Src/Glance.h"
#include "../../GlanceAsset/Src/Asset.h"

#ifndef _ge_GUI_H
#define _ge_GUI_H

#undef DialogBox

namespace ge
{
	namespace gui
	{
		//forward declarations
		class Element;
		class Root;

		class DialogBox;
		class GroupBox;

		class Drager;
		class Cell;

		class ButtonCaption;
		class ButtonImage;
		class CheckBox;
		class EditBox;
		class Slider;
		class ScrollBar;

		class ContextMenu;
		class MenuBar;
		class ListBox;
		class TabBar;
		class TabPage;
		class ComboBox;

		class Picture;
		class ProgressBar;
		class Text;

		
		enum TextAlignment
		{
			AlignedLeft,
			AlignedRight
		};


		/*=============================================================================
		-- Stores the sate an Element is in.
		=============================================================================*/
		enum State
		{
			UNDEFINED = 0,
			DOWN = 1,
			UP = 2,
			RELEASED = 3,
			CHECKED = 4,
			UNCHECKED = 5
		};


		/*=============================================================================
		-- Messages to be sent to Elements by the Root.
		=============================================================================*/
		enum Message
		{
			CURSOR_OVER = 0,				//cursor is over the element's outline
			MBL_DOWN_OVER = 1,				//left mouse button is pressed and cursor is over element's outline
			MBR_DOWN_OVER = 2,				//right mouse button is pressed and cursor is over element's outline
			MBC_DOWN_OVER = 3,				//center mouse button is pressed and cursor is over element's outline
			HAS_FOCUS = 4					//when the Element was just given focus
		};
	};
};

//directory definitions
#define GUI_DIR_BUTTON "/button/"


//asset ids (GAID == GUI ASSET ID)
#define GAID_CHECKED				"checked.png"
#define GAID_UNCHECKED				"unchecked.png"
#define GAID_CHECKED_DISABLED		"checked_disabled.png"
#define GAID_UNCHECKED_DISABLED		"unchecked_disabled.png"
#define GAID_CARET					"caret.png"
#define GAID_BAR					"bar.png"


//miscalcuaneous parameters
#define GUI_GROUPBOX_CAPTION_OFFSET			10
#define GUI_GROUPBOX_CAPTION_PAD_LEFT		3
#define GUI_GROUPBOX_CAPTION_PAD_RIGHT		2
#define GUI_EDIT_TEXT_PAD					5
#define GUI_PROGRESSBAR_TEXT_PAD			5
#define GUI_PROGRESSBAR_PAD					3
#define GUI_DIALOG_TITLE_PAD				8
#define GUI_MENU_BORDER_PAD					3
#define GUI_MENU_PADV						2
#define GUI_MENU_TEXT_PADH					6
#define GUI_MENU_MIN_WIDTH					100
#define GUI_DEFAULT_MENU_HEIGHT				24

#define GUI_CARET_DELAY						400  //milliseconds


#include "Element.h"


#include "IntegratedElementIds.h"
#include "Elements/Helpers/Drager.h"


#include "Elements/Containers/DialogBox.h"
#include "Elements/Containers/GroupBox.h"
#include "Elements/Containers/TabBar.h"
#include "Elements/Containers/TabPage.h"

#include "Elements/Controls/ButtonCaption.h"
#include "Elements/Controls/ButtonImage.h"
#include "Elements/Controls/CheckBox.h"
#include "Elements/Controls/EditBox.h"
#include "Elements/Controls/Slider.h"
#include "Elements/Controls/ScrollBar.h"

#include "Elements/Selectors/ContextMenu.h"
#include "Elements/Selectors/MenuBar.h"
#include "Elements/Statics/Picture.h"
#include "Elements/Statics/ProgressBar.h"
#include "Elements/Statics/Text.h"

#include "Elements/Helpers/Cell.h"
#include "Elements/Selectors/ListBox.h"
#include "Elements/Selectors/ComboBox.h"

#include "Root.h"

#include "Console.h"

#endif

