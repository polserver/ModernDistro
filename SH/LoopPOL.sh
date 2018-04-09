#!/bin/bash

#- $Id: LoopPOL.sh MetaPhaze $

#-- If a special path is needed to pol set it here
POL_PATH="/home/ancaria/099-alt/pol"
#----------

#-- START() FUNCTION
START()
	{
	clear
	echo "loopPOL.sh (v 1.0) by MetaPhaze"
	echo "============================="
	echo "  Starting ${POL_PATH}"
	echo "============================="
	echo ""

	"${POL_PATH}" &
	wait $!
	
	echo ""
	echo ""
	echo "============================="
	echo "  POL has finished running"
	echo "============================="
	echo ""
	START
	}

START
