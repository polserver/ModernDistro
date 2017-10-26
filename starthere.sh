#!/bin/bash

#- $Id: StartHere.sh MetaPhaze $

#-- If a special path is needed to the batch files set it here
BATCH_PATH="/home/ancaria/099-alt/SH/"
#----------

#-- RETURN_TO_MENU() FUNCTION
RETURN_TO_MENU()
	{
	MENU
	}
	
#-- REALM_GEN() FUNCTION
REALM_GEN()
	{
	"${BATCH_PATH}"RealmGen.sh
	RETURN_TO_MENU
	}

#-- ECOMPILE() FUNCTION
ECOMPILE()
	{
	"${BATCH_PATH}"ECompile.sh
#RETURN_TO_MENU
	}
	
#-- CLEANUP() FUNCTION
CLEANUP()
	{
	"${BATCH_PATH}"CleanUp.sh
	RETURN_TO_MENU
	}

#-- POL() FUNCTION
POL()
	{
	"${BATCH_PATH}"POL.sh
	RETURN_TO_MENU
	}

#-- ECOMPILE() FUNCTION
POL_LOOP()
	{
	"${BATCH_PATH}"LoopPOL.sh
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
	echo " StartHere.sh (v 1.0) by MetaPhaze"
	echo "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
	echo " Command        Purpose"
	echo "---------   -------------------------------------------"
	echo "  [ a ]   - RealmGen menu        (Realm building tools)"
	echo "  [ b ]   - Ecompiler menu       (Ecompile tools)"
	echo "  [ c ]   - Cleanup menu         (File removal tools)"
	echo ""
	echo "  [ d ]   - Start POL        (Returns to menu on exit)"
	echo "  [ e ]   - Keep POL running (Restarts when it exits. Use CTRL+C to stop)"
	echo ""
	echo "  [ x ]   - Quit"

	read -n 1 -p "Command: " CMD

	if [  "${CMD}" == "a" ]
		then REALM_GEN
		fi
	if [  "${CMD}" == "b" ]
		then ECOMPILE
		fi
	if [  "${CMD}" == "c" ]
		then CLEANUP
		fi
	if [  "${CMD}" == "d" ]
		then POL
		fi
	if [  "${CMD}" == "e" ]
		then POL_LOOP
		fi
	if [  "${CMD}" == "x" ]
		then QUIT
		fi

	echo ""
	echo " Invalid command."
	RETURN_TO_MENU
	}

MENU