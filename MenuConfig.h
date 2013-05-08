/**
              MICRO-MENU V2

          (C) Dean Camera, 2012
        www.fourwalledcubicle.com
     dean [at] fourwalledcubicle.com
	
        Royalty-free for all uses.
	                                  */

#ifndef _MICRO_MENU_CONFIG_H_
#define _MICRO_MENU_CONFIG_H_

	/** Include header to define the functions/macros used for \ref MENU_ITEM_STORAGE
	 *  and \ref MENU_ITEM_READ_POINTER.
	 */
	#include <avr/pgmspace.h>
	
	/** Configuration for the desired memory storage location of created menu items.
	 *  This may be configured to locate the menus into FLASH memory space, SRAM, or
	 *  any other suitable memory space.
	 */
	#define MENU_ITEM_STORAGE              PROGMEM
	
	/** Configuration for the macro or function required to read out a pointer from
	 *  the memory storage space set by \ref MENU_ITEM_STORAGE.
	 *
	 *  \param[in] Addr  Address of the pointer to read
	 */
	#define MENU_ITEM_READ_POINTER(Addr)   (void*)pgm_read_word(Addr)

#endif
