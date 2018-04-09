#!/bin/bash

#- $Id: RealmGen.sh MetaPhaze $

#-- If a special path is needed to uoconvert set it here
UOCNVRT_PATH="/home/ancaria/099-alt/uoconvert"
#----------
BUILD_ALL=0

#-- RETURN_TO_MENU() FUNCTION
RETURN_TO_MENU()
	{
	sleep 2
	MENU
	}

#-- MULTIS.CFG FUNCTION
MULTIS.CFG()
	{
	"${UOCNVRT_PATH}" multis
	mv multis.cfg config
	if [ ${BUILD_ALL} == 1 ]
		then TILES.CFG
		fi
	RETURN_TO_MENU
	}
	
#-- TILES.CFG FUNCTION
TILES.CFG()
	{
	"${UOCNVRT_PATH}" tiles
	mv tiles.cfg config
	if [ ${BUILD_ALL} == 1 ]
		then LANDTILES.CFG
		fi
	RETURN_TO_MENU
	}
	
#-- LANDTILES.CFG FUNCTION
LANDTILES.CFG()
	{
	"${UOCNVRT_PATH}" landtiles
	mv landtiles.cfg config
	if [ ${BUILD_ALL} == 1 ]
		then BUILD_ALL=0
		fi
	RETURN_TO_MENU
	}
	
#-- ALLCONFIGS FUNCTION
ALLCONFIGS()
	{
	BUILD_ALL=1
	MULTIS.CFG
	}
	
#-- REALM_BRITANNIA()
REALM_BRITANNIA()
	{
	echo " Command        Purpose"
	echo "--------  -------------------------"
	echo " [ a ]  - T2A - UOSE Britannia Map."
	echo " [ b ]  - Mondain's Legacy extended Britannia Map."
	read -n 1 -p "Command: " CMD
	if [ "${CMD}" != "b" ]
		then "${UOCNVRT_PATH}" map realm=britannia mapid=0 usedif=1 width=6144 height=4096
		fi
	if [  "${CMD}" == "b" ]
		then "${UOCNVRT_PATH}" map realm=britannia mapid=0 usedif=1 width=7168 height=4096
		fi
	"${UOCNVRT_PATH}" statics realm=britannia
	"${UOCNVRT_PATH}" maptile realm=britannia
	if [ ${BUILD_ALL} == 1 ]
		then REALM_BRITANNIA_ALT
		fi
	RETURN_TO_MENU
	}
	
#-- REALM_BRITANNIA_ALT() FUNCTION
REALM_BRITANNIA_ALT()
	{
	"${UOCNVRT_PATH}" map     realm=britannia_alt mapid=1 usedif=1 width=6144 height=4096
	"${UOCNVRT_PATH}" statics realm=britannia_alt
	"${UOCNVRT_PATH}" maptile realm=britannia_alt
	if [ ${BUILD_ALL} == 1  ]
		then REALM_ILSHENAR
		fi
	RETURN_TO_MENU
	}
	
#-- REALM_ILSHENAR FUNCTION
REALM_ILSHENAR()
	{
	"${UOCNVRT_PATH}" map     realm=ilshenar mapid=2 usedif=1 width=2304 height=1600
	"${UOCNVRT_PATH}" statics realm=ilshenar
	"${UOCNVRT_PATH}" maptile realm=ilshenar
	if [ ${BUILD_ALL} == 1 ]
		then REALM_MALAS
		fi
	RETURN_TO_MENU
	}
	
#-- REALM_MALAS FUNCTION
REALM_MALAS()
	{
	"${UOCNVRT_PATH}" map     realm=malas mapid=3 usedif=1 width=2560 height=2048
	"${UOCNVRT_PATH}" statics realm=malas
	"${UOCNVRT_PATH}" maptile realm=malas
	if [ ${BUILD_ALL} == 1 ]
		then REALM_TOKUNO
		fi
	RETURN_TO_MENU
	}
	
