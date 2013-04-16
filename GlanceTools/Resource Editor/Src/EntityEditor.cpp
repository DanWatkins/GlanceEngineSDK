/*=================================================================================================
  Glance Resource Editor - (C) 2013 Daniel L. Watkins

  Filename: Editor.h
  Created: 2/13/2013
=================================================================================================*/
#include "Main.h"

namespace ge
{
	namespace gre
	{
		/*=============================================================================
		-- Constructor for EntityEditor.
		=============================================================================*/
		EntityEditor::EntityEditor(gui::Root *root, ResourceEditor *resourceEditor, WeakPtr<TabPage> editorTabPage)
		{
			SetRoot(root);
			SetResourceEditor(resourceEditor);
			SetEditorTabPage(editorTabPage);
		}


		/*=============================================================================
		-- Creates the user interface for the entity editor that resides in the tab
		   page designated by the host ResourceEditor.
		=============================================================================*/
		void EntityEditor::CreateUserInterface()
		{
			WeakPtr<TabPage> editorPage = GetEditorTabPage();
				mBrowserListBox = GetRoot()->CreateListBox(EID_ENTITY_LISTBOX, editorPage, Vector2D<int>(3,27),
															200, editorPage.lock()->GetParent().lock()->GetHeight()-30);
		}


		/*=============================================================================
		-- Loads content for the entity editor by loading the World object and
		   populating the browser list box with every template entity.
		=============================================================================*/
		bool EntityEditor::LoadContent()
		{
			mWorld.Init();

			for (int n=0; n<mWorld.GetEntityManager()->TemplateEntityCount(); n++)
			{
				WeakPtr<world::Entity> templateEntity = mWorld.GetEntityManager()->GetTemplateEntity(n);
				mBrowserListBox.lock()->AddCell(templateEntity.lock()->GetDisplayName(), templateEntity.lock()->GetName());
			}

			return true;
		}


		/*=============================================================================
		-- Loads all content specific for the specified entity. The interface adjusts
		   accordingly for the absolute type of entity. So Actor entities will have
		   slightly different properties to edit than Weapon entities.
		=============================================================================*/
		bool EntityEditor::LoadContentForEntity(WeakPtr<world::Entity> entity)
		{
			_ClearEntityGroupBoxes();

			//load the entity group section
			_CreateInterface_Entity();

			mEBoxType.lock()->SetCaption(ToString(entity.lock()->GetType()));
			mEBoxName.lock()->SetCaption(entity.lock()->GetName());
			mEBoxDisplayName.lock()->SetCaption(entity.lock()->GetDisplayName());
			mEBoxAnchorX.lock()->SetCaption(ToString(entity.lock()->GetBodyAnchor().x));
			mEBoxAnchorY.lock()->SetCaption(ToString(entity.lock()->GetBodyAnchor().y));
			mEBoxAnchorZ.lock()->SetCaption(ToString(entity.lock()->GetBodyAnchor().z));

			//create the appropriate interface depending on the type of entity
			switch (entity.lock()->GetType())
			{
				case world::EntityType::ACTOR:
				{
					_CreateInterface_Actor();
					break;
				}
				
				case world::EntityType::OBJECT:
				{
					_CreateInterface_Object();
					WeakPtr<world::Object> object = DynamicPtrCast<world::Object>(entity.lock());

					mCBoxCombined.lock()->SetChecked(object.lock()->GetCombined());
					mCBoxObtainable.lock()->SetChecked(object.lock()->GetObtainable());
					mCBoxProjectileExclusive.lock()->SetChecked(object.lock()->GetProjectileExclusive());

					switch (object.lock()->GetObjectType())
					{
						case world::ObjectType::AMMO:
						{
							_CreateInterface_Ammo();
							WeakPtr<world::Ammo> ammo = DynamicPtrCast<world::Ammo>(object.lock());

							mEBoxRoundCount.lock()->SetCaption(ToString(ammo.lock()->GetRoundCount()));
							mEBoxProjectileName.lock()->SetCaption(ammo.lock()->GetProjectileName());

							break;
						}

						case world::ObjectType::WEAPON:
						{
							_CreateInterface_Weapon();
							WeakPtr<world::Weapon> weapon = DynamicPtrCast<world::Weapon>(object.lock());

							mEBoxProjectileSpeed.lock()->SetCaption(ToString(weapon.lock()->GetProjectileSpeed()));
							break;
						}
					}

					break;
				}
			}

			return true;
		}


