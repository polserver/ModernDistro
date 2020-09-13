use uo;
use os;
use datafile;

function CHARACTER_GetDatafile()

	var datafile := OpenDataFile( ":character:character" );
	if( !datafile )
		datafile := CreateDataFile( ":character:character", DF_KEYTYPE_INTEGER );
		if( datafile.errortext )
			return error{"errortext":="Could not open ':character:character' : "+datafile.errortext};
		endif
	endif

	return datafile;
endfunction

function CHARACTER_GetCharacterElem( byref serial )

	var datafile := CHARACTER_GetDatafile(),
		df_elem := 0;

	if( datafile.errortext )
		return datafile.errortext;
	endif

	df_elem := datafile.FindElement( serial );
	if( !df_elem )
		df_elem := datafile.CreateElement( serial );
		if( df_elem.errortext )
			return df_elem.errortext;
		endif
	endif

	return df_elem;
endfunction

function CHARACTER_DeleteCharacterElem( byref serial )

	var datafile := CHARACTER_GetDatafile(),
		del_elem := 0;

	if( datafile.errortext )
		return datafile.errortext;
	endif

	del_elem := datafile.DeleteElement( serial );
	if( del_elem.errortext )
		return del_elem.errortext;
	endif

	return 1;
endfunction