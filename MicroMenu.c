/**
              MICRO-MENU V2
              https://github.com/abcminiuser/micromenu-v2

          (C) Dean Camera, 2012
        www.fourwalledcubicle.com
     dean [at] fourwalledcubicle.com

        Royalty-free for all uses.


              MICRO-MENU V3
              https://github.com/LDmicroGitHub/micromenu-v2

          (C) Ihor Nehrutsa, 2018
          LDmicro.GitHub@gmail.com
                                      */

#include "MicroMenu.h"

/** This is used when an invalid menu handle is required in
 *  a \ref MENU_ITEM() definition, i.e. to indicate that a
 *  menu has no linked parent, child, next or previous entry.
 */
Menu_Item_t MENU_ITEM_STORAGE NULL_MENU = {0};
Data_Item_t MENU_ITEM_STORAGE NULL_DATA = {0};

/** \internal
 *  Pointer to the generic menu text display function
 *  callback, to display the configured text of a menu item
 *  if no menu-specific display function has been set
 *  in the select menu item.
 */
static void (*MenuWriteFunc)(const char *Text) = NULL;
static void (*MenuShowFunc)(const Menu_Item_t *MenuItem) = NULL;

#ifdef MICRO_MENU_V3
static void (*MenuShowBit)(const Menu_Item_t *MenuItem) = NULL;
static void (*MenuShowSInt)(const Menu_Item_t *MenuItem) = NULL;
static void (*MenuShowUInt)(const Menu_Item_t *MenuItem) = NULL;
static void (*MenuShowFloat)(const Menu_Item_t *MenuItem) = NULL;

static void (*MenuEditBit)(const Menu_Item_t *MenuItem, signed int Dir) = NULL;
static void (*MenuEditInt)(const Menu_Item_t *MenuItem, signed int Dir) = NULL;
static void (*MenuEditFloat)(const Menu_Item_t *MenuItem, signed int Dir) = NULL;
#endif

/** \internal
 *  Pointer to the currently selected menu item.
 */
static Menu_Item_t *CurrentMenuItem = &NULL_MENU;

Menu_Item_t *Menu_GetCurrentMenu(void)
{
    return CurrentMenuItem;
}

void Menu_Navigate(const Menu_Item_t *NewMenu)
{
    void (*SelectCallback)(void);

    if((NewMenu == &NULL_MENU) || (NewMenu == NULL))
        return;

    CurrentMenuItem = NewMenu;

    if(MenuWriteFunc)
        MenuWriteFunc(CurrentMenuItem->Text);

    if(MenuShowFunc)
        MenuShowFunc(CurrentMenuItem);

    SelectCallback = MENU_ITEM_READ_POINTER(&CurrentMenuItem->SelectCallback);

    if(SelectCallback)
        SelectCallback();
}

void Menu_SetGenericWriteCallback(void (*WriteFunc)(const char *Text))
{
    MenuWriteFunc = WriteFunc;
    Menu_Navigate(CurrentMenuItem);
}

void Menu_SetGenericShowCallback(void (*ShowFunc)(const Menu_Item_t *MenuItem))
{
    MenuShowFunc = ShowFunc;
    Menu_Navigate(CurrentMenuItem);
}

#ifdef MICRO_MENU_V3
void Menu_SetGenericShowSInt(void (*ShowFunc)(const Menu_Item_t *MenuItem))
{
    MenuShowSInt = ShowFunc;
    Menu_Navigate(CurrentMenuItem);
}

void Menu_SetGenericShowUInt(void (*ShowFunc)(const Menu_Item_t *MenuItem))
{
    MenuShowUInt = ShowFunc;
    Menu_Navigate(CurrentMenuItem);
}

void Menu_SetGenericShowBit(void (*ShowFunc)(const Menu_Item_t *MenuItem))
{
    MenuShowBit = ShowFunc;
    Menu_Navigate(CurrentMenuItem);
}

void Menu_SetGenericShowFloat(void (*ShowFunc)(const Menu_Item_t *MenuItem))
{
    MenuShowFloat = ShowFunc;
    Menu_Navigate(CurrentMenuItem);
}

void Menu_SetGenericEditBit(void (*EditFunc)(const Menu_Item_t *MenuItem, signed int Dir))
{
    MenuEditBit = EditFunc;
    Menu_Navigate(CurrentMenuItem);
}

