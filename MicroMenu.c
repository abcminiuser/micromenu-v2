/**
              MICRO-MENU V2

          (C) Dean Camera, 2012
        www.fourwalledcubicle.com
     dean [at] fourwalledcubicle.com

        Royalty-free for all uses.
	                                  */

#include "MicroMenu.h"

/** This is used when an invalid menu handle is required in
 *  a \ref MENU_ITEM() definition, i.e. to indicate that a
 *  menu has no linked parent, child, next or previous entry.
 */
Menu_Item_t PROGMEM NULL_MENU = {0};

/** \internal
 *  Pointer to the generic menu text display function
 *  callback, to display the configured text of a menu item
 *  if no menu-specific display function has been set
 *  in the select menu item.
 */
static void (*MenuWriteFunc)(const char* Text) = NULL;

/** \internal
 *  Pointer to the currently selected menu item.
 */
static Menu_Item_t* CurrentMenuItem = &NULL_MENU;


Menu_Item_t* Menu_GetCurrentMenu(void)
{
	return CurrentMenuItem;
}

void Menu_Navigate(Menu_Item_t* const NewMenu)
{
	if ((NewMenu == &NULL_MENU) || (NewMenu == NULL))
		return;

	CurrentMenuItem = NewMenu;

	if (MenuWriteFunc)
		MenuWriteFunc(CurrentMenuItem->Text);

	void (*SelectCallback)(void) = MENU_ITEM_READ_POINTER(&CurrentMenuItem->SelectCallback);

	if (SelectCallback)
		SelectCallback();
}

void Menu_SetGenericWriteCallback(void (*WriteFunc)(const char* Text))
{
	MenuWriteFunc = WriteFunc;
	Menu_Navigate(CurrentMenuItem);
}

void Menu_EnterCurrentItem(void)
{
	if ((CurrentMenuItem == &NULL_MENU) || (CurrentMenuItem == NULL))
		return;

	void (*EnterCallback)(void) = MENU_ITEM_READ_POINTER(&CurrentMenuItem->EnterCallback);

	if (EnterCallback)
		EnterCallback();
}
