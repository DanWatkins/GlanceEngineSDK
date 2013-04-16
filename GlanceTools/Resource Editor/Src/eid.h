/*=================================================================================================
  Glance Resource Editor - (C) 2010-2013 Daniel L. Watkins

  Filename: eid.h
=================================================================================================*/
#ifndef _gre_EID_H
#define _gre_EID_H


const int EID_ROOT											= -1;
	const int EID_MAIN_MENU_BAR								= 1000000000;
	const int EID_RESOURCE_TAB_BAR							= 2000000000;
		const int EID_RPAGE_ENTITY							= 2010000000;
			const int EID_ENTITY_LISTBOX					= 2010100000;

			//entity data
			const int EID_ENTITY_GBOXE1						= 2010200000;
				const int EID_GBOXE1_TXT_TYPE				= 2010201000;
				const int EID_GBOXE1_EBOX_TYPE				= 2010202000;

				const int EID_GBOXE1_TXT_NAME				= 2010203000;
				const int EID_GBOXE1_EBOX_NAME				= 2010204000;

				const int EID_GBOXE1_TXT_DISP_NAME			= 2010205000;
				const int EID_GBOXE1_EBOX_DISP_NAME			= 2010206000;

				const int EID_GBOXE1_TXT_ROTATION			= 2010207000;
				const int EID_GBOXE1_EBOX_ROATION			= 2010208000;

				const int EID_GBOXE1_TXT_BODY_ANCH			= 2010209000;
				const int EID_GBOXE1_EBOX_BODY_ANCH_X		= 2010210000;
				const int EID_GBOXE1_EBOX_BODY_ANCH_Y		= 2010211000;
				const int EID_GBOXE1_EBOX_BODY_ANCH_Z		= 2010212000;

			//object data
			const int EID_OBJECT_GBOXE2						= 2010300000;
				const int EID_GBOXE2_TXT_COMBINED			= 2010301000;
				const int EID_GBOXE2_CHECK_COMBINED			= 2010302000;

				const int EID_GBOXE2_TXT_OBTAINABLE			= 2010303000;
				const int EID_GBOXE2_CHECK_OBTAINABLE		= 2010304000;

				const int EID_GBOXE2_TXT_PROJECTILE_EXC		= 2010305000;
				const int EID_GBOXE2_CHECK_PROJECTILE_EXC	= 2010306000;

			//actor data
			const int EID_ACTOR_GBOXE3						= 2010400000;


			const int EID_AMMO_GBOXE4						= 2010500000;
				const int EID_GBOXE4_TXT_ROUND_COUNT		= 2010501000;
				const int EID_GBOXE4_EBOX_ROUND_COUNT		= 2010502000;
				const int EID_GBOXE4_TXT_PROJECTILE_NAME	= 2010503000;
				const int EID_GBOXE4_EBOX_PROJECTILE_NAME	= 2010504000;

			const int EID_WEAPON_GBOXE6						= 2010600000;
				const int EID_GBOXE6_TXT_PROJECTILE_SPEED	= 2010601000;
				const int EID_GBOXE6_EBOX_PROJECTILE_SPEED	= 2010602000;

#endif

