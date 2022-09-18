#!/bin/bash

#- $Id: ECompile.bat 373 2006-06-17 18:27:33Z austinheilman $

#-- If a special path is needed to ecompile.exe set it here
#-- Path is considered to be run from the root if started by starthere.bat
ECOMPILE_PATH="./scripts/ecompile"
#-- Path is considered the default ecompile configuration file for linux systems.
ECOMPILE_CONF_PATH="./scripts/ecompile-linux.cfg"

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
	$ECOMPILE_PATH -C "${ECOMPILE_CONF_PATH}" "${CMD}"
	RETURN_TO_MENU
	}

#-- COMPILE_DIRECTORY() FUNCTION
COMPILE_DIRECTORY()
	{
	clear
	read -p "Path to DIRECTORY: " CMD
	"${ECOMPILE_PATH}" -A -b -f -C "${ECOMPILE_CONF_PATH}" "${CMD}"
	RETURN_TO_MENU
	}

#-- COMPILE_ALL_SCRIPTS() FUNCTION
COMPILE_ALL_SCRIPTS()
	{
	"${ECOMPILE_PATH}" -A -b -f -C "${ECOMPILE_CONF_PATH}"
	RETURN_TO_MENU
	}

#-- COMPILE_ALL_SCRIPTS_OPTXT() FUNCTION
COMPILE_ALL_SCRIPTS_OPTXT()
	{
	#ensure the log directory exists
	[ -d log ] || mkdir log
        echo -e "\nCompilation starting"
	echo "Compiler: ${ECOMPILE_PATH}"
        echo "Compiler configuration file: ${ECOMPILE_CONF_PATH}"
	echo "Compiler log files: ./log/"
	"${ECOMPILE_PATH}" -A -b -f -T -C "${ECOMPILE_CONF_PATH}" >log/ecompile.log 2>log/ecompile_error.log
	echo -e "\nCompilation complete."
	RETURN_TO_MENU
	}

#-- GENERATE DEFAULT LINUX ECOMPILE CONFIG
GENCONFIG()
	{
	[ -f "${ECOMPILE_CONF_PATH}" ] || \
		( \
	       	cp scripts/ecompile.cfg.example "${ECOMPILE_CONF_PATH}" && \
		sed -i -e 's|ModuleDirectory=.*|ModuleDirectory=./scripts/modules|g' "${ECOMPILE_CONF_PATH}" && \
		sed -i -e 's|IncludeDirectory=.*|IncludeDirectory=./scripts|g' "${ECOMPILE_CONF_PATH}" && \
		sed -i -e 's|PolScriptRoot=.*|PolScriptRoot=./scripts|g' "${ECOMPILE_CONF_PATH}" && \
		sed -i -e 's|PackageRoot=.*DistroDev\\pkg|PackageRoot=./pkg|g' "${ECOMPILE_CONF_PATH}" && \
		sed -i -e 's|PackageRoot=.*DistroDev\\optpkg|PackageRoot=./optpkg|g' "${ECOMPILE_CONF_PATH}" && \
		sed -i -e 's|PackageRoot=.*DistroDev\\devpkg|PackageRoot=./devpkg|g' "${ECOMPILE_CONF_PATH}" \
	        )
        
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
	GENCONFIG
	clear
	echo "Ecompile.bat (V 1.0) by MetaPhaze"
	echo "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
	echo "Command        Purpose"
	echo "-------   -------------------------"
	echo " [ a ]  - Compile a specific script."
	echo " [ b ]  - Compile a directory."
	echo " [ c ]  - Compile all .src scripts."
	echo " [ d ]  - Compile all scripts and output to ecompile.log"
	echo " [ e ]  - Regenerate ${ECOMPILE_CONF_PATH}"
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
        if [ "${CMD}" == "e" ]
                then 
			rm -f "$ECOMPILE_CONF_PATH"
			RETURN_TO_MENU
                fi
	if [ "${CMD}" == "x" ] 
		then QUIT
		fi

	echo ""
	echo " Invalid command."
	RETURN_TO_MENU
	}

MENU
