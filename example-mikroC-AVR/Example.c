#include "MicroMenu.h"

#define DEBOUNCE_MS 150

enum ButtonValues { BUTTON_NONE, BUTTON_ESC, BUTTON_UP, BUTTON_DOWN, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_ENTER };

enum ButtonValues GetButtonPress(void)
{
    return BUTTON_DOWN;
    if(Button(&PINB, 0, DEBOUNCE_MS, 0))
        return BUTTON_ESC;
    else if(Button(&PINB, 1, DEBOUNCE_MS, 0))
        return BUTTON_UP;
    else if(Button(&PINB, 2, DEBOUNCE_MS, 0))
        return BUTTON_DOWN;
    else if(Button(&PINB, 3, DEBOUNCE_MS, 0))
        return BUTTON_LEFT;
    else if(Button(&PINB, 4, DEBOUNCE_MS, 0))
        return BUTTON_RIGHT;
    else if(Button(&PINB, 5, DEBOUNCE_MS, 0))
        return BUTTON_ENTER;
    return BUTTON_NONE;
}

void Lcd_Out_const(char row, char col, const char *text)
{
    if(*text) {
        Lcd_Chr(row, col, *text);
        text++;
        while(*text) {
            Lcd_Chr_CP(*text);
            text++;
        }
    }
}

char *strcpy_const(char *to, const char *from)
{
    while(*from)
        *to++ = *from++;
    *to = 0;
    return to;
}

/** Example menu item specific enter callback function, run when the associated menu item is entered. */
static void Level1Item1_Enter(void)
{
    Lcd_Out(1, 1, "ENTER");
}

/** Example menu item specific select callback function, run when the associated menu item is selected. */
static void Level1Item1_Select(void)
{
    Lcd_Out(1, 1, "SELECT");
}

/** Generic function to write the text of a menu.
 *
 *  \param[in] Text   Text of the selected menu to write, in \ref MENU_ITEM_STORAGE memory space
 */
static void Generic_Write(const char *Text)
{
    if(Text)
        Lcd_Out_const(1, 1, Text);
}

static void Generic_Show(const Menu_Item_t *MenuItem)
{

    if((MenuItem == &NULL_MENU) || (MenuItem == NULL))
        return;
    if(MenuItem->Text /* && *MenuItem->Text*/)
        Lcd_Out_const(1, 1, MenuItem->Text);
}

//         Name,     Next,      Previous,  Parent,    Child,     SelectFunc,    EnterFunc,    RefreshFunc,       EditFunc,      Text5678,  DataType,      Data,      SizeOrBit
#if 0
MENU_ITEM(Menu_1, Menu_2, Menu_2, NULL_MENU, NULL_MENU, NULL, NULL, "123");
MENU_ITEM(Menu_2, Menu_1, Menu_1, NULL_MENU, NULL_MENU, NULL, NULL, "456");
#else
MENU_ITEM(Menu_1, Menu_2, Menu_3, NULL_MENU, Menu_1_1, Level1Item1_Select, Level1Item1_Enter, NULL, NULL, "1)");
MENU_ITEM(Menu_2, Menu_3, Menu_1, NULL_MENU, NULL_MENU, NULL, NULL, NULL, NULL, "2)");
MENU_ITEM(Menu_3, Menu_1, Menu_2, NULL_MENU, NULL_MENU, NULL, NULL, NULL, NULL, "3)");

MENU_ITEM(Menu_1_1, Menu_1_2, Menu_1_2, NULL_MENU, NULL_MENU, NULL, NULL, NULL, NULL, "1.1_");
MENU_ITEM(Menu_1_2, Menu_1_1, Menu_1_1, NULL_MENU, NULL_MENU, NULL, NULL, NULL, NULL, "1.2_");
#endif
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

code const char s0[] = "GH";
char            s[10];

int main(void)
{
    DDRB = 0;     // Set PORTB pins as inputs
    PORTB = 0xff; // Pull ups ON
    PUD_bit = 0;  // Enable Pull ups

    Lcd_Init();
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Cmd(_LCD_CURSOR_OFF);

/* Set up the default menu text write callback, and navigate to an absolute menu item entry. */
#if 1
    Menu_SetGenericWriteCallback(Generic_Write);
#else
    Menu_SetGenericShowCallback(Generic_Show);
#endif
    Menu_Navigate(&Menu_1);
    /*
    strcpy(s, "ABC");
    strcpy_const(s, s0);
    strcpy_const(s, Menu_GetCurrentMenu()->Text);
    Lcd_Out(1, 1, "ASDF");
    Lcd_Out(1, 1, s);
    Lcd_Out_const(1, 1, s0);
*/
    while(1) {
        /* Example usage of MicroMenu - here you can create your custom menu navigation system; you may wish to perform
         * other tasks while detecting key presses, enter sleep mode while waiting for user input, etc.
         */
        switch(GetButtonPress()) {
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