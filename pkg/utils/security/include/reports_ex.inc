use uo;
use os;
use file;

function VerifyDirectoryExists( path )

	var file_path := ParseFilePath( path ),
		dir_name := file_path.Package,
		find_dir := 0,
		i := 1;

	Print( dir_name );
	var directories := SplitWords( file_path.Directories, "/", -1 );
	for( i:=1; i<=directories.Size(); i+=1 )
		dir_name += directories[i];
		if( dir_name == "" )
			continue;
		endif

		find_dir := ListDirectory( dir_name, LISTDIR_NO_FILES, LISTDIR_LIST_DIRS );
		if( find_dir.errortext )
			CreateDirectory( dir_name );
		endif

		if( i <  directories.Size() )
			dir_name += CChr( 47 ); // Adds a forward slash to the string...
		endif
	endfor

	return 1;
endfunction

function ParseFilePath( file_path )

	//Find positions of ":" characters
	var colon_a := Find( file_path, ":", 1 ),
		colon_b := Find( file_path, ":", 2 ),
		parsed := Struct{ "Package", "Directories" };

	if( !colon_a || !colon_b )
		// If no package, replace with default position (should always have a pkg but Justin Case...
		parsed.Package := "";
		parsed.Directories := file_path;
	else
		parsed.Package := file_path[colon_a, colon_b];
		parsed.Directories := file_path[colon_b+1, Len( file_path )];
	endif

	return parsed;
endfunction
