// Tested: P16F1947

#include "MicroMenu.h"

#define DISP_LEN 16
#define DEBOUNCE_MS 150

enum ButtonValues { BUTTON_NONE, BUTTON_PARENT, BUTTON_PREVIOUS, BUTTON_NEXT, BUTTON_PLUS, BUTTON_MINUS, BUTTON_CHILD };

enum ButtonValues GetButtonPress(void)
{
    if(Button(&PORTB, 0, DEBOUNCE_MS, 0))
        return BUTTON_PARENT;
    else if(Button(&PORTB, 1, DEBOUNCE_MS, 0))
        return BUTTON_PREVIOUS;
    else if(Button(&PORTB, 2, DEBOUNCE_MS, 0))
        return BUTTON_NEXT;
    else if(Button(&PORTB, 3, DEBOUNCE_MS, 0))
        return BUTTON_CHILD;
    else if(Button(&PORTB, 4, DEBOUNCE_MS, 0))
        return BUTTON_MINUS;
    else if(Button(&PORTB, 5, DEBOUNCE_MS, 0))
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

/** Example menu item specific enter callback function, run when the associated menu item is entered. */
static void M_1_Enter(void)
{
    //             12345678
    Lcd_Out(1, 9, " ENTER  ");
}

/** Example menu item specific select callback function, run when the associated menu item is selected. */
static void M_1_Select(void)
{
    Lcd_Out(1, 9, " SELECT ");
}

/** Example menu item specific refresh callback function used to display externally modified data. */
static signed int i = 100;
static void       M_21_Refresh(const Menu_Item_t *MenuItem)
{
    char s[DISP_LEN + 1];
    if((MenuItem == &NULL_MENU) || (MenuItem == NULL))
        return;
    /*
    if(MenuItem->Text && (*MenuItem->Text)) {
        Lcd_Out_const(1, 1, MenuItem->Text);
    }
    */
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

/** Generic function to write the text of a menu and allows access to the data field via the MenuItem pointer.
 *
 *  \param[in] Text   Text of the selected menu to write, in \ref MENU_ITEM_STORAGE memory space
 */
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
#if 0
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
#else
    Menu_DataStr(&s, "%4d", MenuItem);
#endif
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

#if 0
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
#else
    Menu_DataStr(&s, "%4u", MenuItem);
#endif
    Lcd_Out(1, 13, s);
}

static void GenericShowBit(const Menu_Item_t *MenuItem)
{
    char s[DISP_LEN];
    if((MenuItem == &NULL_MENU) || (MenuItem == NULL))
        return;
    if((MenuItem->DataItem == &NULL_DATA) || (MenuItem->DataItem == NULL))
        return;
    if(MenuItem->DataItem->DataPtr == NULL)
        return;
    s[0] = '0';
#if 0
    if(MenuItem->DataItem->Bit < 8) {
        if((*(unsigned short int *)MenuItem->DataItem->DataPtr) & (1 << MenuItem->DataItem->Bit))
            s[0] = '1';
    } else if(MenuItem->DataItem->Bit < 16) {
        if((*(unsigned int *)MenuItem->DataItem->DataPtr) & (1 << MenuItem->DataItem->Bit))
            s[0] = '1';
    } else if(MenuItem->DataItem->Bit < 32) {
        if((*(unsigned long int *)MenuItem->DataItem->DataPtr) & (1L << MenuItem->DataItem->Bit))
            s[0] = '1';
    } else {
        if((*(unsigned long long int *)MenuItem->DataItem->DataPtr) & (1L << MenuItem->DataItem->Bit))
            s[0] = '1';
    }
#else
    Menu_DataStr(s, "", MenuItem);
#endif
    Lcd_Chr(1, DISP_LEN - 1, s[0]);
}
#endif

// clang-format off
#ifndef MICRO_MENU_V3
//         Name,     Next,     Previous, Parent,    Child,     SelectFunc, EnterFunc,  Text567890123456
MENU_ITEM (Menu_1,  Menu_2,    Menu_3,   NULL_MENU, NULL_MENU, M_1_Select, M_1_Enter, "Menu 1.         ");
 MENU_ITEM(Menu_1_1, Menu_1_2, Menu_1_2, NULL_MENU, NULL_MENU, NULL,       NULL,      "Menu 1.1.       ");
 MENU_ITEM(Menu_1_2, Menu_1_1, Menu_1_1, NULL_MENU, NULL_MENU, NULL,       NULL,      "Menu 1.2.       ");
MENU_ITEM (Menu_2,   Menu_3,   Menu_1,   NULL_MENU, NULL_MENU, NULL,       NULL,      "Menu 2.         ");
MENU_ITEM (Menu_3,   Menu_1,   Menu_2,   NULL_MENU, NULL_MENU, NULL,       NULL,      "Menu 3.         ");
#else
//          Name,    Next,      Previous,  Parent,    Child,     SelectFunc, EnterFunc, RefreshFunc,  EditFunc,   Text567890123456,  DataType,      Data,  SizeOrBit
MENU_ITEM  (Menu_1,  Menu_2,    Menu_3,    NULL_MENU, NULL_MENU, M_1_Select, M_1_Enter, NULL,         NULL,      "Menu 1          ");
MENU_ITEM  (Menu_2,  Menu_3,    Menu_1,    NULL_MENU, Menu_21,   NULL,       NULL,      NULL,         NULL,      "Menu 2          ");
 MENU_ITEM (Menu_21, NULL_MENU, NULL_MENU, Menu_2,    NULL_MENU, NULL,       NULL,      M_21_Refresh, NULL,      "Menu 2.1        ");
MENU_ITEM  (Menu_3,  Menu_1,    Menu_2,    NULL_MENU, Menu_31,   NULL,       NULL,      NULL,         NULL,      "Menu 3          ");
 DATA_ITEM (Menu_31, Menu_32,   Menu_33,   Menu_3,    NULL_MENU, NULL,       NULL,      NULL,         NULL,      "Edit bit        ", BIT_TYPE,      PORTE, 5);
 DATA_ITEM (Menu_32, Menu_33,   Menu_31,   Menu_3,    NULL_MENU, NULL,       NULL,      NULL,         NULL,      "Edit signed     ", SIGNED_TYPE,   PORTC, 1);
#    ifdef USE_DATA_RANGE
 DATA_RANGE(Menu_33, Menu_31,   Menu_32,   Menu_3,    NULL_MENU, NULL,       NULL,      NULL,         NULL,      "Edit unsign     ", UNSIGNED_TYPE, PORTC, 1, 10, 20);
#    else
 DATA_ITEM (Menu_33, Menu_31,   Menu_32,   Menu_3,    NULL_MENU, NULL,       NULL,      NULL,         NULL,      "Edit unsign     ", UNSIGNED_TYPE, PORTC, 1);
#    endif
#endif
// clang-format on

// Lcd pinout settings
sbit LCD_RS at RD4_bit;
sbit LCD_EN at RD5_bit;
sbit LCD_D7 at RD3_bit;
sbit LCD_D6 at RD2_bit;
sbit LCD_D5 at RD1_bit;
sbit LCD_D4 at RD0_bit;

// Pin direction
sbit LCD_RS_Direction at TRISD4_bit;
sbit LCD_EN_Direction at TRISD5_bit;
sbit LCD_D7_Direction at TRISD3_bit;
sbit LCD_D6_Direction at TRISD2_bit;
sbit LCD_D5_Direction at TRISD1_bit;
sbit LCD_D4_Direction at TRISD0_bit;

int main(void)
{
    TRISE.B5 = 0; // Set PORTE.B5 pin as output
    PORTE.B5 = 0;

    TRISC = 0; // Set PORTC pins as outputs
    PORTC = 0;

    TRISB = 0xFF;      // Set PORTB pins as inputs
    NOT_WPUEN_bit = 0; // Enable Pull ups // Pull ups ON

    Lcd_Init();
    Lcd_Cmd(_LCD_CURSOR_OFF);
    Lcd_Cmd(_LCD_CLEAR);

/* Only one of the functions Generic_Write () or Generic_Show () is needed to display menu items.
Generic_Write () is simpler, but can only display the Text field. 
Generic_Show () - more difficult, but gives access to the structure of Data */
/* Set up the default menu text write callback, and navigate to an absolute menu item entry. */
#if 0
    Menu_SetGenericWriteCallback(Generic_Write);
#else
    Menu_SetGenericShowCallback(Generic_Show);
#endif
#ifdef MICRO_MENU_V3
    /* These functions displays a Data*/
    /* Set up the default menu Data show callbacks. */
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
                Menu_EnterCurrentItem();
                Menu_Navigate(MENU_CHILD);
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