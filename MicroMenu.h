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
#ifndef _MICRO_MENU_H_
#define _MICRO_MENU_H_

#include <stddef.h>
#include <stdint.h>

#include "MenuConfig.h"

#ifdef USE_DATA
// One simple fundamental data type can be stored in a menu structure.
typedef enum MENU_DATA_TYPE_t {
    MENU_TYPE = 0,    // no data in menu structure
    BIT_TYPE,         // for bit, sbit
    UNSIGNED_TYPE,    // for unsigned short, int, long, long long
    SIGNED_TYPE,      // for   signed short, int, long, long long
#ifdef USE_FLOAT_TYPE //
    FLOAT_TYPE,       // for float, double, long double
#endif                //
#ifdef USE_CHAR_TYPE  //
    CHAR_TYPE,        // for char
#endif
} MENU_DATA_TYPE;

//#ifdef USE_DATA_RANGE
// Data range can be defined to edit the data.
typedef struct tag_Data_MinMax {
    signed long long MinValue;
    signed long long MaxValue;
} Data_MinMax_t;
//#endif

// Structure to stote a data info.
typedef struct tag_Data_Item {
    enum MENU_DATA_TYPE_t DataType;
    void *                DataPtr; // pointer to a data
    union {
        unsigned short int Bit;  // bit number for BIT_TYPE
        unsigned short int Size; // or data size of variable with UNSIGNED_TYPE's, SIGNED_TYPE's, etc.
    };
    //#ifdef USE_DATA_RANGE
    const Data_MinMax_t *MinMax;
    //#endif
} Data_Item_t;
#endif

/** Type define for a menu item. Menu items should be initialized via the helper
 *  macro \ref MENU_ITEM(), not created from this type directly in user-code.
 */
#ifndef MICRO_MENU_V3
typedef const struct Menu_Item {
    const struct Menu_Item *Next;     /**< Pointer to the next menu item of this menu item */
    const struct Menu_Item *Previous; /**< Pointer to the previous menu item of this menu item */
    const struct Menu_Item *Parent;   /**< Pointer to the parent menu item of this menu item */
    const struct Menu_Item *Child;    /**< Pointer to the child menu item of this menu item */
    void (*SelectCallback)(void);     /**< Pointer to the optional menu-specific select callback of this menu item */
    void (*EnterCallback)(void);      /**< Pointer to the optional menu-specific enter callback of this menu item */
#if defined(__MIKROC_PRO_FOR_ARM__) || defined(__MIKROC_PRO_FOR_AVR__) || defined(__MIKROC_PRO_FOR_PIC__)
    const char *Text; /**< Menu item text to pass to the menu display callback function */
#else                 /**/
    char Text[]; /**< Menu item text to pass to the menu display callback function */
#endif
} Menu_Item_t;
#else
typedef const struct Menu_Item {
    const struct Menu_Item *Next;                               /**< Pointer to the next menu item of this menu item */
    const struct Menu_Item *Previous;                           /**< Pointer to the previous menu item of this menu item */
    const struct Menu_Item *Parent;                             /**< Pointer to the parent menu item of this menu item */
    const struct Menu_Item *Child;                              /**< Pointer to the child menu item of this menu item */
    void (*SelectCallback)(void);                               /**< Pointer to the optional menu-specific select callback of this menu item */
    void (*EnterCallback)(void);                                /**< Pointer to the optional menu-specific enter callback of this menu item */
    void (*RefreshCallback)(const struct Menu_Item *);          /**< Pointer to the optional menu-specific refresh data callback of this menu item */
    void (*EditCallback)(const struct Menu_Item *, signed int); /**< Pointer to the optional menu-specific edit data callback of this menu item */
    void (*SaveEditCallback)(const struct Menu_Item *);         /**< Pointer to the optional menu-specific edit data callback of this menu item */
    void (*RestoreEditCallback)(const struct Menu_Item *);      /**< Pointer to the optional menu-specific edit data callback of this menu item */
#if defined(__MIKROC_PRO_FOR_ARM__) || defined(__MIKROC_PRO_FOR_AVR__) || defined(__MIKROC_PRO_FOR_PIC__)
    const char *       Text;                                    /**< Menu item text to pass to the menu display callback function */
#else /**/
    char Text[]; /**< Menu item text to pass to the menu display callback function */
#endif
#ifdef USE_DATA
    const Data_Item_t *DataItem;
#endif
} Menu_Item_t;
#endif

// Typedefs for Callback functions
typedef void (*FuncPtr)(void);                                        // SelectCallback, EnterCallback
typedef void (*WriteFunc)(const char *Text);                          // WriteCallback
typedef void (*ShowFunc)(const Menu_Item_t *MenuItem);                // RefreshCallback, ShowCallback, ShowBit, ShowSInt, ShowUInt, etc.
typedef void (*EditFunc)(const Menu_Item_t *MenuItem, signed intDir); // EditCallback, EditBit, EditInt, etc.

