/*
  Project     : Signs watchface
  Copyright   : Copyright (c) 2011-2013 Little Gem Software. All rights reserved.
  Revision    : $Id: zodiac.c 1087 2013-05-06 19:54:45Z fmeus_lgs $
*/

#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "zodiac_config.h"
#include "zodiac_versions.h"


// Define the watch face structure
PBL_APP_INFO( MY_UUID, APP_NAME, "Little Gem Software", 1, 0, RESOURCE_ID_IMAGE_MENU_ICON, APP_INFO_WATCH_FACE );


// All UI elements
Window window;
TextLayer     text_time_layer;
TextLayer     text_date_layer;
BmpContainer  sign_image;
GFont         font_time;
GFont         font_date;


// This works around the inability to use the current GRect macro for constants. (Taken from drop_zone.c)
#define ConstantGRect(x, y, w, h) {{(x), (y)}, {(w), (h)}}


// Define layer rectangles (x, y, width, height)
GRect TIME_RECT = ConstantGRect(  0, -15, 144, 50 );
GRect DATE_RECT = ConstantGRect(  0, 135, 144, 22 );


// Work around to handle initial display for minutes to work when testing units_changed
static bool first_cycle = true;


// All zodiac symbols
const int ZODIAC_SIGNS[] = {
  RESOURCE_ID_IMAGE_ARIES,            // 21 mar - 19 apr 0321 - 0419
  RESOURCE_ID_IMAGE_TAURUS,           // 20 apr - 20 mei 0420 - 0520
  RESOURCE_ID_IMAGE_GEMINI,           // 21 mei - 20 jun 0521 - 0620
  RESOURCE_ID_IMAGE_CANCER,           // 21 jun - 22 jul 0621 - 0722
  RESOURCE_ID_IMAGE_LEO,              // 23 jul - 22 aug 0722 - 0822
  RESOURCE_ID_IMAGE_VIRGO,            // 23 aug - 22 sep 0823 - 0922
  RESOURCE_ID_IMAGE_LIBRA,            // 23 sep - 22 okt 0923 - 1022
  RESOURCE_ID_IMAGE_SCORPIO,          // 23 okt - 21 nov 1023 - 1121
  RESOURCE_ID_IMAGE_SAGITTARIUS,      // 22 nov - 21 dec 1122 - 1221
  RESOURCE_ID_IMAGE_CAPRICORN,        // 22 dec - 19 jan 1222 - 0119
  RESOURCE_ID_IMAGE_AQUARIUS,         // 20 jan - 18 feb 0119 - 0218
  RESOURCE_ID_IMAGE_PISCES            // 19 feb - 20 mar 0219 - 0320
};


/*
  set_container_image
*/
void set_container_image( BmpContainer *bmp_container, const int resource_id, GPoint origin ) {
  // Cleanup old image
  layer_remove_from_parent( &bmp_container->layer.layer );
  bmp_deinit_container( bmp_container );

  // Load new image
  bmp_init_container( resource_id, bmp_container );

  // Correct image placement
  GRect frame = layer_get_frame( &bmp_container->layer.layer );
  frame.origin.x = origin.x;
  frame.origin.y = origin.y;
  layer_set_frame( &bmp_container->layer.layer, frame );

  // Add image to the window layer (i.e. make it visible)
  layer_add_child( &window.layer, &bmp_container->layer.layer );
}


/* 
  Determine based on the current month/day what the current Zodiac sign
*/
int get_zodiac_sign( PblTm date ) {
  int zodiac_sign;
  int index = ( (date.tm_mon+1) * 100 ) + date.tm_mday;

  if ( ( index >= 321 ) && ( index <= 419 ) ) { zodiac_sign = 0; }  // Aries
  if ( ( index >= 420 ) && ( index <= 520 ) ) { zodiac_sign = 1; }  // Taurus
  if ( ( index >= 521 ) && ( index <= 620 ) ) { zodiac_sign = 2; }  // Gemini
  if ( ( index >= 621 ) && ( index <= 722 ) ) { zodiac_sign = 3; }  // Cancer
  if ( ( index >= 723 ) && ( index <= 822 ) ) { zodiac_sign = 4; }  // Leo
  if ( ( index >= 823 ) && ( index <= 922 ) ) { zodiac_sign = 5; }  // Virgo
  if ( ( index >= 923 ) && ( index <= 1022 ) ) { zodiac_sign = 6; }  // Libra
  if ( ( index >= 1023 ) && ( index <= 1121 ) ) { zodiac_sign = 7; }  // Scorpio
  if ( ( index >= 1122 ) && ( index <= 1221 ) ) { zodiac_sign = 8; }  // Sagittarius
  if ( ( index >= 1222 ) || ( index <= 119 ) ) { zodiac_sign = 9; }  // Capricorn
  if ( ( index >= 119 ) && ( index <= 218 ) ) { zodiac_sign = 10; }  // Aquarius
  if ( ( index >= 219 ) && ( index <= 320 ) ) { zodiac_sign = 11; }  // Pisces

  return zodiac_sign;
}


