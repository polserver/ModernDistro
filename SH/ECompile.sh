#!/bin/bash

#- $Id: ECompile.bat 373 2006-06-17 18:27:33Z austinheilman $

#-- If a special path is needed to ecompile.exe set it here
#-- Path is considered to be run from the root if started by starthere.bat
ECOMPILE_PATH="/home/ancaria/099-alt/scripts/ecompile"
#----------

#-- RETURN_TO_MENU() FUNCTION
RETURN_TO_MENU()
	{
	sleep 2
	MENU
	}

#-- COMPILE_SCRIPT() FUNCTION
COMPILE_SCRIPT()
	{
	clear
	read -p "Path to script to compile: " CMD
	$ECOMPILE_PATH "${CMD}"
	RETURN_TO_MENU
	}

#-- COMPILE_DIRECTORY() FUNCTION
COMPILE_DIRECTORY()
	{
	clear
	read -p "Path to DIRECTORY: " CMD
	$ECOMPILE_PATH -A -b -f "${CMD}"
	RETURN_TO_MENU
	}

#-- COMPILE_ALL_SCRIPTS() FUNCTION
COMPILE_ALL_SCRIPTS()
	{
	$ECOMPILE_PATH -A -b -f
	RETURN_TO_MENU
	}

#-- COMPILE_ALL_SCRIPTS_OPTXT() FUNCTION
COMPILE_ALL_SCRIPTS_OPTXT()
	{
	$ECOMPILE_PATH -A -b -f 2>&1 > ecompile.log
	echo ""
	echo " Compilation complete."
	RETURN_TO_MENU
	}
	
#-- QUIT FUNCTION
QUIT()
	{
	clear
	exit 0
	}

#-- MENU FUNCTION
MENU()
	{
	clear
	echo "Ecompile.bat (V 1.0) by MetaPhaze"
	echo "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
	echo "Command        Purpose"
	echo "-------   -------------------------"
	echo " [ a ]  - Compile a specific script."
	echo " [ b ]  - Compile a directory."
	echo " [ c ]  - Compile all .src scripts."
	echo " [ d ]  - Compile all scripts and output to ecompile.log"
	echo ""
	echo " [ x ]  - Back"

	read -n 1 -p "Command: " CMD

	if [ "${CMD}" == "a" ]
		then COMPILE_SCRIPT
		fi
	if [ "${CMD}" == "b" ]
		then COMPILE_DIRECTORY
		fi
	if [ "${CMD}" == "c" ] 
		then COMPILE_ALL_SCRIPTS
		fi
	if [ "${CMD}" == "d" ]
		then COMPILE_ALL_SCRIPTS_OPTXT
		fi
	if [ "${CMD}" == "x" ] 
		then QUIT
		fi

	echo ""
	echo " Invalid command."
	RETURN_TO_MENU
	}

MENU
