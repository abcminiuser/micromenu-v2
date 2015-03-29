#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#include "MicroMenu.h"


/*** DUMY CODE ***/
enum ButtonValues
{
	BUTTON_NONE,
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_ENTER,
};

enum ButtonValues GetButtonPress(void)
{
	return BUTTON_NONE;
};
/*** END DUMY CODE ***/


/** Example menu item specific enter callback function, run when the associated menu item is entered. */
static void Level1Item1_Enter(void)
{
	puts("ENTER");
}

/** Example menu item specific select callback function, run when the associated menu item is selected. */
static void Level1Item1_Select(void)
{
	puts("SELECT");
}

/** Generic function to write the text of a menu.
 *
 *  \param[in] Text   Text of the selected menu to write, in \ref MENU_ITEM_STORAGE memory space
 */
static void Generic_Write(const char* Text)
{
	if (Text)
		puts_P(Text);
}

MENU_ITEM(Menu_1, Menu_2, Menu_3, NULL_MENU, Menu_1_1 , Level1Item1_Select, Level1Item1_Enter, "1");
MENU_ITEM(Menu_2, Menu_3, Menu_1, NULL_MENU, NULL_MENU, NULL              , NULL             , "2");
MENU_ITEM(Menu_3, Menu_1, Menu_2, NULL_MENU, NULL_MENU, NULL              , NULL             , "3");

MENU_ITEM(Menu_1_1, Menu_1_2, Menu_1_2, NULL_MENU, NULL_MENU, NULL, NULL, "1.1");
MENU_ITEM(Menu_1_2, Menu_1_1, Menu_1_1, NULL_MENU, NULL_MENU, NULL, NULL, "1.2");

int main(void)
{
	/* Set up the default menu text write callback, and navigate to an absolute menu item entry. */
	Menu_SetGenericWriteCallback(Generic_Write);
	Menu_Navigate(&Menu_1);

    while (1)
    {
		/* Example usage of MicroMenu - here you can create your custom menu navigation system; you may wish to perform
		 * other tasks while detecting key presses, enter sleep mode while waiting for user input, etc.
		 */
		switch (GetButtonPress())
		{
			case BUTTON_UP:
				Menu_Navigate(MENU_PREVIOUS);
				break;
			case BUTTON_DOWN:
				Menu_Navigate(MENU_NEXT);
				break;
			case BUTTON_LEFT:
				Menu_Navigate(MENU_PARENT);
				break;
			case BUTTON_RIGHT:
				Menu_Navigate(MENU_CHILD);
				break;
			case BUTTON_ENTER:
				Menu_EnterCurrentItem();
				break;
			default:
				break;
		}
    }
}
