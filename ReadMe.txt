	           MICRO-MENU V2

	     (C) Dean Camera, 2012
	    www.fourwalledcubicle.com
	
	    Royalty-free for all uses.

This is the second iteration of my original "MicroMenu" project, a tiny, royalty free library for the easy creation of menus within an embedded system. Menus can be stored completely in flash memory if desired, saving precious SRAM.

The new version of MicroMenu has increased type safety, a better example and an easier to configure interface. It supports menu navigation through both relative (Parent, Child, Next and Previous) menu item links, as well as to absolute menu item nodes.




	           MICRO-MENU V3

	     (C) Ihor Nehrutsa, 2018

V3.0
Added a data container to the menu.
One simple fundamental data type (bit, short, int, long, float, double) can be stored in the menu structure.
Added callback ShowFunc to display data.
Added callback RefreshFunc to update output when data changes.
Added callback EditFunc to edit data.

MICRO-MENU tested with 
    - MicroC PRO for ARM.
    - Atollic TrueSTUDIO for STM32(GNU Arm Embedded Toolchain).

