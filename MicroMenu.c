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

#define QUESTION "?"

/** This is used when an invalid menu handle is required in
 *  a \ref MENU_ITEM() definition, i.e. to indicate that a
 *  menu has no linked parent, child, next or previous entry.
 */
Menu_Item_t MENU_ITEM_STORAGE NULL_MENU = {0};
#ifdef USE_DATA
Data_Item_t MENU_ITEM_STORAGE NULL_DATA = {0};
Data_MinMax_t MENU_ITEM_STORAGE NULL_MINMAX = {0};
#endif

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
    void (*RefreshCallback)(const struct Menu_Item *);

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

    RefreshCallback = MENU_ITEM_READ_POINTER(&CurrentMenuItem->RefreshCallback);

    if(RefreshCallback)
        RefreshCallback(NewMenu);
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

void Menu_SaveEditedCurrentItem(void)
{
    void (*SaveEditCallback)(const Menu_Item_t *NewMenu);
    if((CurrentMenuItem == &NULL_MENU) || (CurrentMenuItem == NULL))
        return;

    SaveEditCallback = MENU_ITEM_READ_POINTER(&CurrentMenuItem->SaveEditCallback);

    if(SaveEditCallback)
        SaveEditCallback(CurrentMenuItem);
}

void Menu_RestoreEditedCurrentItem(void)
{
    void (*RestoreEditCallback)(const Menu_Item_t *NewMenu);
    if((CurrentMenuItem == &NULL_MENU) || (CurrentMenuItem == NULL))
        return;

    RestoreEditCallback = MENU_ITEM_READ_POINTER(&CurrentMenuItem->RestoreEditCallback);

    if(RestoreEditCallback)
        RestoreEditCallback(CurrentMenuItem);
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
#ifdef USE_FLOAT_TYPE
            case FLOAT_TYPE:
                if(MenuShowFloat)
                    MenuShowFloat(MenuItem);
                break;
#endif
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
        if(MenuItem->DataItem->DataPtr == NULL)
            return;

        if(MenuItem->DataItem->Bit < 8) {
            (*(unsigned short int *)MenuItem->DataItem->DataPtr) ^= (1 << MenuItem->DataItem->Bit);
        } else if(MenuItem->DataItem->Bit < 16) {
            (*(unsigned int *)MenuItem->DataItem->DataPtr) ^= (1 << MenuItem->DataItem->Bit);
        } else if(MenuItem->DataItem->Bit < 32) {
            (*(unsigned long int *)MenuItem->DataItem->DataPtr) ^= (1L << MenuItem->DataItem->Bit);
        } else { // if(MenuItem->DataItem->Bit < 64) {
            (*(unsigned long long int *)MenuItem->DataItem->DataPtr) ^= (1LL << MenuItem->DataItem->Bit);
        }
    }
}

#ifdef JUMP_MIN_TO_MAX_TO_MIN
#define EDIT(a_type)                                                                                                 \
    if((MenuItem->DataItem->MinMax == &NULL_MINMAX) || (MenuItem->DataItem->MinMax == NULL))                         \
        *(a_type *)(MenuItem->DataItem->DataPtr) += Dir;                                                             \
    else if((Dir > 0) && (*(a_type *)(MenuItem->DataItem->DataPtr) == (a_type)MenuItem->DataItem->MinMax->MaxValue)) \
        *(a_type *)(MenuItem->DataItem->DataPtr) = MenuItem->DataItem->MinMax->MinValue;                             \
    else if((Dir < 0) && (*(a_type *)(MenuItem->DataItem->DataPtr) == (a_type)MenuItem->DataItem->MinMax->MinValue)) \
        *(a_type *)(MenuItem->DataItem->DataPtr) = MenuItem->DataItem->MinMax->MaxValue;                             \
    else if((*(a_type *)(MenuItem->DataItem->DataPtr) + Dir) < (a_type)MenuItem->DataItem->MinMax->MinValue)         \
        *(a_type *)(MenuItem->DataItem->DataPtr) = MenuItem->DataItem->MinMax->MinValue;                             \
    else if((*(a_type *)(MenuItem->DataItem->DataPtr) + Dir) > (a_type)MenuItem->DataItem->MinMax->MaxValue)         \
        *(a_type *)(MenuItem->DataItem->DataPtr) = MenuItem->DataItem->MinMax->MaxValue;                             \
    else                                                                                                             \
        *(a_type *)(MenuItem->DataItem->DataPtr) += Dir;