/** Creates a new menu item entry with the specified links and callbacks.
*
*  \param[in] Name      Name of the menu entry, must be unique.
*  \param[in] Next      Name of the next linked menu item, or \ref NULL_MENU if no menu link.
*  \param[in] Previous  Name of the previous linked menu item, or \ref NULL_MENU if no menu link.
*  \param[in] Parent    Name of the parent linked menu item, or \ref NULL_MENU if no menu link.
*  \param[in] Child     Name of the child linked menu item, or \ref NULL_MENU if no menu link.
*  \param[in] SelectFunc  Function callback to execute when the menu item is selected, or \c NULL for no callback.
*  \param[in] EnterFunc   Function callback to execute when the menu item is entered, or \c NULL for no callback.
*/
#if !defined(MICRO_MENU_V3) || !defined(USE_DATA)
#define MENU_ITEM(Name, Next, Previous, Parent, Child, SelectFunc, EnterFunc, Text) \
    extern Menu_Item_t MENU_ITEM_STORAGE Next;                                      \
    extern Menu_Item_t MENU_ITEM_STORAGE Previous;                                  \
    extern Menu_Item_t MENU_ITEM_STORAGE Parent;                                    \
    extern Menu_Item_t MENU_ITEM_STORAGE Child;                                     \
    Menu_Item_t MENU_ITEM_STORAGE Name = {&Next, &Previous, &Parent, &Child, SelectFunc, EnterFunc, Text}
#else
#define MENU_ITEM(Name, Next, Previous, Parent, Child, SelectFunc, EnterFunc, RefreshFunc, EditFunc, Text) \
    extern Menu_Item_t MENU_ITEM_STORAGE Next;                                                             \
    extern Menu_Item_t MENU_ITEM_STORAGE Previous;                                                         \
    extern Menu_Item_t MENU_ITEM_STORAGE Parent;                                                           \
    extern Menu_Item_t MENU_ITEM_STORAGE Child;                                                            \
    Menu_Item_t MENU_ITEM_STORAGE Name = {&Next, &Previous, &Parent, &Child, SelectFunc, EnterFunc, RefreshFunc, EditFunc, NULL, NULL, Text, &NULL_DATA}

