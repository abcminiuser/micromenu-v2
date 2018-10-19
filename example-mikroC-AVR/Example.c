// Tested: ATMEGA2560

#include "MicroMenu.h"

#define DISP_LEN 16
#define DEBOUNCE_MS 150

enum ButtonValues { BUTTON_NONE, BUTTON_PARENT, BUTTON_PREVIOUS, BUTTON_NEXT, BUTTON_PLUS, BUTTON_MINUS, BUTTON_CHILD };

enum ButtonValues GetButtonPress(void)
{
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
    //                 12345678
    Lcd_Out(1, 9, " ENTER  ");
}

/** Example menu item specific select callback function, run when the associated menu item is selected. */
static void M_1_Select(void)
{
    Lcd_Out(1, 9, " SELECT ");
}

static signed int i = 100;
static void       M_21_Refresh(const Menu_Item_t *MenuItem)
{
    char s[DISP_LEN + 1];
    if((MenuItem == &NULL_MENU) || (MenuItem == NULL))
        return;
    if(MenuItem->Text && (*MenuItem->Text)) {
        //      Lcd_Out_const(1, 1, MenuItem->Text);
    }
    sprinti(s, "i=%6d", i);
    Lcd_Out(1, 9, s);
    i--;
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

#ifdef MICRO_MENU_V3
static void GenericShowSInt(const Menu_Item_t *MenuItem)
{
    signed int i;
    char       s[DISP_LEN];
    if((MenuItem == &NULL_MENU) || (MenuItem == NULL))
        return;
    if((MenuItem->DataItem == &NULL_DATA) || (MenuItem->DataItem == NULL))
        return;
    if(MenuItem->DataItem->DataPtr == NULL)
        return;

    switch(MenuItem->DataItem->Size) {
        case 1:
            i = *(signed short int *)MenuItem->DataItem->DataPtr;
            sprinti(s, "%4d", i);
            break;
        case 2:
            sprinti(s, "%4d", (signed int)(*(signed int *)(MenuItem->DataItem->DataPtr)));
            break;
        case 4:
            sprintl(s, "%4Ld", (signed long int)(*(signed long int *)(MenuItem->DataItem->DataPtr)));
            break;
        case 8:
            sprintl(s, "%4LLd", (signed long long int)(*(signed long long int *)(MenuItem->DataItem->DataPtr)));
            break;
        default:
            break;
    }
    Lcd_Out(1, 13, s);
}

static void GenericShowUInt(const Menu_Item_t *MenuItem)
{
    unsigned int i;
    char         s[DISP_LEN];
    if((MenuItem == &NULL_MENU) || (MenuItem == NULL))
        return;
    if((MenuItem->DataItem == &NULL_DATA) || (MenuItem->DataItem == NULL))
        return;
    if(MenuItem->DataItem->DataPtr == NULL)
        return;

    switch(MenuItem->DataItem->Size) {
        case 1:
            i = *(unsigned short int *)MenuItem->DataItem->DataPtr;
            sprinti(s, "%4u", i);
            break;
        case 2:
            sprinti(s, "%4u", (unsigned int)(*(unsigned int *)(MenuItem->DataItem->DataPtr)));
            break;
        case 4:
            sprintl(s, "%4Lu", (unsigned long int)(*(unsigned long int *)(MenuItem->DataItem->DataPtr)));
            break;
        case 8:
            sprintl(s, "%4LLu", (unsigned long long int)(*(unsigned long long int *)(MenuItem->DataItem->DataPtr)));
            break;
        default:
            break;
    }
    Lcd_Out(1, 13, s);
}

static void GenericShowBit(const Menu_Item_t *MenuItem)
{
    char s = '0';
    if((MenuItem == &NULL_MENU) || (MenuItem == NULL))
        return;
    if((MenuItem->DataItem == &NULL_DATA) || (MenuItem->DataItem == NULL))
        return;
    if(MenuItem->DataItem->DataPtr == NULL)
        return;

    if((MenuItem->DataItem->Bit >= 0) && (MenuItem->DataItem->Bit <= 7)) {
        if((*(unsigned short int *)MenuItem->DataItem->DataPtr) & (1 << MenuItem->DataItem->Bit))
            s = '1';
    } else if((MenuItem->DataItem->Bit >= 8) && (MenuItem->DataItem->Bit <= 15)) {
        if((*(unsigned int *)MenuItem->DataItem->DataPtr) & (1 << MenuItem->DataItem->Bit))
            s = '1';
    } else if((MenuItem->DataItem->Bit >= 16) && (MenuItem->DataItem->Bit <= 31)) {
        if((*(unsigned long int *)MenuItem->DataItem->DataPtr) & (1L << MenuItem->DataItem->Bit))
            s = '1';
    } else if((MenuItem->DataItem->Bit >= 32) && (MenuItem->DataItem->Bit <= 63)) {
        if((*(unsigned long long int *)MenuItem->DataItem->DataPtr) & (1L << MenuItem->DataItem->Bit))
            s = '1';
    }
    Lcd_Chr(1, DISP_LEN - 1, s);
}
#endif

// clang-format off
#ifndef MICRO_MENU_V3
//        Name,   Next,   Previous, Parent,  Child,     SelectFunc, EnterFunc,  Text567890123456
MENU_ITEM(Menu_1, Menu_2, Menu_2, NULL_MENU, NULL_MENU, NULL,       NULL,      "Menu 1.         ");
MENU_ITEM(Menu_2, Menu_1, Menu_1, NULL_MENU, NULL_MENU, NULL,       NULL,      "Menu 2.         ");
#else
//        Name,    Next,      Previous,  Parent,    Child,     SelectFunc, EnterFunc, RefreshFunc,  EditFunc,   Text567890123456,  DataType,      Data,  SizeOrBit
MENU_ITEM(Menu_1,  Menu_2,    Menu_3,    NULL_MENU, NULL_MENU, M_1_Select, M_1_Enter, NULL,         NULL,      "Menu 1          ");
MENU_ITEM(Menu_2,  Menu_3,    Menu_1,    NULL_MENU, Menu_21,   NULL,       NULL,      NULL,         NULL,      "Menu 2          ");
MENU_ITEM(Menu_21, NULL_MENU, NULL_MENU, Menu_2,    NULL_MENU, NULL,       NULL,      M_21_Refresh, NULL,      "Menu 21         ");
MENU_ITEM(Menu_3,  Menu_1,    Menu_2,    NULL_MENU, Menu_31,   NULL,       NULL,      NULL,         NULL,      "Menu 3          ");
DATA_ITEM(Menu_31, Menu_32,   Menu_33,   Menu_3,    NULL_MENU, NULL,       NULL,      NULL,         NULL,      "Edit bit        ", BIT_TYPE,      PORTE, 5);
DATA_ITEM(Menu_32, Menu_33,   Menu_31,   Menu_3,    NULL_MENU, NULL,       NULL,      NULL,         NULL,      "Edit signed     ", SIGNED_TYPE,   PORTC, 1);
DATA_ITEM(Menu_33, Menu_31,   Menu_32,   Menu_3,    NULL_MENU, NULL,       NULL,      NULL,         NULL,      "Edit unsign     ", UNSIGNED_TYPE, PORTC, 1);
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
    DDRE.B5 = 1; // Set PORTE.B5 pin as output
    PORTE.B5 = 0;

    DDRC = 0xFF; // Set PORTC pins as outputs
    PORTC = 0;

    DDRB = 0;     // Set PORTB pins as inputs
    PUD_bit = 0;  // Enable Pull ups
    PORTB = 0xff; // Pull ups ON

    Lcd_Init();
    Lcd_Cmd(_LCD_CURSOR_OFF);
    Lcd_Cmd(_LCD_CLEAR);

/* Set up the default menu text write callback, and navigate to an absolute menu item entry. */
#if 1
    Menu_SetGenericWriteCallback(Generic_Write);
#else
    Menu_SetGenericShowCallback(Generic_Show);
#endif
#ifdef MICRO_MENU_V3
    Menu_SetGenericShowSInt(GenericShowSInt);
    Menu_SetGenericShowUInt(GenericShowUInt);
    Menu_SetGenericShowBit(GenericShowBit);
#endif
    Menu_Navigate(&Menu_1);

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
            case BUTTON_CHILD:
                Menu_Navigate(MENU_CHILD);
                Menu_EnterCurrentItem();
                break;
            case BUTTON_PARENT:
                Menu_Navigate(MENU_PARENT);
                break;
#ifdef MICRO_MENU_V3
            case BUTTON_PLUS:
                Menu_Edit(MENU_CURRENT, +1);
                break;
            case BUTTON_MINUS:
                Menu_Edit(MENU_CURRENT, -1);
                break;
            default:
                Menu_Refresh(MENU_CURRENT);
                break;
#endif
        }
    }
    return 0;
}