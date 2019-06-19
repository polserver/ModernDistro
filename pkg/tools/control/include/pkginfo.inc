use os;
use file;

function GetPackageCfgInfo( pkg_name )

	var info := dictionary;
	var pkgfile := ReadFile( ":"+pkg_name+":pkg.cfg" );
	if( pkgfile.errortext )
		return pkgfile; // Contains an error.
	endif

	foreach line in ( pkgfile )
		var parsed := SplitWords( line );
		var key := parsed[1];

		var pos := Find(line, parsed[2], Len( parsed[1] ) +1 );
		var value := line[pos, Len( line )];

		info[key] := value;
		SleepMs(2);
	endforeach

	return info;
endfunction
