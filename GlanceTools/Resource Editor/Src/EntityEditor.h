/*=================================================================================================
  Glance Resource Editor - (C) 2013 Daniel L. Watkins

  Filename: Editor.h
  Created: 2/13/2013
=================================================================================================*/
#ifndef gre_ENTITY_EDITOR_H
#define gre_ENTITY_EDITOR_H

#include "Editor.h"

namespace ge
{
	namespace gre
	{
		/*=============================================================================
		-- Abstract base Editor class that declares essential functionality such as
		   saving and reloading content.
		=============================================================================*/
		class EntityEditor : public SubEditor
		{
		private:
			world::World mWorld;	//used for serialization/deserialization of entity data
			WeakPtr<Cell> mLastSelectedCell;

			WeakPtr<ListBox> mBrowserListBox;

			//main entity group boxes
			WeakPtr<GroupBox> mEntityGroupBox;
			WeakPtr<GroupBox> mActorGroupBox;
			WeakPtr<GroupBox> mObjectGroupBox;
			WeakPtr<GroupBox> mAmmoGroupBox;
			WeakPtr<GroupBox> mWeaponGroupBox;

			//entity group box content
			WeakPtr<EditBox> mEBoxType;
			WeakPtr<EditBox> mEBoxName;
			WeakPtr<EditBox> mEBoxDisplayName;
			WeakPtr<EditBox> mEBoxAnchorX;
			WeakPtr<EditBox> mEBoxAnchorY;
			WeakPtr<EditBox> mEBoxAnchorZ;
			
			WeakPtr<CheckBox> mCBoxCombined;
			WeakPtr<CheckBox> mCBoxObtainable;
			WeakPtr<CheckBox> mCBoxProjectileExclusive;

			WeakPtr<EditBox> mEBoxRoundCount;
			WeakPtr<EditBox> mEBoxProjectileName;

			WeakPtr<EditBox> mEBoxProjectileSpeed;


			//dynamic interface creation methods
			void _CreateInterface_Entity();
			void _CreateInterface_Actor();
			void _CreateInterface_Object();
			void _CreateInterface_Ammo();
			void _CreateInterface_Weapon();
			void _ClearEntityGroupBoxes();

			//metrics for consistent spacing
			static const int SPC_GBOX_LEFT_BOUND_X		= 210;	//used for placing each groupbox that holds specific types of properties for entities
			static const int SPC_LEFT_BOUND_X			= 10;	//used for text or other GUI elements that are placed to the left-hand of their parent
			static const int SPC_GBOX_SUB_CONTROL_X		= 160;	//used for placing the primary control for editing a certain property towards the center
			static const int SPC_TXT_IDENTIFIER_WIDTH	= 140;

		public:
			EntityEditor(gui::Root *root, ResourceEditor *resourceEditor, WeakPtr<TabPage> editorTabPage);

			void CreateUserInterface();
			bool LoadContent();
			bool LoadContentForEntity(WeakPtr<world::Entity> entity);
			void SaveContent();
			void Update();
		};
	}
}

#endif