#else
#define EDIT(a_type)                                                                                                 \
    if((MenuItem->DataItem->MinMax == &NULL_MINMAX) || (MenuItem->DataItem->MinMax == NULL))                         \
        *(a_type *)(MenuItem->DataItem->DataPtr) += Dir;                                                             \
    else if((Dir > 0) && (*(a_type *)(MenuItem->DataItem->DataPtr) == (a_type)MenuItem->DataItem->MinMax->MaxValue)) \
        ;                                                                                                            \
    else if((Dir < 0) && (*(a_type *)(MenuItem->DataItem->DataPtr) == (a_type)MenuItem->DataItem->MinMax->MinValue)) \
        ;                                                                                                            \
    else if((*(a_type *)(MenuItem->DataItem->DataPtr) + Dir) < (a_type)MenuItem->DataItem->MinMax->MinValue)         \
        *(a_type *)(MenuItem->DataItem->DataPtr) = MenuItem->DataItem->MinMax->MinValue;                             \
    else if((*(a_type *)(MenuItem->DataItem->DataPtr) + Dir) > (a_type)MenuItem->DataItem->MinMax->MaxValue)         \
        *(a_type *)(MenuItem->DataItem->DataPtr) = MenuItem->DataItem->MinMax->MaxValue;                             \
    else                                                                                                             \
        *(a_type *)(MenuItem->DataItem->DataPtr) += Dir;
#endif

static void Edit1Byte(const Menu_Item_t *MenuItem, signed int Dir)
{
#ifdef USE_DATA_RANGE
    EDIT(signed short int)
#else
    (*(signed short int *)(MenuItem->DataItem->DataPtr)) += Dir;
#endif
}

static void Edit2Byte(const Menu_Item_t *MenuItem, signed int Dir)
{
#ifdef USE_DATA_RANGE
    EDIT(signed int)
#else
    (*(signed int *)(MenuItem->DataItem->DataPtr)) += Dir;
#endif
}

static void Edit4Byte(const Menu_Item_t *MenuItem, signed int Dir)
{
#ifdef USE_DATA_RANGE
    EDIT(signed long int)
#else
    (*(signed long int *)(MenuItem->DataItem->DataPtr)) += Dir;
#endif
}

static void Edit8Byte(const Menu_Item_t *MenuItem, signed int Dir)
{
#ifdef USE_DATA_RANGE
    EDIT(signed long long int)
#else
    (*(signed long long int *)(MenuItem->DataItem->DataPtr)) += Dir;
#endif
}

