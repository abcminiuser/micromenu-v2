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

#ifndef _MICRO_MENU_CONFIG_H_
#define _MICRO_MENU_CONFIG_H_

#if defined(__MIKROC_PRO_FOR_ARM__) || defined(__MIKROC_PRO_FOR_AVR__) || defined(__MIKROC_PRO_FOR_PIC__) // || defined(__arm__)
#define MICRO_MENU_V3

#ifdef MICRO_MENU_V3
#define USE_DATA_RANGE
#endif
#endif

/** Include header to define the functions/macros used for \ref MENU_ITEM_STORAGE
 * and \ref MENU_ITEM_READ_POINTER.
 */
#if defined(__AVR__)
#include <avr/pgmspace.h>
#endif

/** Configuration for the desired memory storage location of created menu items.
 *  This may be configured to locate the menus into FLASH memory space, SRAM, or
 *  any other suitable memory space.
 */
#if defined(__AVR__)
#define MENU_ITEM_STORAGE PROGMEM
#elif defined(__MIKROC_PRO_FOR_ARM__) || defined(__MIKROC_PRO_FOR_AVR__) || defined(__MIKROC_PRO_FOR_PIC__)
#define MENU_ITEM_STORAGE const
#elif defined(__arm__) // && defined(__GNUC__)
#define MENU_ITEM_STORAGE const
#else
#error Use your own definition.
#endif

/** Configuration for the macro or function required to read out a pointer from
 *  the memory storage space set by \ref MENU_ITEM_STORAGE.
 *
 *  \param[in] Addr  Address of the pointer to read
 */
#if defined(__AVR__)
#define MENU_ITEM_READ_POINTER(Addr) (void *)pgm_read_word(Addr)
#elif defined(__MIKROC_PRO_FOR_ARM__) || defined(__MIKROC_PRO_FOR_AVR__) || defined(__MIKROC_PRO_FOR_PIC__)
#define MENU_ITEM_READ_POINTER(Addr) *(Addr)
#elif defined(__arm__) // && defined(__GNUC__)
#define MENU_ITEM_READ_POINTER(Addr) *(Addr)
#else
#error Use your own definition.
#endif

#endif