/*
  Handle tick event
*/
void handle_tick( AppContextRef ctx, PebbleTickEvent *event ) {
  (void)ctx;

  // Get the current time
  PblTm currentTime;
  get_time( &currentTime );

  // Handle day change
  if ( ( ( event->units_changed & DAY_UNIT ) == DAY_UNIT ) || first_cycle  ) {
    string_format_time( date_text, sizeof( date_text ), date_format, &currentTime );
    text_layer_set_text( &text_date_layer, date_text );

    // Draw image to divide day box from top line
    set_container_image( &sign_image, ZODIAC_SIGNS[ get_zodiac_sign( currentTime ) ], GPoint( 26, 45 ) );
  }

  // Handle time (hour and minute) change
  if ( ( ( event->units_changed & MINUTE_UNIT ) == MINUTE_UNIT ) || first_cycle  ) {
    // Placeholder for time info
    static char time_text[] = "00:00";

    // Display hours (i.e. 18 or 06)
    string_format_time( time_text, sizeof( time_text ), clock_is_24h_style() ? "%H:%M" : "%I:%M", &currentTime );

    // Remove leading zero (only in 12h-mode)
    if ( !clock_is_24h_style() && (time_text[0] == '0') ) {
      memmove( time_text, &time_text[1], sizeof( time_text ) - 1 );
    }

    text_layer_set_text( &text_time_layer, time_text );

    // Update AM/PM indicator (i.e. AM or PM or nothing when using 24-hour style)
    // string_format_time( ampm_text, sizeof( ampm_text ), clock_is_24h_style() ? "\0" : "%p", &currentTime );
    // text_layer_set_text( &text_ampm_layer, ampm_text );
  }

  // Clear
  if ( first_cycle ) {
    first_cycle = false;
  }
}


/*
  Setup new TextLayer
*/
void setup_text_layer( TextLayer *newLayer, GRect rect, GTextAlignment align , GFont font ) {
  text_layer_init( newLayer, rect );
  text_layer_set_text_color( newLayer, GColorWhite );
  text_layer_set_background_color( newLayer, GColorClear );
  text_layer_set_text_alignment( newLayer, align );
  text_layer_set_font( newLayer, font );
}


/*
  Initialization
*/
void handle_init( AppContextRef ctx ) {
  (void)ctx;

  // Load resources
  resource_init_current_app( &APP_RESOURCES );

  // Setup main window
  window_init( &window, "Signs" );
  window_stack_push( &window, true );
  window_set_background_color( &window, GColorBlack );

  // Load custom fonts
  font_time = fonts_load_custom_font( resource_get_handle( RESOURCE_ID_FONT_ARCHISM_SUBSET_48 ) );
  font_date = fonts_load_custom_font( resource_get_handle( RESOURCE_ID_FONT_ARCHISM_20 ) );

  // Time layer
  setup_text_layer( &text_time_layer, TIME_RECT, GTextAlignmentCenter, font_time );
  layer_add_child( &window.layer, &text_time_layer.layer );

  // Date layer
  setup_text_layer( &text_date_layer, DATE_RECT, GTextAlignmentCenter, font_date );
  layer_add_child( &window.layer, &text_date_layer.layer );

  // Force tick event (to avoid blank screen)
  handle_tick( ctx, NULL );
}


/*
  dealloc
*/
void handle_deinit( AppContextRef ctx ) {
  (void)ctx;

  // Unload custom fonts
  fonts_unload_custom_font( font_time );
  fonts_unload_custom_font( font_date );

  // Unload bitmaps
  bmp_deinit_container( &sign_image );
}


/* Main
*/
void pbl_main( void *params ) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .deinit_handler = &handle_deinit,
    .tick_info = {
      .tick_handler = &handle_tick,
      .tick_units = MINUTE_UNIT
    }
  };
  app_event_loop( params, &handlers );
}