void Generic_EditInt(const Menu_Item_t *MenuItem, signed int Dir)
{
    if((MenuItem == &NULL_MENU) || (MenuItem == NULL))
        return;

    if(MenuEditInt) {
        MenuEditInt(MenuItem, Dir);
    } else {
        if((MenuItem->DataItem == &NULL_DATA) || (MenuItem->DataItem == NULL))
            return;
        if(MenuItem->DataItem->DataPtr == NULL)
            return;
        switch(MenuItem->DataItem->Size) {
            case 1:
                Edit1Byte(MenuItem, Dir);
                break;
            case 2:
                Edit2Byte(MenuItem, Dir);
                break;
            case 4:
                Edit4Byte(MenuItem, Dir);
                break;
            case 8:
                Edit8Byte(MenuItem, Dir);
                break;
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
        if(MenuItem->DataItem->DataPtr == NULL)
            return;
        switch(MenuItem->DataItem->Size) {
            case 4:
                *(float *)(MenuItem->DataItem->DataPtr) = *(long double *)(MenuItem->DataItem->DataPtr) + Dir;
                break;
            case 8:
                *(long double *)(MenuItem->DataItem->DataPtr) = *(long double *)(MenuItem->DataItem->DataPtr) + Dir;
                break;
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
            case BIT_TYPE:
                Generic_EditBit(MenuItem, Dir);
                break;
            case UNSIGNED_TYPE:
                Generic_EditInt(MenuItem, Dir);
                break;
            case SIGNED_TYPE:
                Generic_EditInt(MenuItem, Dir);
                break;
#ifdef USE_FLOAT_TYPE
            case FLOAT_TYPE:
                Generic_EditFloat(MenuItem, Dir);
                break;
#endif
        }
    }
}
#endif

char *strcpy_const(char *dest, MENU_ITEM_STORAGE char *src)
{
    while(*src) {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = 0;
    return dest;
}

char *strncpy_const(char *dest, MENU_ITEM_STORAGE char *src, size_t n)
{
    size_t k = 0;
    while((*src) && (k < n)) {
        *dest = *src;
        dest++;
        src++;
        k++;
    }
    *dest = 0;
    return dest;
}

char *Menu_GetText(char *dest, const Menu_Item_t *MenuItem)
{
    *dest = 0;
    if((MenuItem == &NULL_MENU) || (MenuItem == NULL))
        return dest;
    if(MenuItem->Text && (*MenuItem->Text)) {
        return strcpy_const(dest, MenuItem->Text);
    }
    return dest;
}

#ifdef USE_DATA
#if 1
char *Menu_DataStr(char *dest, const Menu_Item_t *MenuItem)
{
    int i;
    *dest = 0;
    if((MenuItem == &NULL_MENU) || (MenuItem == NULL))
        return dest;
    if((MenuItem->DataItem == &NULL_DATA) || (MenuItem->DataItem == NULL))
        return dest;
    if(MenuItem->DataItem->DataPtr == NULL)
        return dest;

    switch(MenuItem->DataItem->DataType) {
        case BIT_TYPE:
            *dest = '0';
            if(MenuItem->DataItem->Bit < 8) {
                if((*(unsigned short int *)MenuItem->DataItem->DataPtr) & (1 << MenuItem->DataItem->Bit))
                    *dest = '1';
            } else if(MenuItem->DataItem->Bit < 16) {
                if((*(unsigned int *)MenuItem->DataItem->DataPtr) & (1 << MenuItem->DataItem->Bit))
                    *dest = '1';
            } else if(MenuItem->DataItem->Bit < 32) {
                if((*(unsigned long int *)MenuItem->DataItem->DataPtr) & (1L << MenuItem->DataItem->Bit))
                    *dest = '1';
            } else { // if(MenuItem->DataItem->Bit < 64) {
                if((*(unsigned long long int *)MenuItem->DataItem->DataPtr) & (1LL << MenuItem->DataItem->Bit))
                    *dest = '1';
            }
            dest++;
            *dest = 0;
            break;
        case UNSIGNED_TYPE:
            switch(MenuItem->DataItem->Size) {
                case 1:
                    i = *(unsigned short int *)MenuItem->DataItem->DataPtr;
                    sprinti(dest, "%u", i);
                    break;
                case 2:
                    sprinti(dest, "%u", (unsigned int)(*(unsigned int *)(MenuItem->DataItem->DataPtr)));
                    break;
                case 4:
                    sprintl(dest, "%Lu", (unsigned long int)(*(unsigned long int *)(MenuItem->DataItem->DataPtr)));
                    break;
                case 8:
                    sprintl(dest, "%LLu", (unsigned long long int)(*(unsigned long long int *)(MenuItem->DataItem->DataPtr)));
                    break;
                default:
                    strcpy(dest, QUESTION);
                    break;
            }
            break;
        case SIGNED_TYPE:
            switch(MenuItem->DataItem->Size) {
                case 1:
                    i = *(signed short int *)MenuItem->DataItem->DataPtr;
                    sprinti(dest, "%d", i);
                    break;
                case 2:
                    sprinti(dest, "%d", (signed int)(*(signed int *)(MenuItem->DataItem->DataPtr)));
                    break;
                case 4:
                    sprintl(dest, "%Ld", (signed long int)(*(signed long int *)(MenuItem->DataItem->DataPtr)));
                    break;
                case 8:
                    sprintl(dest, "%LLd", (signed long long int)(*(signed long long int *)(MenuItem->DataItem->DataPtr)));
                    break;
                default:
                    strcpy(dest, QUESTION);
                    break;
            }
            break;
#ifdef USE_FLOAT_TYPE
        case FLOAT_TYPE:
            switch(MenuItem->DataItem->Size) {
                case 4:
                    sprintf(dest, "%g", (double)(*(double *)(MenuItem->DataItem->DataPtr)));
                    break;
                case 8:
                    sprintf(dest, "%g", (long double)(*(long double *)(MenuItem->DataItem->DataPtr)));
                    break;
                default:
                    strcpy(dest, QUESTION);
                    break;
            }
            break;
#endif
    }
    return dest;
}
#else
char *Menu_DataStr(char *dest, MENU_ITEM_STORAGE char *formatStr, const Menu_Item_t *MenuItem)
{
    int i;
    *dest = 0;
    if((MenuItem == &NULL_MENU) || (MenuItem == NULL))
        return dest;
    if((MenuItem->DataItem == &NULL_DATA) || (MenuItem->DataItem == NULL))
        return dest;
    if(MenuItem->DataItem->DataPtr == NULL)
        return dest;

    switch(MenuItem->DataItem->DataType) {
        case BIT_TYPE:
            *dest = '0';
            if(MenuItem->DataItem->Bit < 8) {
                if((*(unsigned short int *)MenuItem->DataItem->DataPtr) & (1 << MenuItem->DataItem->Bit))
                    *dest = '1';
            } else if(MenuItem->DataItem->Bit < 16) {
                if((*(unsigned int *)MenuItem->DataItem->DataPtr) & (1 << MenuItem->DataItem->Bit))
                    *dest = '1';
            } else if(MenuItem->DataItem->Bit < 32) {
                if((*(unsigned long int *)MenuItem->DataItem->DataPtr) & (1L << MenuItem->DataItem->Bit))
                    *dest = '1';
            } else { // if(MenuItem->DataItem->Bit < 64) {
                if((*(unsigned long long int *)MenuItem->DataItem->DataPtr) & (1LL << MenuItem->DataItem->Bit))
                    *dest = '1';
            }
            dest++;
            *dest = 0;
            break;
        case UNSIGNED_TYPE:
            switch(MenuItem->DataItem->Size) {
                case 1:
                    i = *(unsigned short int *)MenuItem->DataItem->DataPtr;
                    sprinti(dest, formatStr, i);
                    break;
                case 2:
                    sprinti(dest, formatStr, (unsigned int)(*(unsigned int *)(MenuItem->DataItem->DataPtr)));
                    break;
                case 4:
                    sprintl(dest, formatStr, (unsigned long int)(*(unsigned long int *)(MenuItem->DataItem->DataPtr)));
                    break;
                case 8:
                    sprintl(dest, formatStr, (unsigned long long int)(*(unsigned long long int *)(MenuItem->DataItem->DataPtr)));
                    break;
                default:
                    strcpy(dest, QUESTION);
                    break;
            }
            break;
        case SIGNED_TYPE:
            switch(MenuItem->DataItem->Size) {
                case 1:
                    i = *(signed short int *)MenuItem->DataItem->DataPtr;
                    sprinti(dest, formatStr, i);
                    break;
                case 2:
                    sprinti(dest, formatStr, (signed int)(*(signed int *)(MenuItem->DataItem->DataPtr)));
                    break;
                case 4:
                    sprintl(dest, formatStr, (signed long int)(*(signed long int *)(MenuItem->DataItem->DataPtr)));
                    break;
                case 8:
                    sprintl(dest, formatStr, (signed long long int)(*(signed long long int *)(MenuItem->DataItem->DataPtr)));
                    break;
                default:
                    strcpy(dest, QUESTION);
                    break;
            }
            break;
#ifdef USE_FLOAT_TYPE
        case FLOAT_TYPE:
            switch(MenuItem->DataItem->Size) {
                case 4:
                    sprintf(dest, formatStr, (double)(*(double *)(MenuItem->DataItem->DataPtr)));
                    break;
                case 8:
                    sprintf(dest, formatStr, (long double)(*(long double *)(MenuItem->DataItem->DataPtr)));
                    break;
                default:
                    strcpy(dest, QUESTION);
                    break;
            }
            break;
#endif
    }
    return dest;
}
#endif
#endif