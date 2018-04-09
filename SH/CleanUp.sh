#!/bin/bash

# $Id: CleanUp.sh MetaPhaze $

# -- RETURN_TO_MENU() FUNCTION
RETURN_TO_MENU ()
	{
	sleep 2
	MENU
	}

# -- REMOVE FUNCTION()
REMOVE ()
	{
	if [ $REMOVE_TYPE != "" ] 
		then rm -rf /home/ancaria/099-alt/$REMOVE_TYPE
		fi
	RETURN_TO_MENU
	}

# -- DELETE_CUSTOM FUNCTION()
DELETE_CUSTOM ()
	{
	clear
	read -p "Enter the file type to remove (e.g. *.bak or orphans.txt): " REMOVE_TYPE
	REMOVE
	}

# -- QUIT() FUNCTION
QUIT () { 
	clear
	exit 0
	}

# -- MENU FUNCTION
MENU () 
	{
	clear
	REMOVE_TYPE=""
	echo "CleanUp.sh (v 1.0) by MetaPhaze"
	echo "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
	echo "Command        Purpose"
	echo "-------   ------------------"
	echo " [ a ]  - Remove *.ecl files. (Will need to recompile scripts)"
	echo " [ b ]  - Remove *.bak files."
	echo " [ c ]  - Remove *.dep files."
	echo " [ d ]  - Remove *.log files."
	echo " [ e ]  - Remove *.lst files."
	echo " [ f ]  - Remove *.dbg files."
	echo " [ h ]  - Remove other."
	echo ""
	echo " [ x ] - Back"
	read -n 1 -p "Command: " CMD
	if [ "${CMD}" == "a" ] 
		then REMOVE_TYPE="*.ecl" 
		fi
	if [ "${CMD}" == "b" ] 
		then REMOVE_TYPE="*.bak" 
		fi
	if [ "${CMD}" == "c" ] 
		then REMOVE_TYPE="*.dep" 
		fi
	if [ "${CMD}" == "d" ] 
		then REMOVE_TYPE="*.log" 
		fi
	if [ "${CMD}" == "e" ] 
		then REMOVE_TYPE="*.lst"
		fi
	if [ "${CMD}" == "f" ] 
		then REMOVE_TYPE="*.dbg" 
		fi
	if [ "${CMD}" == "h" ] 
		then DELETE_CUSTOM
		fi
	if [ "${CMD}" == "x" ] 
		then QUIT
		fi
	if [ "${REMOVE_TYPE}" != "" ] 
		then REMOVE
		fi
	echo ""
	echo "Invalid command."
	RETURN_TO_MENU
	}

MENU