#-- REALM_TOKUNO FUNCTION
REALM_TOKUNO()
	{
	"${UOCNVRT_PATH}" map     realm=tokuno mapid=4 usedif=1 width=1448 height=1448
	"${UOCNVRT_PATH}" statics realm=tokuno
	"${UOCNVRT_PATH}" maptile realm=tokuno
	if [ ${BUILD_ALL} == 1 ]
		then REALM_TERMUR
		fi
	RETURN_TO_MENU
	}
    
#-- REALM_TERMUR FUNCTION
REALM_TERMUR()
	{
	"${UOCNVRT_PATH}" map     realm=termur mapid=5 usedif=1 width=1280 height=4096
	"${UOCNVRT_PATH}" statics realm=termur
	"${UOCNVRT_PATH}" maptile realm=termur
	if [ ${BUILD_ALL} == 1 ]
		then BUILD_ALL=0
		fi
	RETURN_TO_MENU
	}
	
#-- BUILD_ALL_REALMS() FUNCTION
BUILD_ALL_REALMS()
	{
	BUILD_ALL=1
	REALM_BRITANNIA
	}

#-- COPY_CLIENT_FILES() FUNCTION
COPY_CLIENT_FILES()
	{
	clear
	read -p "Full path to UO directory: " UOPATH
	if [ "${UOPATH}" == "" ]
		then RETURN_TO_MENU
		fi
	if [ -e "mul" ]
		then mkdir mul
		fi
	cp "${UOPATH}/multi.*" MUL/
	cp "${UOPATH}/map*" MUL/
	cp "${UOPATH}/staidx*" MUL/
	cp "${UOPATH}/statics*" MUL/
	cp "${UOPATH}/tiledata.mul" MUL/
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
	echo " RealmGen.sh (v 1.0) by MetaPhaze"
	echo "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
	echo " Command        Purpose"
	echo "--------  -------------------------------------"
	echo " [ a ]  - Build multis.cfg"
	echo " [ b ]  - Build tiles.cfg"
	echo " [ c ]  - Build landtiles.cfg"
	echo " [ d ]  - Build all config files"
	echo ""
	echo " [ e ]  - Build 'Britannia' realm     (mapid=0)"
	echo " [ f ]  - Build 'Britannia Alt' realm (mapid=1)"
	echo " [ g ]  - Build 'Ilshenar' realm      (mapid=2)"
	echo " [ h ]  - Build 'Malas' realm         (mapid=3)"
	echo " [ i ]  - Build 'Tokuno' realm        (mapid=4)"
    echo " [ k ]  - Build 'TerMur' realm        (mapid=5)"
	echo " [ j ]  - Build all realms - Takes a very long time!"
	echo ""
	echo " [ o ]  - Copy needed client files to pol/mul/"
	echo ""
	echo " [ x ]  - Back"

	read -n 1 -p "Command: " CMD

	if [  "${CMD}" == "a" ]
		then MULTIS.CFG
		fi
	if [  "${CMD}" == "b" ]
		then TILES.CFG
		fi
	if [  "${CMD}" == "c" ]
		then LANDTILES.CFG
		fi
	if [  "${CMD}" == "d" ]
		then ALLCONFIGS
		fi
	if [  "${CMD}" == "e" ]
		then REALM_BRITANNIA
		fi
	if [  "${CMD}" == "f" ]
		then REALM_BRITANNIA_ALT
		fi
	if [  "${CMD}" == "g" ]
		then REALM_ILSHENAR
		fi
	if [  "${CMD}" == "h" ]
		then REALM_MALAS
		fi
	if [  "${CMD}" == "i" ]
		then REALM_TOKUNO
		fi
    if [  "${CMD}" == "k" ]
		then REALM_TERMUR
		fi
	if [  "${CMD}" == "j" ]
		then BUILD_ALL_REALMS
		fi
	if [  "${CMD}" == "o" ]
		then COPY_CLIENT_FILES
		fi

	if [ "${CMD}" == "x" ]
		then QUIT
		fi

	echo ""
	echo " Invalid command."
	RETURN_TO_MENU
	}

MENU