void Menu_SetGenericEditInt(void (*EditFunc)(const Menu_Item_t *MenuItem, signed int Dir))
{
    MenuEditInt = EditFunc;
    Menu_Navigate(CurrentMenuItem);
}

void Menu_SetGenericEditFloat(void (*EditFunc)(const Menu_Item_t *MenuItem, signed int Dir))
{
    MenuEditFloat = EditFunc;
    Menu_Navigate(CurrentMenuItem);
}

#endif

void Menu_EnterCurrentItem(void)
{
    void (*EnterCallback)(void);
#if 0 // defined(__MIKROC_PRO_FOR_AVR__) // || defined(__MIKROC_PRO_FOR_PIC__)
    if((CurrentMenuItem == &NULL_MENU) || (CurrentMenuItem == NULL))
#else
    if((CurrentMenuItem == &NULL_MENU) || (CurrentMenuItem == NULL))
#endif
    return;

    EnterCallback = MENU_ITEM_READ_POINTER(&CurrentMenuItem->EnterCallback);

    if(EnterCallback)
        EnterCallback();
}

#ifdef MICRO_MENU_V3
void Menu_Refresh(const Menu_Item_t *MenuItem)
{
    if((MenuItem == &NULL_MENU) || (MenuItem == NULL))
        return;
    if(MenuItem->RefreshCallback) {
        MenuItem->RefreshCallback(MenuItem);
    } else {
        if((MenuItem->DataItem == &NULL_DATA) || (MenuItem->DataItem == NULL))
            return;
        switch(MenuItem->DataItem->DataType) {
            case MENU_TYPE:
                break;
            case BIT_TYPE:
                if(MenuShowBit)
                    MenuShowBit(MenuItem);
                break;
            case UNSIGNED_TYPE:
                if(MenuShowUInt)
                    MenuShowUInt(MenuItem);
                break;
            case SIGNED_TYPE:
                if(MenuShowSInt)
                    MenuShowSInt(MenuItem);
                break;
            case FLOAT_TYPE:
                if(MenuShowFloat)
                    MenuShowFloat(MenuItem);
                break;
            default:
                break;
        }
    }
}

static void Generic_EditBit(const Menu_Item_t *MenuItem, signed int Dir)
{
    if((MenuItem == &NULL_MENU) || (MenuItem == NULL))
        return;

    if(MenuEditBit) {
        MenuEditBit(MenuItem, Dir);
    } else {
        if((MenuItem->DataItem == &NULL_DATA) || (MenuItem->DataItem == NULL))
            return;
        if(MenuItem->DataItem->DataPtr) {
            if((MenuItem->DataItem->Bit >= 0) && (MenuItem->DataItem->Bit <= 7)) {
                (*(unsigned short int *)MenuItem->DataItem->DataPtr) ^= (1 << MenuItem->DataItem->Bit);

            } else if((MenuItem->DataItem->Bit >= 8) && (MenuItem->DataItem->Bit <= 15)) {
                (*(unsigned int *)MenuItem->DataItem->DataPtr) ^= (1 << MenuItem->DataItem->Bit);

            } else if((MenuItem->DataItem->Bit >= 16) && (MenuItem->DataItem->Bit <= 31)) {
                (*(unsigned long int *)MenuItem->DataItem->DataPtr) ^= (1 << MenuItem->DataItem->Bit);

            } else if((MenuItem->DataItem->Bit >= 32) && (MenuItem->DataItem->Bit <= 63)) {
                (*(unsigned long long int *)MenuItem->DataItem->DataPtr) ^= (1 << MenuItem->DataItem->Bit);

            } else {
            }
        }
    }
}

