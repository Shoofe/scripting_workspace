#!/bin/bash

if [ $# -eq 0 ]; then

	echo "Error - no arguments"
	exit 1

fi


if [ $1 = "music" ]; then
	
	firefox https://open.spotify.com &
	konsole --hold -e "pipes.sh" &
	konsole --hold -e "vis" &
	exit 0
fi
