## Zodiac (Watchface for Pebble Watch)

It shows the time (12h and 24h modes are supported), date (dd/mm/yyyy or mm/dd/yyyy) and the Zodiac sign for the current date.

Compiled version of this watchface can be downloaded [here](http://scriptogr.am/fmeus/pebble-zodiac-watchface).

![Sample](https://dl.dropboxusercontent.com/u/265253/pebble/zodiac/zodiac.png)

The font used is D3 Archism, it is freely available for download [here](http://www.fontspace.com/digitaldreamdesign/d3-archism).


## Build Instructions

Clone this repository in an appropriate directory:

	git clone https://github.com/fmeus/pebble-zodiac.git

Set up waf:

	python ~/pebble-dev/pebble-sdk-release-001/tools/create_pebble_project.py --symlink-only ~/pebble-dev/pebble-sdk-release-001/sdk/ pebble-zodiac
	cd pebble-zodiac

Configure and build:

	./waf configure
	src/makeall.sh

Compiled version can be found in the folder `releases`