#define EDIT(type_prefix)                                                                                                                           \
    if(Dir > 0) {                                                                                                                                   \
        if((MenuItem->DataItem->MinMax) && ((*(type_prefix int *)(MenuItem->DataItem->DataPtr) + Dir) > MenuItem->DataItem->MinMax->MaxValue))      \
            *(type_prefix int *)(MenuItem->DataItem->DataPtr) = MenuItem->DataItem->MinMax->MaxValue;                                               \
        else if((MenuItem->DataItem->MinMax) && ((*(type_prefix int *)(MenuItem->DataItem->DataPtr) + Dir) < MenuItem->DataItem->MinMax->MinValue)) \
            *(type_prefix int *)(MenuItem->DataItem->DataPtr) = MenuItem->DataItem->MinMax->MinValue;                                               \
        else                                                                                                                                        \
            *(type_prefix int *)(MenuItem->DataItem->DataPtr) += Dir;                                                                               \
    } else if(Dir < 0) {                                                                                                                            \
        if((MenuItem->DataItem->MinMax) && ((*(type_prefix int *)(MenuItem->DataItem->DataPtr) + Dir) < MenuItem->DataItem->MinMax->MinValue))      \
            *(type_prefix int *)(MenuItem->DataItem->DataPtr) = MenuItem->DataItem->MinMax->MinValue;                                               \
        else if((MenuItem->DataItem->MinMax) && ((*(type_prefix int *)(MenuItem->DataItem->DataPtr) + Dir) > MenuItem->DataItem->MinMax->MaxValue)) \
            *(type_prefix int *)(MenuItem->DataItem->DataPtr) = MenuItem->DataItem->MinMax->MaxValue;                                               \
        else                                                                                                                                        \
            *(type_prefix int *)(MenuItem->DataItem->DataPtr) += Dir;                                                                               \
    }

static void Generic_EditInt(const Menu_Item_t *MenuItem, signed int Dir)
{
    if((MenuItem == &NULL_MENU) || (MenuItem == NULL))
        return;

    if(MenuEditInt) {
        MenuEditInt(MenuItem, Dir);
    } else {
        if((MenuItem->DataItem == &NULL_DATA) || (MenuItem->DataItem == NULL))
            return;
        if(MenuItem->DataItem->DataPtr) {
            switch(MenuItem->DataItem->Size) {
                case 1:
#ifdef USE_DATA_RANGE
                    EDIT(signed short)
#else
                    (*(signed short int *)(MenuItem->DataItem->DataPtr)) += Dir;
#endif
                    break;
                case 2:
#ifdef USE_DATA_RANGE
                    EDIT(signed)
#else
                    (*(signed int *)(MenuItem->DataItem->DataPtr)) += Dir;
#endif
                    break;
                case 4:
#ifdef USE_DATA_RANGE
                    EDIT(signed long)
#else
                    (*(signed long int *)(MenuItem->DataItem->DataPtr)) += Dir;
#endif
                    break;
                case 8:
#ifdef USE_DATA_RANGE
                    EDIT(signed long long)
#else
                    (*(signed long long int *)(MenuItem->DataItem->DataPtr)) += Dir;
#endif
                    break;
                default:
                    break;
            }
        }
    }
}

static void Generic_EditFloat(const Menu_Item_t *MenuItem, signed int Dir)
{
    if((MenuItem == &NULL_MENU) || (MenuItem == NULL))
        return;

    if(MenuEditFloat) {
        MenuEditFloat(MenuItem, Dir);
    } else {
        if((MenuItem->DataItem == &NULL_DATA) || (MenuItem->DataItem == NULL))
            return;
        if(MenuItem->DataItem->DataPtr) {
            switch(MenuItem->DataItem->Size) {
                case 4:
                    *(float *)(MenuItem->DataItem->DataPtr) = *(long double *)(MenuItem->DataItem->DataPtr) + Dir;
                    break;
                case 8:
                    *(long double *)(MenuItem->DataItem->DataPtr) = *(long double *)(MenuItem->DataItem->DataPtr) + Dir;
                    break;
                default:
                    break;
            }
        }
    }
}

void Menu_Edit(const Menu_Item_t *MenuItem, signed int Dir)
{
    if((MenuItem == &NULL_MENU) || (MenuItem == NULL))
        return;

    if(MenuItem->EditCallback) {
        MenuItem->EditCallback(MenuItem, Dir);
    } else {
        switch(MenuItem->DataItem->DataType) {
            case MENU_TYPE:
                break;
            case BIT_TYPE:
                Generic_EditBit(MenuItem, Dir);
                break;
            case UNSIGNED_TYPE:
                Generic_EditInt(MenuItem, Dir);
                break;
            case SIGNED_TYPE:
                Generic_EditInt(MenuItem, Dir);
                break;
            case FLOAT_TYPE:
                Generic_EditFloat(MenuItem, Dir);
                break;
            default:
                break;
        }
    }
}
#endif