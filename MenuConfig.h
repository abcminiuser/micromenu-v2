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

#ifndef _MICRO_MENU_CONFIG_H_
#define _MICRO_MENU_CONFIG_H_

#if defined(__MIKROC_PRO_FOR_ARM__) || defined(__MIKROC_PRO_FOR_AVR__) || defined(__MIKROC_PRO_FOR_PIC__) // || defined(__arm__)
#define MICRO_MENU_V3 // Comment out this line if only V2 features are needed.
#endif

#ifdef MICRO_MENU_V3
#define USE_DATA
#ifdef USE_DATA
#define USE_DATA_RANGE
#ifdef USE_DATA_RANGE
//#define JUMP_MIN_TO_MAX_TO_MIN // Comment out this line to prevent the jumps from maxValue to MinValue and backward when editing data.
#endif

#define USE_FLOAT_TYPE // Comment out this line if you do not plan to use float, double and long double type variables.
#define USE_CHAR_TYPE  // Comment out this line if you do not plan to use char type variables.
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