		/*=============================================================================
		-- Creates the interface for entities in general.
		=============================================================================*/
		void EntityEditor::_CreateInterface_Entity()
		{
			mEntityGroupBox = GetRoot()->CreateGroupBox(EID_ENTITY_GBOXE1, GetEditorTabPage(), Vector2D<int>(SPC_GBOX_LEFT_BOUND_X,32), 600, 144, "Entity Data");
				GetRoot()->CreateText(EID_GBOXE1_TXT_TYPE, mEntityGroupBox, Vector2D<int>(SPC_LEFT_BOUND_X,20), SPC_TXT_IDENTIFIER_WIDTH, "Type", TextAlignment::AlignedRight);
				mEBoxType = GetRoot()->CreateEditBox(EID_GBOXE1_EBOX_TYPE, mEntityGroupBox, Vector2D<int>(SPC_GBOX_SUB_CONTROL_X,20), 230, 24);

				GetRoot()->CreateText(EID_GBOXE1_TXT_NAME, mEntityGroupBox, Vector2D<int>(SPC_LEFT_BOUND_X,50), SPC_TXT_IDENTIFIER_WIDTH, "Name", TextAlignment::AlignedRight);
				mEBoxName = GetRoot()->CreateEditBox(EID_GBOXE1_EBOX_NAME, mEntityGroupBox, Vector2D<int>(SPC_GBOX_SUB_CONTROL_X,50), 230, 24);

				GetRoot()->CreateText(EID_GBOXE1_TXT_DISP_NAME, mEntityGroupBox, Vector2D<int>(SPC_LEFT_BOUND_X,80), SPC_TXT_IDENTIFIER_WIDTH, "Display Name", TextAlignment::AlignedRight);
				mEBoxDisplayName = GetRoot()->CreateEditBox(EID_GBOXE1_EBOX_DISP_NAME, mEntityGroupBox, Vector2D<int>(SPC_GBOX_SUB_CONTROL_X,80), 230, 24);

				GetRoot()->CreateText(EID_GBOXE1_TXT_BODY_ANCH, mEntityGroupBox, Vector2D<int>(SPC_LEFT_BOUND_X,110), SPC_TXT_IDENTIFIER_WIDTH, "Body Anchor", TextAlignment::AlignedRight);
				mEBoxAnchorX = GetRoot()->CreateEditBox(EID_GBOXE1_EBOX_BODY_ANCH_X, mEntityGroupBox, Vector2D<int>(SPC_GBOX_SUB_CONTROL_X,110), 70, 24);
				mEBoxAnchorY = GetRoot()->CreateEditBox(EID_GBOXE1_EBOX_BODY_ANCH_Y, mEntityGroupBox, Vector2D<int>(SPC_GBOX_SUB_CONTROL_X+80,110), 70, 24);
				mEBoxAnchorZ = GetRoot()->CreateEditBox(EID_GBOXE1_EBOX_BODY_ANCH_Z, mEntityGroupBox, Vector2D<int>(SPC_GBOX_SUB_CONTROL_X+160,110), 70, 24);
		}


		/*=============================================================================
		-- Creates an interface specific to Actor entites.
		=============================================================================*/
		void EntityEditor::_CreateInterface_Actor()
		{
			mActorGroupBox = GetRoot()->CreateGroupBox(EID_ACTOR_GBOXE3, GetEditorTabPage(), Vector2D<int>(SPC_GBOX_LEFT_BOUND_X,190), 600, 144, "Actor Data");
		}


		/*=============================================================================
		-- Creates an interface specific to Object entites.
		=============================================================================*/
		void EntityEditor::_CreateInterface_Object()
		{
			mObjectGroupBox = GetRoot()->CreateGroupBox(EID_OBJECT_GBOXE2, GetEditorTabPage(), Vector2D<int>(SPC_GBOX_LEFT_BOUND_X,190), 600, 144, "Object Data");
				mCBoxCombined = GetRoot()->CreateCheckBox(EID_GBOXE2_CHECK_COMBINED, mObjectGroupBox, Vector2D<int>(SPC_GBOX_SUB_CONTROL_X,20), "Combined");
					mCBoxCombined.lock()->SetTextAlginment(TextAlignment::AlignedLeft);

				mCBoxObtainable = GetRoot()->CreateCheckBox(EID_GBOXE2_CHECK_OBTAINABLE, mObjectGroupBox, Vector2D<int>(SPC_GBOX_SUB_CONTROL_X,50), "Obtainable");
					mCBoxObtainable.lock()->SetTextAlginment(TextAlignment::AlignedLeft);

				mCBoxProjectileExclusive = GetRoot()->CreateCheckBox(EID_GBOXE2_CHECK_PROJECTILE_EXC, mObjectGroupBox, Vector2D<int>(SPC_GBOX_SUB_CONTROL_X,80), "Projectile Exclusive");
					mCBoxProjectileExclusive.lock()->SetTextAlginment(TextAlignment::AlignedLeft);
		}


