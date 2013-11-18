#!/bin/bash

VER=2.0
REL=./releases/

function std_version {
	echo "#define FORMAT_$1" > src/zodiac_config.h
	pebble clean
	pebble build
	mv build/pebble-zodiac.pbw $REL/zodiac_$1_v$2.pbw
}


function build {
	std_version $1 $VER
}


formats=(DMY MDY)
for fmt in "${formats[@]}"
do
	build $fmt
done
