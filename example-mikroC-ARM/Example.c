#include "MicroMenu.h"

#define DEBOUNCE_MS 150
#define DISP_LEN 16

enum ButtonValues { BUTTON_NONE, BUTTON_PARENT, BUTTON_PREVIOUS, BUTTON_NEXT, BUTTON_PLUS, BUTTON_MINUS, BUTTON_CHILD };

enum ButtonValues GetButtonPress(void)
{
    //return BUTTON_NONE;
    //return BUTTON_NEXT;
    if(Button(&PINB, 0, DEBOUNCE_MS, 0))
        return BUTTON_PARENT;
    else if(Button(&PINB, 1, DEBOUNCE_MS, 0))
        return BUTTON_PREVIOUS;
    else if(Button(&PINB, 2, DEBOUNCE_MS, 0))
        return BUTTON_NEXT;
    else if(Button(&PINB, 3, DEBOUNCE_MS, 0))
        return BUTTON_CHILD;
    else if(Button(&PINB, 4, DEBOUNCE_MS, 0))
        return BUTTON_MINUS;
    else if(Button(&PINB, 5, DEBOUNCE_MS, 0))
        return BUTTON_PLUS;
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

char *strcpy_const(char *dest, const char *src)
{
    while(*src) {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = 0;
    return dest;
}

/** Example menu item specific enter callback function, run when the associated menu item is entered. */
static void M_1_Enter(void)
{
    Lcd_Out(1, 1, "ENTER           ");
}

/** Example menu item specific select callback function, run when the associated menu item is selected. */
static void M_1_Select(void)
{
    Lcd_Out(1, 1, "SELECT          ");
}

static int  i = -20;
static void M_21_Refresh(const Menu_Item_t *MenuItem)
{
    char s1[20];
    char s2[20];
    if((MenuItem == &NULL_MENU) || (MenuItem == NULL))
        return;
    if(MenuItem->Text /* && *MenuItem->Text*/) {
        strcpy_const(s1, MenuItem->Text);
        sprinti(s2, "%s i=%d ", s1, i);
        Lcd_Out(1, 1, s2);
    }
    i++;
}

/** Generic function to write the text of a menu.
 *
 *  \param[in] Text   Text of the selected menu to write, in \ref MENU_ITEM_STORAGE memory space
 */
static void Generic_Write(const char *Text)
{
    if(Text && (*Text)) {
        Lcd_Out_const(1, 1, Text);
    }
}

static void Generic_Show(const Menu_Item_t *MenuItem)
{

    if((MenuItem == &NULL_MENU) || (MenuItem == NULL))
        return;
    if(MenuItem->Text && (*MenuItem->Text)) {
        Lcd_Out_const(1, 1, MenuItem->Text);
    }
}

static void GenericShowSInt(const Menu_Item_t *MenuItem)
{
    char s[DISP_LEN];
    if((MenuItem == &NULL_MENU) || (MenuItem == NULL))
        return;
    if(MenuItem->DataItem)
        if(MenuItem->DataItem->DataPtr) {
            switch(MenuItem->DataItem->Size) {
                case 1:
                    sprinti(s, "    %4d ", *(signed short int *)MenuItem->DataItem->DataPtr);
                    break;
                case 2:
                    sprinti(s, "  %6d ", *(signed int *)MenuItem->DataItem->DataPtr);
                    break;
                case 4:
                    sprintl(s, "%8d ", *(signed long int *)MenuItem->DataItem->DataPtr);
                    break;
                case 8:
                    sprintl(s, "%8d ", *(signed long long int *)MenuItem->DataItem->DataPtr);
                    break;
                default:
                    break;
            }
            Lcd_Out(1, 1, s);
        }
}

static void GenericShowUInt(const Menu_Item_t *MenuItem)
{
    char s[DISP_LEN];
    if((MenuItem == &NULL_MENU) || (MenuItem == NULL))
        return;

    if(MenuItem->DataItem->DataPtr) {
        switch(MenuItem->DataItem->Size) {
            case 1:
                sprinti(s, " %8u ", *(unsigned short int *)(MenuItem->DataItem->DataPtr));
                break;
            case 2:
                sprinti(s, " %8u ", *(unsigned int *)(MenuItem->DataItem->DataPtr));
                break;
            case 4:
                sprintl(s, " %8u ", *(unsigned long int *)(MenuItem->DataItem->DataPtr));
                break;
            case 8:
                sprintl(s, " %8u ", *(unsigned long long int *)(MenuItem->DataItem->DataPtr));
                break;
            default:
                break;
        }
        Lcd_Out(1, 1, s);
    }
}

static void GenericShowBit(const Menu_Item_t *MenuItem)
{
    char s[DISP_LEN + 1];
    bit  b;
    b = 0;
    if((MenuItem == &NULL_MENU) || (MenuItem == NULL))
        return;

    if(MenuItem->DataItem->DataPtr) {
        if((MenuItem->DataItem->Bit >= 0) && (MenuItem->DataItem->Bit <= 7)) {
            if((*(unsigned short int *)MenuItem->DataItem->DataPtr) & (1 << MenuItem->DataItem->Bit))
                b = 1;
        } else if((MenuItem->DataItem->Bit >= 8) && (MenuItem->DataItem->Bit <= 15)) {
            if((*(unsigned int *)MenuItem->DataItem->DataPtr) & (1 << MenuItem->DataItem->Bit))
                b = 1;
        } else if((MenuItem->DataItem->Bit >= 16) && (MenuItem->DataItem->Bit <= 31)) {
            if((*(unsigned long int *)MenuItem->DataItem->DataPtr) & (1 << MenuItem->DataItem->Bit))
                b = 1;
        } else if((MenuItem->DataItem->Bit >= 32) && (MenuItem->DataItem->Bit <= 63)) {
            if((*(unsigned long long int *)MenuItem->DataItem->DataPtr) & (1 << MenuItem->DataItem->Bit))
                b = 1;
        } else {
        }
        s[0] = b ? '1' : '0';
        s[1] = 0;
        //      Lcd_Chr(1, DISP_LEN-5, s[0]);
        Lcd_Out(1, DISP_LEN - 5, s);
    }
}

// clang-format off
#ifndef MICRO_MENU_V3
MENU_ITEM(Menu_1, Menu_2, Menu_2, NULL_MENU, NULL_MENU, NULL, NULL, "Menu 1. ");
MENU_ITEM(Menu_2, Menu_1, Menu_1, NULL_MENU, NULL_MENU, NULL, NULL, "Menu 2. ");
#else
//        Name,    Next,      Previous,  Parent,    Child,     SelectFunc, EnterFunc, RefreshFunc,  EditFunc,  Text5678,  DataType,      Data,      SizeOrBit
MENU_ITEM(Menu_1,  Menu_2,    Menu_3,    NULL_MENU, NULL_MENU, M_1_Select, M_1_Enter, NULL,         NULL,      "Menu 1 ");
MENU_ITEM(Menu_2,  Menu_3,    Menu_1,    NULL_MENU, Menu_21,   NULL,       NULL,      NULL,         NULL,      "Menu 2 ");
MENU_ITEM(Menu_21, NULL_MENU, NULL_MENU, Menu_2,    NULL_MENU, NULL,       NULL,      M_21_Refresh, NULL,      "Menu 21 ");
MENU_ITEM(Menu_3,  Menu_1,    Menu_2,    NULL_MENU, Menu_31,   NULL,       NULL,      NULL,         NULL,      "Menu 3 ");
DATA_ITEM(Menu_31, Menu_32,   Menu_33,   Menu_3,    NULL_MENU, NULL,       NULL,      NULL,         NULL,      "Edit bit ", BIT_TYPE, PORTA, 0);
DATA_ITEM(Menu_32, Menu_33,   Menu_31,   Menu_3,    NULL_MENU, NULL,       NULL,      NULL,         NULL,      "Edit signed ", SIGNED_TYPE, PORTE, 1);
DATA_ITEM(Menu_33, Menu_31,   Menu_32,   Menu_3,    NULL_MENU, NULL,       NULL,      NULL,         NULL,      "Edit unsigned ", UNSIGNED_TYPE, PORTE, 1);
#endif
// clang-format on

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
    DDRA.B0 = 1; // Set PORTA.0 pin as output
    DDRE = 0xFF; // Set PORTE pins as outputs

    DDRB = 0;     // Set PORTB pins as inputs
    PORTB = 0xff; // Pull ups ON
    PUD_bit = 0;  // Enable Pull ups

    Lcd_Init();
    Lcd_Cmd(_LCD_CURSOR_OFF);
    Lcd_Cmd(_LCD_CLEAR);

/* Set up the default menu text write callback, and navigate to an absolute menu item entry. */
#if 1
    Menu_SetGenericWriteCallback(Generic_Write);
#else
    Menu_SetGenericShowCallback(Generic_Show);
#endif
    Menu_SetGenericShowSInt(GenericShowSInt);
    Menu_SetGenericShowUInt(GenericShowUInt);
    Menu_SetGenericShowBit(GenericShowBit);
    Menu_Navigate(&Menu_32);

    while(1) {
        /* Example usage of MicroMenu - here you can create your custom menu navigation system; you may wish to perform
         * other tasks while detecting key presses, enter sleep mode while waiting for user input, etc.
         */
        switch(GetButtonPress()) {
            case BUTTON_PREVIOUS:
                Menu_Navigate(MENU_PREVIOUS);
                break;
            case BUTTON_NEXT:
                Menu_Navigate(MENU_NEXT);
                break;
            case BUTTON_PLUS:
                Menu_Edit(MENU_CURRENT, +1);
                break;
            case BUTTON_MINUS:
                Menu_Edit(MENU_CURRENT, -1);
                break;
            case BUTTON_CHILD:
                Menu_Navigate(MENU_CHILD);
                Menu_EnterCurrentItem();
                break;
            case BUTTON_PARENT:
                Menu_Navigate(MENU_PARENT);
                break;
            default:
                Menu_Refresh(MENU_CURRENT);
                break;
        }
    }
    return 0;
}