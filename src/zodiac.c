/*
  Project     : Signs watchface
  Copyright   : Copyright (c) 2011-2013 Little Gem Software. All rights reserved.
  Pebble SDK  : 2.0-BETA2
*/

#include <pebble.h>
#include "zodiac_config.h"
#include "zodiac_versions.h"


// All UI elements
static Window      *window;
static TextLayer   *text_time_layer;
static TextLayer   *text_date_layer;
static GBitmap     *sign_image;
static BitmapLayer *sign_layer;


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
void set_container_image( GBitmap **bmp_container, const int resource_id, GPoint origin ) {
  GBitmap *old_image = *bmp_container;

  // Load new image
  *bmp_container = gbitmap_create_with_resource( resource_id );
  bitmap_layer_set_bitmap( sign_layer, *bmp_container );

  // Cleanup old image
  if ( old_image != NULL ) {
    gbitmap_destroy( old_image );    
  }
}


/* 
  Determine based on the current month/day what the current Zodiac sign
*/
int get_zodiac_sign( struct tm *t ) {
  int zodiac_sign;
  int index = ( (t->tm_mon+1) * 100 ) + t->tm_mday;

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
void handle_tick( struct tm *tick_time, TimeUnits units_changed ) {
  // Handle day change
  if ( ( ( units_changed & DAY_UNIT ) == DAY_UNIT ) || first_cycle  ) {
    strftime( date_text, sizeof( date_text ), date_format, tick_time );
    text_layer_set_text( text_date_layer, date_text );

    // Draw image to divide day box from top line
    set_container_image( &sign_image, ZODIAC_SIGNS[ get_zodiac_sign( tick_time ) ], GPoint( 26, 45 ) );
  }

  // Handle time (hour and minute) change
  if ( ( ( units_changed & MINUTE_UNIT ) == MINUTE_UNIT ) || first_cycle  ) {
    // Placeholder for time info
    static char time_text[] = "00:00";

    // Display hours (i.e. 18 or 06)
    strftime( time_text, sizeof( time_text ), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time );

    // Remove leading zero (only in 12h-mode)
    if ( !clock_is_24h_style() && (time_text[0] == '0') ) {
      memmove( time_text, &time_text[1], sizeof( time_text ) - 1 );
    }

    text_layer_set_text( text_time_layer, time_text );
  }

  // Clear
  if ( first_cycle ) {
    first_cycle = false;
  }
}


/*
  Setup new TextLayer
*/
static TextLayer * setup_text_layer( GRect rect, GTextAlignment align , GFont font ) {
  TextLayer *newLayer = text_layer_create( rect );
  text_layer_set_text_color( newLayer, GColorWhite );
  text_layer_set_background_color( newLayer, GColorClear );
  text_layer_set_text_alignment( newLayer, align );
  text_layer_set_font( newLayer, font );

  return newLayer;
}


/*
  Initialization
*/
static void handle_init( ) {
  // Setup main window
  window = window_create();
  window_stack_push( window, true );
  window_set_background_color( window, GColorBlack );
  Layer *window_layer = window_get_root_layer( window );

  // Sign layer
  GRect sign_frame = { {26,45}, {90, 90} };
  sign_layer = bitmap_layer_create( sign_frame );
  layer_add_child( window_layer, bitmap_layer_get_layer( sign_layer ) );

  // Time layer
  text_time_layer = setup_text_layer( TIME_RECT, GTextAlignmentCenter, fonts_load_custom_font( resource_get_handle( RESOURCE_ID_FONT_ARCHISM_SUBSET_48 ) ) );
  layer_add_child( window_layer, text_layer_get_layer(text_time_layer) );

  // Date layer
  text_date_layer = setup_text_layer( DATE_RECT, GTextAlignmentCenter, fonts_load_custom_font( resource_get_handle( RESOURCE_ID_FONT_ARCHISM_20 ) ) );
  layer_add_child( window_layer, text_layer_get_layer(text_date_layer) );

  // Subscribe to services
  tick_timer_service_subscribe( MINUTE_UNIT, handle_tick );
}


/*
  dealloc
*/
static void handle_deinit( void ) {
  // Subscribe from services
  tick_timer_service_unsubscribe();

  // Destroy objects
  text_layer_destroy( text_time_layer );
  text_layer_destroy( text_date_layer );
  layer_remove_from_parent(bitmap_layer_get_layer(sign_layer));
  bitmap_layer_destroy( sign_layer );
  gbitmap_destroy( sign_image );

  window_destroy( window );
}


/* Main
*/
int main( void ) {
  handle_init();
  app_event_loop();
  handle_deinit();
}