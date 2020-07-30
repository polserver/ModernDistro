use cfgfile;
use os;
use uo;

UnloadConfigFile( ":multihousing:settings" );
var	housing_cfg := ReadConfigFile(":multihousing:settings");
if(!housing_cfg)
	Broadcast("No housing settings file found.");
endif

function get_abandon_time()

var	elem := FindConfigElem( housing_cfg, "Settings" );
var	decayperiod := GetConfigInt( elem, "Abandoned_Timeout" );

return decayperiod;

endfunction

