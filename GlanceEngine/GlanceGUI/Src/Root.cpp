/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Root.cpp
=================================================================================================*/

#include "GUI.h"

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- Constructor for Root.
		=============================================================================*/
		Root::Root()
		{
			std::memset(&mConsecutiveButtonPress, false, sizeof(mConsecutiveButtonPress));

			mDefaultFont					= NULL;
			mSkipMouseMessagesThisUpdate	= false;
		}


		/*=============================================================================
		-- Initializes the Root.
		=============================================================================*/
		bool Root::Init(Window *window)
		{
			_SetWindow(window);

			LoadFonts();

			CreateText(-2, -1, Vector2D<int>(), "");
			mDefaultText = DynamicPtrCast<Text>(GetChild(-2).lock());

			//load all the asset directories listied in the config.ini file
			std::fstream file;
			file.open( Combine(DIR_GUI, "config.ini" ).GetChar());
			if (file.is_open())
			{
				while (!file.eof())
				{
					std::string type, line;
					file >> type;
					file >> line;

					//load a frame image
					if (type == "FRAME")
					{
						//what is the id for the frame?
						std::string id;
						file >> id;

						//load the frame image
						FrameImage *frameImage = new FrameImage(id);
						frameImage->LoadFromFile(DIR_GUI+line);
						mAssetManager.AddAsset(frameImage);
					}
					//load the entire asset directory
					else if (type == "REGULAR")
						mAssetManager.LoadAssetsFromDirectory(DIR_GUI+line);
				}
			}

			return (window != NULL);
		}


		/*=============================================================================
		  Loads every font file in @fontDir and adds it to @mFontList.
		=============================================================================*/
		void Root::LoadFonts(String fontDir)
		{
			io::DirectoryListing list;
			list = io::GetFilesInDirectory(fontDir, ".ini");

			//load every font in the list
			io::DirectoryListing::iterator iter = list.begin();
			while (iter != list.end())
			{
				TextReference<Font*> font;
				font.ref = new Font();

				//remove the path and extension
				font.name = io::ShortenPath(*iter);
				font.name = io::RemoveExtension(font.name);

				//load the new font, set it up, and add it to the list
				if (!font.ref->LoadFontFromFile(font.name))
					continue;
				font.ref->SetWindow(GetWindow());

				//add it to the list
				mFontList.push_back(font);

				//if this is the default font, make it that
				if (font.name == DEFAULT_FONT)
					mDefaultFont = mFontList.back().ref;

				iter++;
			}

			//if the default font file was not loaded, make the first font loaded the default
			if (!mDefaultFont)
			{
				if (mFontList.size() > 0)
					mDefaultFont = mFontList.front().ref;
			}
		}


		/*=============================================================================
		  Searches for a font with @name and then returns a pointer to it.
		=============================================================================*/
		Font *Root::GetFont(String name)
		{
			std::list< TextReference<Font*> >::iterator iter = mFontList.begin();
			while (iter != mFontList.end())
			{
				if ( (*iter).name == name )
					return (*iter).ref;

				iter++;
			}

			return NULL;
		}


		/*=============================================================================
		-- Updates the entire element tree below the Root.
		=============================================================================*/
		void Root::_Update()
		{
			_UpdateChildren();

			//mosue messages must be processed after the children have been updated
			//buttons switch from DOWN to UP during the update process. If it is
			//told to be DOWN, then switches from DOWN to UP in its update process,
			//all external systems will be unaware that the button was ever down
			HandleMouseMessages();

			SetSkipMouseMessagesThisUpdate(false);
		}


		/*=============================================================================
		-- Draws the entire element tree below the Root.
		=============================================================================*/
		void Root::_Draw()
		{
			_DrawChildren();
		}


		/*=============================================================================
		-- Searches every element in the tree below Root for the element with @id.
		=============================================================================*/
		WeakPtr<Element> Root::GetElement(int id)
		{
			return _GetElementSearch(id);
		}


		/*=============================================================================
		-- Determines every element that needs a message about the mouse's position
		   and button status.
		=============================================================================*/
		void Root::HandleMouseMessages()
		{
			if (mSkipMouseMessagesThisUpdate == false)
			{
				//get the position and button status of the mouse
				unsigned x = (unsigned)GetWindow()->GetInput()->GetMouseX();
				unsigned y = (unsigned)GetWindow()->GetInput()->GetMouseY();
				Vector2D<int> cursor_pos(x, y);
				bool mbl_isDown = GetWindow()->GetInput()->GetButtonState(GMB_Left);
				bool mbr_isDown = GetWindow()->GetInput()->GetButtonState(GMB_Right);
				bool mbc_isDown = GetWindow()->GetInput()->GetButtonState(GMB_Center);

				//get a list every element that the cursor is over
				std::vector< SharedPtr<Element> > relevantElements = _GetAllDerivedElementsWithIntersectingPoint(cursor_pos);

				//get the top-most solid element under the cursor
				SharedPtr<Element> e;
				std::vector< SharedPtr<Element> >::iterator iter = relevantElements.begin();
				while (iter != relevantElements.end())
				{
					if ((*iter)->GetSolid())
					{
						e = *iter;
						break;
					}

					iter++;
				}

				//was there at least one element under the cursor that was solid?
				if (e)
				{
					//tell only the top-most element if the mouse is pressed over it, or if it is just over it (hover effect)
					e->_SendMessage(CURSOR_OVER);

					//notify Elements about focus changes
					if (mbl_isDown || mbr_isDown || mbc_isDown)
					{
						//let it know it has the current focus (only if the left mouse button was pressed)
						_SetAllBelow_HasFocus(false);
						e->BringToFocus();
					}

					if (mbl_isDown && !mConsecutiveButtonPress[0])
						e->_SendMessage(MBL_DOWN_OVER);
					if (mbr_isDown && mConsecutiveButtonPress[1])
						e->_SendMessage(MBR_DOWN_OVER);
					if (mbc_isDown && mConsecutiveButtonPress[2])
						e->_SendMessage(MBC_DOWN_OVER);

					mElementInFocus = e;
				}
				//there was not an element under the cursor, so set hasFocus for all elements to false if the lmb was pressed
				else if (mbl_isDown)
					_SetAllBelow_HasFocus(false);

				//save the consecutive button press information
				if (mbl_isDown)
					mConsecutiveButtonPress[0] = true;
				else
					mConsecutiveButtonPress[0] = false;

				if (mbr_isDown)
					mConsecutiveButtonPress[1] = true;
				else
					mConsecutiveButtonPress[1] = false;

				if (mbc_isDown)
					mConsecutiveButtonPress[2] = true;
				else
					mConsecutiveButtonPress[2] = false;
			}

			while (_BringAllChildrenScheduledToBeBroughtToFocusToFocus() == false);

			_RemoveAllChildrenScheduledToBeRemoved();
			_UpdateTabbing();
		}


		WeakPtr<Element> Root::GetHighestElementAtPoint(Vector2D<int> point)
		{
			std::vector< SharedPtr<Element> > relevantElements = _GetAllDerivedElementsWithIntersectingPoint(point);

			//was there at least one element under the point?
			if (relevantElements.size() > 0)
				return WeakPtr<Element>(relevantElements.front());

			return WeakPtr<Element>();
		}


		/*=============================================================================
		-- Returns true if the button has been pressed since the last frame or before
		   without being released.
		=============================================================================*/
		bool Root::GetConsecutiveButtonPress(int button)
		{
			switch(button)
			{
			case GMB_Left:
				return mConsecutiveButtonPress[0];

			case GMB_Right:
				return mConsecutiveButtonPress[1];

			case GMB_Center:
				return mConsecutiveButtonPress[2];
			}

			return false;
		}


		void Root::_UpdateTabbing()
		{
			if (mElementInFocus.expired())
				return;

			//reconize TAB or CTRL+TAB to switch elements
			Input *input = GetWindow()->GetInput();
			bool tabDown = input->GetKeyState(GK_Tab, KEY_DOWN);
			bool ctrlDown = input->GetKeyState(GK_LCtrl, KEY_DOWN) || input->GetKeyState(GK_RCtrl);

			if (tabDown)
			{
				std::vector< SharedPtr<Element> > children;
				if (mElementInFocus.lock()->GetParent().expired())
					children = this->mChildren;
				else
					children = mElementInFocus.lock()->GetParent().lock()->mChildren;

				//get the iter to the current element in focus
				std::vector< SharedPtr<Element> >::iterator iter = children.begin();
				while (iter != children.end())
				{
					if ((*iter) == mElementInFocus.lock())
						break;

					iter ++;
				}

				std::vector< SharedPtr<Element> >::iterator startIter = iter;

				if (ctrlDown)
				{
					//TODO add backwards tabbing
				}
				else
				{
					do  //forward tabbing between elements
					{
						iter++;

						if (iter == children.end())
							iter = children.begin();

						if ((*iter)->GetCanFocus())
						{
							(*iter)->BringToFocus();
							mElementInFocus = *iter;
							break;
						}

					} while (iter != startIter);
				}
			}
		}


		/*=============================================================================
		-- Returns true if the id value is available. If the id is less than zero,
		   only elements that are direct children of the parent will be checked for the
		   same id value.
		=============================================================================*/
		bool Root::_ValidId(int id, WeakPtr<Element> parent)
		{
			//this is a direct descendant of the root (parent is NULL)
			if (parent.expired())
			{
				if (!GetElement(id).expired())
				{
					gDebug.Error( Combine("Could not make the requested Text bcause an element with id=", ToString(id), " alredy exists!") );
					return false;
				}
			}

			//make sure there is not already an element with this id
			else if (id >= 0)
			{
				if (!GetElement(id).expired())
				{
					gDebug.Error( Combine("Could not make the requested Text bcause an element with id=", ToString(id), " alredy exists!") );
					return false;
				}
			}

			//the id is under 0, meaning it is integrated, so only search the parent's direct children
			else
			{
				if (!parent.lock()->GetChild(id).expired())
				{
					gDebug.Error( Combine("Could not make the requested Text bcause an element with id=", ToString(id), " alredy exists!") );
					return false;
				}
			}

			return true;
		}


		WeakPtr<Element> Root::_ProcessNewElement(WeakPtr<Element> element, int elementId, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width, unsigned height)
		{
			if (_ValidId(elementId, parent))
			{
				if (!element.lock()->_Init(elementId, parent, this, pos, width, height, GetWindow()))
					gDebug.Error(String()+"Error initializing element with id"+ToString(elementId));

				//add the new element to the parent (or root)
				if (!parent.expired())
					parent.lock()->_AddChild(element);
				else
					_AddChild(element);

				//post initialize
				if (!element.lock()->_PostInit())
					gDebug.Error(String()+"Error post-initializing element with id"+ToString(elementId));

				element.lock()->_UpdateAssets();
			}

			return element;
		}


		/*=============================================================================
		-- Creates a neww Picture and gives it to the element @parent.
		=============================================================================*/
		WeakPtr<Picture> Root::CreatePicture(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width, unsigned height, Image *image)
		{
			SharedPtr<Picture> picture(new Picture);
			

			picture = DynamicPtrCast<Picture>(_ProcessNewElement(picture, id, parent, pos, width, height).lock());
			picture->SetImage(image);

			return picture;
		}

		WeakPtr<Picture> Root::CreatePicture(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width, unsigned height, String filepath)
		{
			SharedPtr<Picture> picture(new Picture);
			picture->SetFilePath(filepath);

			return DynamicPtrCast<Picture>(_ProcessNewElement(picture, id, parent, pos, width, height).lock());
		}

		WeakPtr<Picture> Root::CreatePicture(int id, int parent, Vector2D<int> pos, unsigned width, unsigned height, String filepath)
		{
			WeakPtr<Element> sParent = GetElement(parent).lock();

			return CreatePicture(id, sParent, pos, width, height, filepath);
		}


		/*=============================================================================
		-- Creates a new ButtonCaption and gives it to the element @parent.
		=============================================================================*/
		WeakPtr<ButtonCaption> Root::CreateButtonCaption(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width, unsigned height, String caption)
		{
			SharedPtr<ButtonCaption> button(new ButtonCaption);
			button->SetCaption(caption);

			return DynamicPtrCast<ButtonCaption>(_ProcessNewElement(button, id, parent, pos, width, height).lock());
		}

		WeakPtr<ButtonCaption> Root::CreateButtonCaption(int id, int parent, Vector2D<int> pos, unsigned width, unsigned height, String caption)
		{
			WeakPtr<Element> sParent = GetElement(parent).lock();

			return CreateButtonCaption(id, sParent, pos, width, height, caption);
		}


		/*=============================================================================
		-- Creates a new ButtonImage and gives it to the element @parent.
		=============================================================================*/
		WeakPtr<ButtonImage> Root::CreateButtonImage(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width, unsigned height, String imageName)
		{
			SharedPtr<ButtonImage> button(new ButtonImage);
			button->_SetImageName(imageName);

			return DynamicPtrCast<ButtonImage>(_ProcessNewElement(button, id, parent, pos, width, height).lock());
		}

		WeakPtr<ButtonImage> Root::CreateButtonImage(int id, int parent, Vector2D<int> pos, unsigned width, unsigned height, String imageName)
		{
			WeakPtr<Element> sParent = GetElement(parent).lock();

			return CreateButtonImage(id, sParent, pos, width, height, imageName);
		}


		/*=============================================================================
		-- Creates a new Text element and gives it to @parent.
		=============================================================================*/
		WeakPtr<Text> Root::CreateText(int id, WeakPtr<Element> parent, Vector2D<int> pos, String text)
		{
			SharedPtr<Text> textObj(new Text);
			textObj->SetFont(GetDefaultFont());
			textObj->SetText(text);

			return DynamicPtrCast<Text>(_ProcessNewElement(textObj, id, parent, pos, 0, 0).lock());
		}

		WeakPtr<Text> Root::CreateText(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width, String text, TextAlignment textAlignment)
		{
			WeakPtr<Text> textObj = CreateText(id, parent, pos, text);
			textObj.lock()->SetTextAlignment(textAlignment);
			textObj.lock()->SetSize(width, 0);

			return textObj;
		}

		WeakPtr<Text> Root::CreateText(int id, int parent, Vector2D<int> pos, String text)
		{
			WeakPtr<Element> sParent = GetElement(parent).lock();

			return CreateText(id, sParent, pos, text);
		}


		/*=============================================================================
		-- Creates a new CheckBox element and gives it to @parent.
		=============================================================================*/
		WeakPtr<CheckBox> Root::CreateCheckBox(int id, WeakPtr<Element> parent, Vector2D<int> pos, String caption)
		{
			SharedPtr<CheckBox> checkBoxObj(new CheckBox);
			checkBoxObj->SetCaption(caption);

			return DynamicPtrCast<CheckBox>(_ProcessNewElement(checkBoxObj, id, parent, pos, 0, 0).lock());
		}

		WeakPtr<CheckBox> Root::CreateCheckBox(int id, int parent, Vector2D<int> pos, String caption)
		{
			WeakPtr<Element> sParent = GetElement(parent).lock();

			return CreateCheckBox(id, sParent, pos, caption);
		}


		/*=============================================================================
		-- Creates a new GroupBox element and gives it to @parent.
		=============================================================================*/
		WeakPtr<GroupBox> Root::CreateGroupBox(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width, unsigned height, String caption)
		{
			SharedPtr<GroupBox> groupBox(new GroupBox);

			groupBox->SetCaption(caption);

			return DynamicPtrCast<GroupBox>(_ProcessNewElement(groupBox, id, parent, pos, width, height).lock());
		}

		WeakPtr<GroupBox> Root::CreateGroupBox(int id, int parent, Vector2D<int> pos, unsigned width, unsigned height, String caption)
		{
			WeakPtr<Element> sParent = GetElement(parent).lock();

			return CreateGroupBox(id, sParent, pos, width, height, caption);
		}


		/*=============================================================================
		-- Creates a new EditBox element and gives it to @parent.
		=============================================================================*/
		WeakPtr<EditBox> Root::CreateEditBox(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width, unsigned height, String text)
		{
			SharedPtr<EditBox> editBox(new EditBox);
			editBox->SetCaption(text);

			return DynamicPtrCast<EditBox>(_ProcessNewElement(editBox, id, parent, pos, width, height).lock());
		}


		WeakPtr<EditBox> Root::CreateEditBox(int id, int parent, Vector2D<int> pos, unsigned width, unsigned height, String text)
		{
			WeakPtr<Element> sParent = GetElement(parent).lock();

			return CreateEditBox(id, sParent, pos, width, height, text);
		}


		/*=============================================================================
		-- Creates a new ProgressBar element and gives it to @parent.
		=============================================================================*/
		WeakPtr<ProgressBar> Root::CreateProgressBar(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width, unsigned height, unsigned percent)
		{
			SharedPtr<ProgressBar> progressBar(new ProgressBar);
			progressBar->SetPercent(percent);

			return DynamicPtrCast<ProgressBar>(_ProcessNewElement(progressBar, id, parent, pos, width, height).lock());
		}


		WeakPtr<ProgressBar> Root::CreateProgressBar(int id, int parent, Vector2D<int> pos, unsigned width, unsigned height, unsigned percent)
		{
			WeakPtr<Element> sParent = GetElement(parent).lock();

			return CreateProgressBar(id, sParent, pos, width, height, percent);
		}


		/*=============================================================================
		-- Creates a new Drager element and gives it to @parent.
		=============================================================================*/
		WeakPtr<Drager> Root::CreateDrager(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width, unsigned height, bool dragsHorizontal, bool dragsVertical)
		{
			SharedPtr<Drager> drager(new Drager);
			drager->SetDragsHorizontal(dragsHorizontal);
			drager->SetDragsVertical(dragsVertical);

			return DynamicPtrCast<Drager>(_ProcessNewElement(drager, id, parent, pos, width, height).lock());
		}

		WeakPtr<Drager> Root::CreateDrager(int id, int parent, Vector2D<int> pos, unsigned width, unsigned height, bool dragsHorizontal, bool dragsVertical)
		{
			WeakPtr<Element> sParent = GetElement(parent).lock();

			return CreateDrager(id, sParent, pos, width, height, dragsHorizontal, dragsVertical);
		}


		/*=============================================================================
		-- Creates a new Cell element and gives it to @parent, which has to be
		   a ListBox.
		=============================================================================*/
		WeakPtr<Cell> Root::CreateCell(int id, WeakPtr<ListBox> parent, Vector2D<int> pos, unsigned width, unsigned height)
		{
			SharedPtr<Cell> cell(new Cell);

			return DynamicPtrCast<Cell>(_ProcessNewElement(cell, id, parent, pos, width, height).lock());
		}

		WeakPtr<Cell> Root::CreateCell(int id, int parent, Vector2D<int> pos, unsigned width, unsigned height)
		{
			WeakPtr<ListBox> parentAsListBox = DynamicPtrCast<ListBox>(GetElement(parent).lock());

			//check if the parent was actually a list box
			if (!parentAsListBox.expired())
				return CreateCell(id, parentAsListBox, pos, width, height);
			else
				return WeakPtr<Cell>();
		}


		/*=============================================================================
		-- Creates a new DialogBox element and gives it to @parent.
		=============================================================================*/
		WeakPtr<DialogBox> Root::CreateDialogBox(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width, unsigned height, String caption)
		{
			SharedPtr<DialogBox> dialogBox(new DialogBox);
			WeakPtr<DialogBox> boxPtr = DynamicPtrCast<DialogBox>(_ProcessNewElement(dialogBox, id, parent, pos, width, height).lock());
			boxPtr.lock()->SetCaption(caption);

			return boxPtr;
		}

		WeakPtr<DialogBox> Root::CreateDialogBox(int id, int parent, Vector2D<int> pos, unsigned width, unsigned height, String caption)
		{
			WeakPtr<Element> sParent = GetElement(parent).lock();

			return CreateDialogBox(id, sParent, pos, width, height, caption);
		}


		/*=============================================================================
		-- Creates a new ContextMenu element and gives it to @parent.
		=============================================================================*/
		WeakPtr<ContextMenu> Root::CreateContextMenu(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width, unsigned height, String caption, Rect<int> rightClickRegion)
		{
			SharedPtr<ContextMenu> contextMenu(new ContextMenu);

			contextMenu->SetCaption(caption);
			contextMenu->SetRightClickRegion(rightClickRegion);

			return DynamicPtrCast<ContextMenu>(_ProcessNewElement(contextMenu, id, parent, pos, width, height).lock());
		}


		/*=============================================================================
		-- Creates a new ContextMenu element and gives it to @parent.
		=============================================================================*/
		WeakPtr<ContextMenu> Root::CreateContextMenu(int id, int parent, Vector2D<int> pos, unsigned width, unsigned height, String caption, Rect<int> rightClickRegion)
		{
			WeakPtr<Element> sParent = GetElement(parent).lock();

			return CreateContextMenu(id, sParent, pos, width, height, caption, rightClickRegion);
		}


		/*=============================================================================
		-- Creates a new MenuBar element and gives it to @parent.
		=============================================================================*/
		WeakPtr<MenuBar> Root::CreateMenuBar(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width)
		{
			SharedPtr<MenuBar> menuBar(new MenuBar);

			return DynamicPtrCast<MenuBar>(_ProcessNewElement(menuBar, id, parent, pos, width, GUI_DEFAULT_MENU_HEIGHT).lock());
		}

		WeakPtr<MenuBar> Root::CreateMenuBar(int id, int parent, Vector2D<int> pos, unsigned width)
		{
			WeakPtr<Element> sParent = GetElement(parent).lock();

			return CreateMenuBar(id, sParent, pos, width);
		}


		/*=============================================================================
		-- Creates a new ListBox element and gives it to @parent.
		=============================================================================*/
		WeakPtr<ListBox> Root::CreateListBox(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width, unsigned height)
		{
			SharedPtr<ListBox> listBox(new ListBox);

			return DynamicPtrCast<ListBox>(_ProcessNewElement(listBox, id, parent, pos, width, height).lock());
		}

		WeakPtr<ListBox> Root::CreateListBox(int id, int parent, Vector2D<int> pos, unsigned width, unsigned height)
		{
			WeakPtr<Element> sParent = GetElement(parent).lock();

			return CreateListBox(id, sParent, pos, width, height);
		}


		/*=============================================================================
		-- Creates a new ComboBox element and gives it to @parent.
		=============================================================================*/
		WeakPtr<ComboBox> Root::CreateComboBox(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width)
		{
			SharedPtr<ComboBox> comboBox(new ComboBox);

			return DynamicPtrCast<ComboBox>(_ProcessNewElement(comboBox, id, parent, pos, width, COMBOBOX_HEIGHT).lock());
		}


		WeakPtr<ComboBox> Root::CreateComboBox(int id, int parent, Vector2D<int> pos, unsigned width)
		{
			WeakPtr<Element> sParent = GetElement(parent).lock();

			return CreateComboBox(id, sParent, pos, width);
		}


		/*=============================================================================
		-- Creates a new Slider element and gives it to @parent.
		=============================================================================*/
		WeakPtr<Slider> Root::CreateSlider(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width)
		{
			SharedPtr<Slider> slider(new Slider);

			return DynamicPtrCast<Slider>(_ProcessNewElement(slider, id, parent, pos, width, TRACK_HEIGHT).lock());
		}

		WeakPtr<Slider> Root::CreateSlider(int id, int parent, Vector2D<int> pos, unsigned width)
		{
			WeakPtr<Element> sParent = GetElement(parent).lock();

			return CreateSlider(id, sParent, pos, width);
		}


		/*=============================================================================
		-- Creates a new Slider element and gives it to @parent.
		=============================================================================*/
		WeakPtr<TabBar> Root::CreateTabBar(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width, unsigned height)
		{
			SharedPtr<TabBar> tabBar(new TabBar);

			return DynamicPtrCast<TabBar>(_ProcessNewElement(tabBar, id, parent, pos, width, height).lock());
		}

		WeakPtr<TabBar> Root::CreateTabBar(int id, int parent, Vector2D<int> pos, unsigned width, unsigned height)
		{
			WeakPtr<Element> sParent = GetElement(parent).lock();

			return CreateTabBar(id, sParent, pos, width, height);
		}


		/*=============================================================================
		-- Creates a new Slider element and gives it to @parent.
		=============================================================================*/
		WeakPtr<TabPage> Root::_CreateTabPage(int id, WeakPtr<TabBar> parent, String caption)
		{
			SharedPtr<TabPage> tabPage(new TabPage);
			tabPage->SetCaption(caption);

			return DynamicPtrCast<TabPage>(_ProcessNewElement(tabPage, id, parent, Vector2D<int>(), 0, 0).lock());
		}


		/*=============================================================================
		-- Creates a new ScrollBar element and gives it to @parent.
		=============================================================================*/
		WeakPtr<ScrollBar> Root::CreateScrollBar(int id, WeakPtr<Element> parent, Vector2D<int> pos, int width, int height, SpanDirection spanDirection)
		{
			SharedPtr<ScrollBar> scrollBar(new ScrollBar);
			scrollBar->SetSpanDirection(spanDirection);

			return DynamicPtrCast<ScrollBar>(_ProcessNewElement(scrollBar, id, parent, pos, width, height).lock());
		}

		WeakPtr<ScrollBar> Root::CreateScrollBar(int id, int parent, Vector2D<int> pos, int width, int height, SpanDirection spanDirection)
		{
			WeakPtr<Element> sParent = GetElement(parent).lock();

			return CreateScrollBar(id, sParent, pos, width, height, spanDirection);
		}

		WeakPtr<ScrollBar> Root::CreateScrollBarHorizontal(int id, int parent, Vector2D<int> pos, int width)
		{
			WeakPtr<Element> sParent = GetElement(parent).lock();

			return CreateScrollBar(id, sParent, pos, width, SCROLL_BAR_BUTTON_SIZE, HORIZONTAL);
		}

		WeakPtr<ScrollBar> Root::CreateScrollBarVertical(int id, int parent, Vector2D<int> pos, int height)
		{
			WeakPtr<Element> sParent = GetElement(parent).lock();

			return CreateScrollBar(id, sParent, pos, SCROLL_BAR_BUTTON_SIZE, height, VERTICAL);
		}
	};
};
