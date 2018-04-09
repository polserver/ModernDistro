/* $Id: itemInfo.inc 375 2006-06-17 19:26:32Z austinheilman $
 *
 */
use uo;
use os;
use cfgfile;
use polsys;


/*
 * IU_GetTileInfo(graphic)
 *
 * Purpose
 * Retrieves information on a tile from ::tiles.cfg
 *
 * Parameters
 * graphic:	Graphic number of the tile to retrieve information for.
 *
 * Return value
 * Returns a config elem on success.
 * Returns an error on failure.
 *
 */
function IU_GetTileInfo(graphic)
	var config := ReadConfigFile("::tiles");
	return config[Hex(CInt(graphic))];
endfunction

