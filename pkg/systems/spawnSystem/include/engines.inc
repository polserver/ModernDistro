/*

	spawnSystem originally created by Austin for the 0.97 Distro

	Bugs fixed by Edwards

	Adapted for the Basic Distro by Yukiko 10-Sep-2015


*/
use uo;
use os;
use cfgfile;

include ":datafile:datafile";
include ":spawnsystem:settings";

/*
 * SS_GetEnginesList()
 *
 * Purpose
 * Retrieves the list of engines in :spawnsystem:configs/settings.cfg
 *
 * Parameters
 *
 * Return Value
 * Array of strings
 *
 */
function SS_GetEnginesList()

	var cfg_file := SS_GetSettingsCfgFile();
	var cfg_elem := SS_GetSettingsCfgElem( cfg_file, "EngineList" );

	return GetConfigStringArray( cfg_elem, "Engine" );
endfunction

/*
 * SS_EngineEnabled(engine_name)
 *
 * Purpose
 * Tells if an engine is set to be enabled or disabled
 *
 * Parameters
 * engine_name:	Name of the engine to check.
 *
 * Return Value
 * 1 if the engine is enabled
 *
 */
function SS_EngineEnabled( engine_name )

	var settings := SS_GetSettingsCfgFile( engine_name );
	settings := settings["Settings"];

	return CInt( settings.Enabled );
endfunction

/*
 * SS_StartSpawnEngine(engine_name, realm_name, region_name, group_name)
 *
 * Purpose
 * Instructs an engine to do a spawn cycle
 *
 * Parameters
 * engine_name:	Name of the engine to start
 * realm_name:	Name of the realm the engine will run in
 * region_name:	Name of the regin that will be spawning
 * group_name:	Name of the spawn group that will be used
 *
 * Return Value
 * 1 on engine start
 *
 */
function SS_StartSpawnEngine( engine_name, realm_name, region_name, group_name )

	var params := array{engine_name, realm_name, region_name, group_name};
	var script := Start_Script( SS_GetEnginePath( engine_name ), params );

	return script;
endfunction

/*
 * SS_GetEnginePath(engine_name)
 *
 * Purpose
 * Takes in the name of the engine and gives the path to call configs/scripts in it.
 *
 * Parameters
 * engine_name:	Name of the engine to setup the path for.
 *
 * Return Value
 * String
 *
 */
function SS_GetEnginePath( engine_name )

	return ":spawnsystem:engines/"+engine_name+"/spawnCycle";
endfunction
