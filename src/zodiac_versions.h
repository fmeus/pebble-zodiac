/*
  Project     : Zodiac watchface
  Copyright   : Copyright (c) 2011-2013 Little Gem Software. All rights reserved.
  Revision    : $Id: lgs_languages.h 1076 2013-05-06 08:04:47Z fmeus_lgs $
*/

#if defined(FORMAT_DMY)
	#define APP_NAME "Zodiac DMY"
	#define MY_UUID { 0x90, 0xFD, 0xE3, 0xAF, 0xAE, 0x49, 0x4F, 0xCC, 0x9A, 0xBD, 0xE4, 0x45, 0x77, 0x14, 0x44, 0x67 }
	static char date_format[] = "%d/%m/%Y";
	static char date_text[] = "00/00/0000";

#elif defined(FORMAT_MDY)
	#define APP_NAME "Zodiac MDY"
	#define MY_UUID { 0x61, 0x50, 0x1F, 0x90, 0xDD, 0xC7, 0x4C, 0x76, 0xB7, 0xB6, 0xEA, 0x15, 0xE2, 0x5D, 0xA5, 0x77 }
	static char date_format[] = "%m/%d/%Y";
	static char date_text[] = "00/00/0000";
#endif