		/*=============================================================================
		-- Creates an interface specific to Ammo entities.
		=============================================================================*/
		void EntityEditor::_CreateInterface_Ammo()
		{
			mAmmoGroupBox = GetRoot()->CreateGroupBox(EID_AMMO_GBOXE4, GetEditorTabPage(), Vector2D<int>(SPC_GBOX_LEFT_BOUND_X,348), 600, 144, "Ammo Data");
				GetRoot()->CreateText(EID_GBOXE4_TXT_ROUND_COUNT, mAmmoGroupBox, Vector2D<int>(SPC_LEFT_BOUND_X,20), SPC_TXT_IDENTIFIER_WIDTH, "Round Count", TextAlignment::AlignedRight);
				mEBoxRoundCount = GetRoot()->CreateEditBox(EID_GBOXE4_EBOX_ROUND_COUNT, mAmmoGroupBox, Vector2D<int>(SPC_GBOX_SUB_CONTROL_X,20), 230, 24);

				GetRoot()->CreateText(EID_GBOXE4_TXT_PROJECTILE_NAME, mAmmoGroupBox, Vector2D<int>(SPC_LEFT_BOUND_X,50), SPC_TXT_IDENTIFIER_WIDTH, "Projectile Name", TextAlignment::AlignedRight);
				mEBoxProjectileName = GetRoot()->CreateEditBox(EID_GBOXE4_EBOX_PROJECTILE_NAME, mAmmoGroupBox, Vector2D<int>(SPC_GBOX_SUB_CONTROL_X,50), 230, 24);
		}


		/*=============================================================================
		-- Creates an interface specific to Weapon entities.
		=============================================================================*/
		void EntityEditor::_CreateInterface_Weapon()
		{
			mWeaponGroupBox = GetRoot()->CreateGroupBox(EID_WEAPON_GBOXE6, GetEditorTabPage(), Vector2D<int>(SPC_GBOX_LEFT_BOUND_X,348), 600, 144, "Weapon Data");
				GetRoot()->CreateText(EID_GBOXE6_TXT_PROJECTILE_SPEED, mWeaponGroupBox, Vector2D<int>(SPC_LEFT_BOUND_X,20), SPC_TXT_IDENTIFIER_WIDTH, "Projectile Speed", TextAlignment::AlignedRight);
				mEBoxProjectileSpeed = GetRoot()->CreateEditBox(EID_GBOXE6_EBOX_PROJECTILE_SPEED, mWeaponGroupBox, Vector2D<int>(SPC_GBOX_SUB_CONTROL_X,20), 230, 24);
		}


		/*=============================================================================
		-- Removes the group box sections that show the current entity's data.
		=============================================================================*/
		void EntityEditor::_ClearEntityGroupBoxes()
		{
			if (!mEntityGroupBox.expired())
				GetEditorTabPage().lock()->RemoveChild(mEntityGroupBox);
			if (!mActorGroupBox.expired())
				GetEditorTabPage().lock()->RemoveChild(mActorGroupBox);
			if (!mObjectGroupBox.expired())
				GetEditorTabPage().lock()->RemoveChild(mObjectGroupBox);
			if (!mAmmoGroupBox.expired())
				GetEditorTabPage().lock()->RemoveChild(mAmmoGroupBox);
			if (!mWeaponGroupBox.expired())
				GetEditorTabPage().lock()->RemoveChild(mWeaponGroupBox);
		}


		/*=============================================================================
		-- Saves the current world object's template entity data to file.
		=============================================================================*/
		void EntityEditor::SaveContent()
		{
		}


		/*=============================================================================
		-- Updates various interface related systems.
		=============================================================================*/
		void EntityEditor::Update()
		{
			if (mBrowserListBox.lock()->GetSelectedCell().lock() != mLastSelectedCell.lock())
			{
				mLastSelectedCell = mBrowserListBox.lock()->GetSelectedCell();

				if (!mLastSelectedCell.expired())
				{
					WeakPtr<world::Entity> entityAtFocus = mWorld.GetEntityManager()->GetTemplateEntity(mLastSelectedCell.lock()->GetUID());
					LoadContentForEntity(entityAtFocus);
				}
				else
					_ClearEntityGroupBoxes();
			}
		}
	}
}