#ifndef LV_CONF_H
#define LV_CONF_H

#define LV_USE_OS  LV_OS_NONE

/* Use Arduino millis() for ticks */
#define LV_TICK_CUSTOM 1
#define LV_TICK_CUSTOM_INCLUDE "Arduino.h"
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (millis())

/* Display resolution (set this to YOUR screen) */
#define LV_HOR_RES_MAX 320   // ILI9341 native
#define LV_VER_RES_MAX 240

/* Colour depth */
#define LV_COLOR_DEPTH 16

/* LVGL heap size */
#define LV_MEM_SIZE (48U * 1024U)

/* Interface for Lovyan_GFX */
#define LV_USE_LOVYAN_GFX 1

#if LV_USE_LOVYAN_GFX
    #define LV_LGFX_USER_INCLUDE "my_display.hpp"
#endif /*LV_USE_LOVYAN_GFX*/

#endif // LV_CONF_H

//fonts
#define LV_FONT_MONTSERRAT_14  1
#define LV_FONT_MONTSERRAT_20  1
#define LV_FONT_MONTSERRAT_28  1
#define LV_FONT_MONTSERRAT_24 1
#define LV_FONT_MONTSERRAT_40 1