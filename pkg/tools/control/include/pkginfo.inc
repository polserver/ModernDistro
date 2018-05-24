/* $Id$
 *
 */

use os;
use file;

/*
 * GetPackageCfgInfo(pkg_name)
 *
 * Purpose
 * Reads a pkg.cfg for a package and returns its information.
 *
 * Parameters
 * pkg_name:	Name of the package to retrieve the information for.
 *
 * Return Value
 * Returns a dictionary on success.
 * Returns an error on failure.
 *
 */

function GetPackageCfgInfo(pkg_name)
	var info := dictionary;
	var pkgfile := ReadFile(":"+pkg_name+":pkg.cfg");
	if ( pkgfile.errortext )
		return pkgfile; // Contains an error.
	endif

	foreach line in ( pkgfile )
		var parsed := SplitWords(line);
		var key := parsed[1];

		var pos := Find(line, parsed[2], Len(parsed[1])+1);
		var value := line[pos, Len(line)];

		info[key] := value;
		SleepMs(2);
	endforeach

	return info;
endfunction
