              MICRO-MENU V2
              https://github.com/abcminiuser/micromenu-v2

          (C) Dean Camera, 2012
        www.fourwalledcubicle.com
     dean [at] fourwalledcubicle.com

        Royalty-free for all uses.

This is the second iteration of my original "MicroMenu" project, a tiny, royalty free library for the easy creation of menus within an embedded system. Menus can be stored completely in flash memory if desired, saving precious SRAM.

The new version of MicroMenu has increased type safety, a better example and an easier to configure interface. It supports menu navigation through both relative (Parent, Child, Next and Previous) menu item links, as well as to absolute menu item nodes.

Only 3 files are needed:
MenuConfig.h 
MicroMenu.h
MicroMenu.c 
See usage in examples.


              MICRO-MENU V3
              https://github.com/LDmicroGitHub/micromenu-v2

          (C) Ihor Nehrutsa, 2018
          LDmicro.GitHub@gmail.com

V3.0.0
Added a data container to the menu.
One simple fundamental data type (bit, short, int, long, float, double) can be stored in the menu structure.
Added callback ShowFunc to display data.
Added callback RefreshFunc to update output when data changes.
Added callback EditFunc to edit data.
Added generic EditFunc to edit data.

MICRO-MENU tested with:
    - MicroC PRO for AVR;
    - MicroC PRO for PIC;
    - MicroC PRO for ARM;
    - Atollic TrueSTUDIO for STM32(GNU Arm Embedded Toolchain).

MICRO-MENU links:
https://www.avrfreaks.net/projects/micromenu

http://easyelectronics.ru/organizaciya-drevovidnogo-menyu.html
https://stm32withoutfear.blogspot.com/2017/02/stm32-micromenu-2.html
