/*
  Project     : Zodiac watchface
  Copyright   : Copyright (c) 2011-2013 Little Gem Software. All rights reserved.
  Pebble SDK  : 2.0-BETA2
*/

#if defined(FORMAT_DMY)
	static char date_format[] = "%d/%m/%Y";
	static char date_text[] = "00/00/0000";

#elif defined(FORMAT_MDY)
	static char date_format[] = "%m/%d/%Y";
	static char date_text[] = "00/00/0000";
#endif
