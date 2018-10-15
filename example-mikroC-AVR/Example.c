#include "MicroMenu.h"

enum ButtonValues
{
        BUTTON_NONE,
        BUTTON_ESC,
        BUTTON_UP,
        BUTTON_DOWN,
        BUTTON_LEFT,
        BUTTON_RIGHT,
        BUTTON_ENTER
};

enum ButtonValues GetButtonPress(void)
{
       return BUTTON_DOWN;
    if (Button(&PINB, 0, 1, 0))
       return BUTTON_ESC;
    else if (Button(&PINB, 1, 1, 0))
       return BUTTON_UP;
    else if (Button(&PINB, 2, 1, 0))
       return BUTTON_DOWN;
    else if (Button(&PINB, 3, 1, 0))
       return BUTTON_LEFT;
    else if (Button(&PINB, 4, 1, 0))
       return BUTTON_RIGHT;
    else if (Button(&PINB, 5, 1, 0))
       return BUTTON_ENTER;
    return BUTTON_NONE;
}

/** Example menu item specific enter callback function, run when the associated menu item is entered. */
static void Level1Item1_Enter(void)
{
        //Lcd_Out(1, 1, "ENTER");
}

/** Example menu item specific select callback function, run when the associated menu item is selected. */
static void Level1Item1_Select(void)
{
        //Lcd_Out(1, 1, "SELECT");
}

/** Generic function to write the text of a menu.
 *
 *  \param[in] Text   Text of the selected menu to write, in \ref MENU_ITEM_STORAGE memory space
 */
      code const char s0[] = "DEF";

static void Generic_Show(const Menu_Item_t *MenuItem)
{

    if((MenuItem == &NULL_MENU) || (MenuItem == NULL))
        return;
    if(MenuItem->Text) {
      char s[5];
      strcpy(s,"ABC");
      strcpy(s, s0);
      strcpy(s, &s0);
      strcpy(s, MenuItem->Text);
        Lcd_Out(1, 1, MenuItem->Text);
    }
}

MENU_ITEM(Menu_1, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, Level1Item1_Select, Level1Item1_Enter, "1))");
//MENU_ITEM(Menu_2, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, Level1Item1_Select, Level1Item1_Enter, "2))");
/*
MENU_ITEM(Menu_1, Menu_2, Menu_3, NULL_MENU, Menu_1_1 , Level1Item1_Select, Level1Item1_Enter, "1)");
MENU_ITEM(Menu_11, Menu_2, Menu_3, NULL_MENU, Menu_1_1 , Level1Item1_Select, Level1Item1_Enter, "1)");
MENU_ITEM(Menu_2, Menu_3, Menu_1, NULL_MENU, NULL_MENU, NULL              , NULL             , "2)");
MENU_ITEM(Menu_3, Menu_1, Menu_2, NULL_MENU, NULL_MENU, NULL              , NULL             , "3)");

MENU_ITEM(Menu_1_1, Menu_1_2, Menu_1_2, NULL_MENU, NULL_MENU, NULL, NULL, "1.1");
MENU_ITEM(Menu_1_2, Menu_1_1, Menu_1_1, NULL_MENU, NULL_MENU, NULL, NULL, "1.2");
*/
// Lcd pinout settings
sbit LCD_RS at PORTD2_bit;
sbit LCD_EN at PORTD3_bit;
sbit LCD_D7 at PORTD4_bit;
sbit LCD_D6 at PORTD5_bit;
sbit LCD_D5 at PORTD6_bit;
sbit LCD_D4 at PORTD7_bit;

// Pin direction
sbit LCD_RS_Direction at DDD2_bit;
sbit LCD_EN_Direction at DDD3_bit;
sbit LCD_D7_Direction at DDD4_bit;
sbit LCD_D6_Direction at DDD5_bit;
sbit LCD_D5_Direction at DDD6_bit;
sbit LCD_D4_Direction at DDD7_bit;

int main(void)
{
    DDRB = 0;                         // Set PORTB pin as input
    PORTB = 0xff;                     // Pull ups on
    PUD_bit = 0;                      // Enable Pull ups

    Lcd_Init();
    Lcd_Out(1, 1, "Inited");                    
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Cmd(_LCD_CURSOR_OFF);

    /* Set up the default menu text write callback, and navigate to an absolute menu item entry. */
    Menu_SetGenericShowCallback(Generic_Show);
    Menu_Navigate(&Menu_1);

    while (1) {
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