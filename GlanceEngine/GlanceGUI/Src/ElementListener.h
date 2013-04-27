/*=================================================================================================
  Glance Engine - (C) 2010-2013 Daniel L. Watkins

  Filename: ElementListener.h
  Created: 4/26/2013 @ 9:29 UTC-6 @ [MHS Media Center]
=================================================================================================*/
#include "GUI.h"

#ifndef _ge_ELEMENT_LISTENER_H
#define _ge_ELEMENT_LISTENER_H

namespace ge
{
	namespace gui
	{
		/*=============================================================================
		-- 
		=============================================================================*/
		class ElementListener
		{
		public:
			virtual void SendElementMessage(ElementEvent elementEvent, WeakPtr<Element> element, String eventParam) = 0;
		};
	};
};

#endif

