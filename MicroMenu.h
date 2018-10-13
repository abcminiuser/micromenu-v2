/**
              MICRO-MENU V2

          (C) Dean Camera, 2012
        www.fourwalledcubicle.com
     dean [at] fourwalledcubicle.com

        Royalty-free for all uses.


              MICRO-MENU V3

          (C) Ihor Nehrutsa, 2018
          LDmicro.GitHub@gmail.com
                                      */
#ifndef _MICRO_MENU_H_
#define _MICRO_MENU_H_

#include <stddef.h>
#include <stdint.h>

#include "MenuConfig.h"

// One simple fundamental data type can be stored in a menu structure.
typedef enum MENU_DATA_TYPE_t {
    MENU_TYPE,     // no data in menu structure
    BIT_TYPE,      // for bit, sbit
    UNSIGNED_TYPE, // for unsigned short, int, long, long long
    SIGNED_TYPE,   // for   signed short, int, long, long long
    FLOAT_TYPE,    // for float, double, long double
    CHAR_TYPE,     // for char
} MENU_DATA_TYPE;

// Data range can be defined to edit the data.
typedef struct tag_Data_MinMAx Data_MinMax_t;

typedef struct tag_Data_MinMax {
    long long MinValue;
    long long MaxValue;
} Data_MinMax;

// Structure to stote a data info.
typedef struct tag_Data_Item Data_Item_t;

typedef struct tag_Data_Item {
    enum MENU_DATA_TYPE_t DataType;
    const void *          DataPtr; // pointer to data
    union {
        unsigned short int Bit;  // bit number for BIT_TYPE
        unsigned short int Size; // or data size of variable with UNSIGNED_TYPE's, SIGNED_TYPE's, etc.
    };
#ifdef USE_DATA_RANGE
    Data_MinMax_t *MinMax;
#endif
} Data_Item;

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
    const char *Text;                 /**< Menu item text to pass to the menu display callback function */
} Menu_Item_t;
#else
typedef struct tag_Menu_Item Menu_Item_t;

typedef struct tag_Menu_Item {
    const Menu_Item_t *Next;                         /**< Pointer to the next menu item of this menu item */
    const Menu_Item_t *Previous;                     /**< Pointer to the previous menu item of this menu item */
    const Menu_Item_t *Parent;                       /**< Pointer to the parent menu item of this menu item */
    const Menu_Item_t *Child;                        /**< Pointer to the child menu item of this menu item */
    void (*SelectCallback)(void);                    /**< Pointer to the optional menu-specific select callback of this menu item */
    void (*EnterCallback)(void);                     /**< Pointer to the optional menu-specific enter callback of this menu item */
    void (*RefreshCallback)(void);                   /**< Pointer to the optional menu-specific refresh data callback of this menu item */
    void (*EditCallback)(Menu_Item_t *, signed int); /**< Pointer to the optional menu-specific edit data callback of this menu item */
    const char * Text;                               /**< Menu item text to pass to the menu display callback function */
    Data_Item_t *DataItem;
};
#endif

// Typedefs for Callback functions
typedef void (*FuncPtr)(void);                                        // SelectCallback, EnterCallback, RefreshCallback
typedef void (*WriteFunc)(const char *Text);                          // WriteCallback
typedef void (*ShowFunc)(const Menu_Item_t *MenuItem);                // ShowCallback, ShowBit, ShowSInt, ShowUInt, etc.
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
#ifndef MICRO_MENU_V3
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
    Menu_Item_t MENU_ITEM_STORAGE Name = {&Next, &Previous, &Parent, &Child, SelectFunc, EnterFunc, RefreshFunc, EditFunc, Text, NULL}

#define DATA_ITEM(Name, Next, Previous, Parent, Child, SelectFunc, EnterFunc, RefreshFunc, EditFunc, Text, DataType, Data, SizeOrBit) \
    extern Menu_Item_t MENU_ITEM_STORAGE Next;                                                                                        \
    extern Menu_Item_t MENU_ITEM_STORAGE Previous;                                                                                    \
    extern Menu_Item_t MENU_ITEM_STORAGE Parent;                                                                                      \
    extern Menu_Item_t MENU_ITEM_STORAGE Child;                                                                                       \
    extern Data_Item_t MENU_ITEM_STORAGE Data_##Name = {DataType, &Data, SizeOrBit};                                                  \
    Menu_Item_t MENU_ITEM_STORAGE Name = {                                                                                            \
        &Next, &Previous, &Parent, &Child, SelectFunc, EnterFunc, RefreshFunc, EditFunc, Text, &Data_##Name}

#ifdef USE_DATA_RANGE
#define DATA_RANGE(Name,                                                                             \
                   Next,                                                                             \
                   Previous,                                                                         \
                   Parent,                                                                           \
                   Child,                                                                            \
                   SelectFunc,                                                                       \
                   EnterFunc,                                                                        \
                   RefreshFunc,                                                                      \
                   EditFunc,                                                                         \
                   Text,                                                                             \
                   DataType,                                                                         \
                   Data,                                                                             \
                   SizeOrBit,                                                                        \
                   MinValue,                                                                         \
                   MaxValue)                                                                         \
    extern Menu_Item_t MENU_ITEM_STORAGE Next;                                                       \
    extern Menu_Item_t MENU_ITEM_STORAGE Previous;                                                   \
    extern Menu_Item_t MENU_ITEM_STORAGE Parent;                                                     \
    extern Menu_Item_t MENU_ITEM_STORAGE Child;                                                      \
    extern Data_MinMax_t MENU_ITEM_STORAGE MinMax_##Name = {MinValue, MaxValue};                     \
    extern Data_Item_t MENU_ITEM_STORAGE Data_##Name = {DataType, &Data, SizeOrBit, &MinMax_##Name}; \
    Menu_Item_t MENU_ITEM_STORAGE Name = {                                                           \
        &Next, &Previous, &Parent, &Child, SelectFunc, EnterFunc, RefreshFunc, EditFunc, Text, &Data_##Name}
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
#ifdef MICRO_MENU_V3
void Menu_SetGenericShowCallback(void (*ShowFunc)(const Menu_Item_t *MenuItem));

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
#endif

#endif