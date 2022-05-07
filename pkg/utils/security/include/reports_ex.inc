use uo;
use os;
use file;

/*
 *
 * VerifyDirectoryExists( path )
 *
 * Parameters:
 * path: string to a given directory within a pkg. Example: :security:logs/myLogs
 *
 * Purpose:
 * Verifies the folders exist in a given path, if they do not they are
 * created.
 *
 * Date: 2022-04-30 01:58:45
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * N/A
 *
 */
function VerifyDirectoryExists( path )

	var file_path := ParseDirectoryPath( path ),
		dir_name := file_path.Package,
		find_dir := 0,
		i := 1;

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

/*
 *
 * ParseFilePath( file_path )
 *
 * Parameters:
 * file_path: Path to a file. Example: :security:logs/myLogs
 *
 * Purpose:
 * To break a string path into a struct containing the package in the
 * Package key and directories in the Directories key.
 *
 * Date: 2022-04-30 02:00:57
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * Struct containing 'Package' and 'Directories' keys
 *
 */
function ParseDirectoryPath( file_path )

	//Find positions of ":" characters
	var colon_a := Find( file_path, ":", 1 ),
		colon_b := Find( file_path, ":", 2 ),
		parsed := Struct{ "Package", "Directories" };

	if( !colon_a || !colon_b )
		// If no package, replace with default position (should always have a pkg but Justin Case...
		parsed.Package := "::";
		parsed.Directories := file_path;
	else
		parsed.Package := file_path[colon_a, colon_b];
		parsed.Directories := file_path[colon_b+1, Len( file_path )];
	endif

	return parsed;
endfunction
