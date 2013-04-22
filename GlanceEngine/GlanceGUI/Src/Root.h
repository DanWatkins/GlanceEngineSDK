/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: Root.h
=================================================================================================*/

#include "GUI.h"

#ifndef _ge_ROOT_H
#define _ge_ROOT_H

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- Manager element that hosts the entire GUI system.
		=============================================================================*/
		class Root : public Element
		{
		//member data
		private:
			AssetManager mAssetManager;
			bool mConsecutiveButtonPress[3];
			WeakPtr<Element> mElementInFocus;

			//shared resources
			std::list< TextReference<Font*> > mFontList;
			Font *mDefaultFont;
			WeakPtr<Text> mDefaultText;

			void _UpdateTabbing();
			bool _ValidId(int id, WeakPtr<Element> parent);
			WeakPtr<Element> _ProcessNewElement(WeakPtr<Element> element, int elementId, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width, unsigned height);
			
			bool mSkipMouseMessagesThisUpdate;	//true if the mouse messages should not be updated for the current update (will reset after every Root::Update())

			WeakPtr<TabPage> _CreateTabPage(int id, WeakPtr<TabBar> parent, String caption);

		protected:
			void _Update();
			void _Draw();


		public:
			friend class TabBar;

			Root();
			~Root() {}

			//general
			bool Init(Window *window);
			void Update() { _Update(); }
			void Draw() { _Draw(); }

			void SetSkipMouseMessagesThisUpdate(bool skipMouseMessagesThisUpdate) { mSkipMouseMessagesThisUpdate = skipMouseMessagesThisUpdate; }
			bool GetSkipMouseMessagesThisUpdate() { return mSkipMouseMessagesThisUpdate; }

			AssetManager *GetAssetManager() { return &mAssetManager; }

			//font managment
			void LoadFonts(String fontDir=DIR_FONTS);
			Font *GetFont(String name);
			Font *GetDefaultFont() { return mDefaultFont; }
			WeakPtr<Text> GetDefaultText() { return mDefaultText; }
			void DrawDefaultText() { mDefaultText.lock()->_Draw(); }

			//element managment
			WeakPtr<Element> GetElement(int id);
			void HandleMouseMessages();//TODO make private
			WeakPtr<Element> GetHighestElementAtPoint(Vector2D<int> point);


			//info
			bool GetConsecutiveButtonPress(int button);

			//element creation methods
			WeakPtr<Picture> CreatePicture(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width, unsigned height, Image *image);
			WeakPtr<Picture> CreatePicture(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width, unsigned height, String filepath);
			WeakPtr<Picture> CreatePicture(int id, int parent, Vector2D<int> pos, unsigned width, unsigned height, String filepath);

			WeakPtr<ButtonCaption> CreateButtonCaption(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width, unsigned height, String caption);
			WeakPtr<ButtonCaption> CreateButtonCaption(int id, int parent, Vector2D<int> pos, unsigned width, unsigned height, String caption);

			WeakPtr<ButtonImage> CreateButtonImage(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width, unsigned height, String imageName);
			WeakPtr<ButtonImage> CreateButtonImage(int id, int parent, Vector2D<int> pos, unsigned width, unsigned height, String imageName);

			WeakPtr<Text> CreateText(int id, WeakPtr<Element> parent, Vector2D<int> pos, String text);
			WeakPtr<Text> CreateText(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width, String text, TextAlignment textAlignment);
			WeakPtr<Text> CreateText(int id, int parent, Vector2D<int> pos, String text);

			WeakPtr<CheckBox> CreateCheckBox(int id, WeakPtr<Element> parent, Vector2D<int> pos, String caption);
			WeakPtr<CheckBox> CreateCheckBox(int id, int parent, Vector2D<int> pos, String caption);

			WeakPtr<GroupBox> CreateGroupBox(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width, unsigned height, String caption="");
			WeakPtr<GroupBox> CreateGroupBox(int id, int parent, Vector2D<int> pos, unsigned width, unsigned height, String caption="");

			WeakPtr<EditBox> CreateEditBox(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width, unsigned height, String text="");
			WeakPtr<EditBox> CreateEditBox(int id, int parent, Vector2D<int> pos, unsigned width, unsigned height, String text="");

			WeakPtr<ProgressBar> CreateProgressBar(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width, unsigned height, unsigned percent=0.0);
			WeakPtr<ProgressBar> CreateProgressBar(int id, int parent, Vector2D<int> pos, unsigned width, unsigned height, unsigned percent=0.0);

			WeakPtr<Drager> CreateDrager(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width, unsigned height, bool dragsHorizontal, bool dragsVertical);
			WeakPtr<Drager> CreateDrager(int id, int parent, Vector2D<int> pos, unsigned width, unsigned height, bool dragsHorizontal, bool dragsVertical);

			WeakPtr<Cell> CreateCell(int id, WeakPtr<ListBox> parent, Vector2D<int> pos, unsigned width, unsigned height);
			WeakPtr<Cell> CreateCell(int id, int parent, Vector2D<int> pos, unsigned width, unsigned height);

			WeakPtr<DialogBox> CreateDialogBox(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width, unsigned height, String caption="");
			WeakPtr<DialogBox> CreateDialogBox(int id, int parent, Vector2D<int> pos, unsigned width, unsigned height, String caption="");

			WeakPtr<ContextMenu> CreateContextMenu(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width, unsigned height, String caption, Rect<int> rightClickRegion = Rect<int>());
			WeakPtr<ContextMenu> CreateContextMenu(int id, int parent, Vector2D<int> pos, unsigned width, unsigned height, String caption, Rect<int> rightClickRegion = Rect<int>());

			WeakPtr<MenuBar> CreateMenuBar(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width);
			WeakPtr<MenuBar> CreateMenuBar(int id, int parent, Vector2D<int> pos, unsigned width);

			WeakPtr<ListBox> CreateListBox(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width, unsigned height);
			WeakPtr<ListBox> CreateListBox(int id, int parent, Vector2D<int> pos, unsigned width, unsigned height);

			WeakPtr<ComboBox> CreateComboBox(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width);
			WeakPtr<ComboBox> CreateComboBox(int id, int parent, Vector2D<int> pos, unsigned width);

			WeakPtr<Slider> CreateSlider(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width);
			WeakPtr<Slider> CreateSlider(int id, int parent, Vector2D<int> pos, unsigned width);

			WeakPtr<TabBar> CreateTabBar(int id, WeakPtr<Element> parent, Vector2D<int> pos, unsigned width, unsigned height);
			WeakPtr<TabBar> CreateTabBar(int id, int parent, Vector2D<int> pos, unsigned width, unsigned height);

			WeakPtr<ScrollBar> CreateScrollBar(int id, WeakPtr<Element> parent, Vector2D<int> pos, int width, int height, SpanDirection spanDirection);
			WeakPtr<ScrollBar> CreateScrollBar(int id, int parent, Vector2D<int> pos, int width, int height, SpanDirection spanDirection);
			WeakPtr<ScrollBar> CreateScrollBarHorizontal(int id, int parent, Vector2D<int> pos, int width);
			WeakPtr<ScrollBar> CreateScrollBarVertical(int id, int parent, Vector2D<int> pos, int height);

			WeakPtr<Panel> CreatePanel(int id, WeakPtr<Element> parent, Vector2D<int> pos, int width, int height);
			WeakPtr<Panel> CreatePanel(int id, int parent, Vector2D<int> pos, int width, int height);
		};
	};
}

#endif