#define DATA_ITEM(Name, Next, Previous, Parent, Child, SelectFunc, EnterFunc, RefreshFunc, EditFunc, Text, DataType, Data, SizeOrBit) \
    extern Menu_Item_t MENU_ITEM_STORAGE Next;                                                                                        \
    extern Menu_Item_t MENU_ITEM_STORAGE Previous;                                                                                    \
    extern Menu_Item_t MENU_ITEM_STORAGE Parent;                                                                                      \
    extern Menu_Item_t MENU_ITEM_STORAGE Child;                                                                                       \
    Data_Item_t MENU_ITEM_STORAGE Data_##Name = {DataType, &Data, SizeOrBit, &NULL_MINMAX};                                           \
    Menu_Item_t MENU_ITEM_STORAGE Name = {&Next, &Previous, &Parent, &Child, SelectFunc, EnterFunc, RefreshFunc, EditFunc, NULL, NULL, Text, &Data_##Name}

#define DATA_ITEM_(                                                                                                                                    \
    Name, Next, Previous, Parent, Child, SelectFunc, EnterFunc, RefreshFunc, EditFunc, Text, DataType, Data, SizeOrBit, SaveEditFunc, RestoreEditFunc) \
    extern Menu_Item_t MENU_ITEM_STORAGE Next;                                                                                                         \
    extern Menu_Item_t MENU_ITEM_STORAGE Previous;                                                                                                     \
    extern Menu_Item_t MENU_ITEM_STORAGE Parent;                                                                                                       \
    extern Menu_Item_t MENU_ITEM_STORAGE Child;                                                                                                        \
    Data_Item_t MENU_ITEM_STORAGE Data_##Name = {DataType, &Data, SizeOrBit, &NULL_MINMAX};                                                            \
    Menu_Item_t MENU_ITEM_STORAGE Name = {                                                                                                             \
        &Next, &Previous, &Parent, &Child, SelectFunc, EnterFunc, RefreshFunc, EditFunc, SaveEditFunc, RestoreEditFunc, Text, &Data_##Name}

#ifdef USE_DATA_RANGE
#define DATA_RANGE(Name, Next, Previous, Parent, Child, SelectFunc, EnterFunc, RefreshFunc, EditFunc, Text, DataType, Data, SizeOrBit, MinValue, MaxValue) \
    extern Menu_Item_t MENU_ITEM_STORAGE Next;                                                                                                             \
    extern Menu_Item_t MENU_ITEM_STORAGE Previous;                                                                                                         \
    extern Menu_Item_t MENU_ITEM_STORAGE Parent;                                                                                                           \
    extern Menu_Item_t MENU_ITEM_STORAGE Child;                                                                                                            \
    Data_MinMax_t MENU_ITEM_STORAGE MinMax_##Name = {MinValue, MaxValue};                                                                                  \
    Data_Item_t MENU_ITEM_STORAGE Data_##Name = {DataType, &Data, SizeOrBit, &MinMax_##Name};                                                              \
    Menu_Item_t MENU_ITEM_STORAGE Name = {&Next, &Previous, &Parent, &Child, SelectFunc, EnterFunc, RefreshFunc, EditFunc, NULL, NULL, Text, &Data_##Name}
#endif
#endif

/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the menu parent. */
#define MENU_PARENT MENU_ITEM_READ_POINTER(&Menu_GetCurrentMenu()->Parent)

/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the menu child. */
#define MENU_CHILD MENU_ITEM_READ_POINTER(&Menu_GetCurrentMenu()->Child)

/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the next linked menu item. */
#define MENU_NEXT MENU_ITEM_READ_POINTER(&Menu_GetCurrentMenu()->Next)

/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the previous linked menu item. */
#define MENU_PREVIOUS MENU_ITEM_READ_POINTER(&Menu_GetCurrentMenu()->Previous)

/** Return Pointer to the currently selected menu item. */
#define MENU_CURRENT (Menu_GetCurrentMenu())

/** Null menu entry, used in \ref MENU_ITEM() definitions where no menu link is to be made. */
extern Menu_Item_t MENU_ITEM_STORAGE NULL_MENU;
#ifdef USE_DATA
extern Data_Item_t MENU_ITEM_STORAGE NULL_DATA;
extern Data_MinMax_t MENU_ITEM_STORAGE NULL_MINMAX;
#endif

/** Retrieves the currently selected menu item.
 *
 *  \return Pointer to the currently selected menu item.
 */
Menu_Item_t *Menu_GetCurrentMenu(void);

/** Navigates to an absolute or relative menu entry.
*
* \param[in] NewMenu  Pointer to the absolute menu item to select, or one of \ref MENU_PARENT,
*                     \ref MENU_CHILD, \ref MENU_NEXT or \ref MENU_PREVIOUS for relative navigation.
*/
void Menu_Navigate(const Menu_Item_t *NewMenu);

/** Configures the menu text write callback function, fired for all menu items. Within this callback
*  function the user should implement code to display the current menu text stored in \ref MENU_ITEM_STORAGE
*  memory space.
*
*  \ref WriteFunc  Pointer to a callback function to execute for each selected menu item.
*/
void Menu_SetGenericWriteCallback(void (*WriteFunc)(const char *Text));
void Menu_SetGenericShowCallback(void (*ShowFunc)(const Menu_Item_t *MenuItem));

#ifdef MICRO_MENU_V3
void Menu_SetGenericShowSInt(void (*ShowFunc)(const Menu_Item_t *MenuItem));
void Menu_SetGenericShowUInt(void (*ShowFunc)(const Menu_Item_t *MenuItem));
void Menu_SetGenericShowBit(void (*ShowFunc)(const Menu_Item_t *MenuItem));
void Menu_SetGenericShowFloat(void (*ShowFunc)(const Menu_Item_t *MenuItem));

// Optional. Generic_EditBit, Generic_EditInt, Generic_EditFload are used by
// default.
void Menu_SetGenericEditBit(void (*EditFunc)(const Menu_Item_t *MenuItem, signed int Dir));
void Menu_SetGenericEditInt(void (*EditFunc)(const Menu_Item_t *MenuItem, signed int Dir));
void Menu_SetGenericEditFloat(void (*EditFunc)(const Menu_Item_t *MenuItem, signed int Dir));
#endif

/** Enters the currently selected menu item, running its configured callback function (if any). */
void Menu_EnterCurrentItem(void);

#ifdef MICRO_MENU_V3

void Menu_Refresh(const Menu_Item_t *Menu);
void Menu_Edit(const Menu_Item_t *MenuItem, signed int Dir);

void Menu_SaveEditedCurrentItem(void);
void Menu_RestoreEditedCurrentItem(void);
#endif

#define IS_MENU_NULL(Menu) (((Menu) == &NULL_MENU) || ((Menu) == NULL))

char *strcpy_const(char *dest, MENU_ITEM_STORAGE char *src);
char *strncpy_const(char *dest, MENU_ITEM_STORAGE char *src, size_t n);
char *Menu_GetText(char *dest, const Menu_Item_t *MenuItem);
char *Menu_DataStr(char *dest, const Menu_Item_t *MenuItem);

#endif