use uo;
use os;
use cfgfile;
use datafile;

function BODY_GetCfgFile()

	var cfg_file := ReadConfigFile( ":body:bodyTable" );
	if( cfg_file.errortext )
		SysLog( "Error::GetBodyTableCfgFile() - Unable to open bodyTable.cfg ->"+cfg_file.errortext );
	endif

	return cfg_file;
endfunction

function BODY_GetDataFile()

	var datafile := OpenDataFile( ":body:body" );
	if( !datafile )
		CreateDataFile( ":body:body", DF_KEYTYPE_STRING );
		datafile := OpenDataFile( ":body:body" );
	endif

	if( datafile )
		return datafile;
	else
		return error{"errortext":="Could not open ':body:body' : "+datafile.errortext};
	endif
endfunction

function BODY_GetDatafileElem( elem_name )

	var datafile := BODY_GetDataFile();
	var data_elem := datafile.FindElement( CStr( elem_name ) );
	if( !data_elem )
		data_elem := datafile.CreateElement( CStr( elem_name ) );
	endif

	if( data_elem.errortext )
		SysLog( "Error::BODY_GetDatafileElem() - Unable to find elem ["+elem_name+"] -> "+data_elem.errortext );
		return error{"errortext":="Error::BODY_GetDatafileElem() - Unable to find elem ["+elem_name+"] -> "+data_elem.errortext};
	endif

	return data_elem;
endfunction

function BODY_ListBodiesByType( type:="" )

	if( type == "" )
		var cfg_file := BODY_GetCfgFile();
		return GetConfigIntKeys( cfg_file );
	endif

	type := Lower( type );
	var data_elem := BODY_GetDatafileElem( "Categories" );
	return data_elem.GetProp( type );
endfunction

function HasPaperdoll( byref graphic )

	return ( graphic in { 400, 401, 402, 403, 605, 606, 607, 608, 666, 667, 695, 694, 987 } );
endfunction

function IsMonster( graphic )

	return ( graphic in BODY_ListBodiesByType( "monster" ) );
endfunction

function IsAnimal( graphic )

	return ( graphic in BODY_ListBodiesByType( "animal" ) );
endfunction

function IsHuman( graphic )

	return ( graphic in BODY_ListBodiesByType( "human" ) );
endfunction

function IsSea( graphic )

	return ( graphic in BODY_ListBodiesByType( "sea" ) );
endfunction

function IsEquipment( graphic )

	return ( graphic in BODY_ListBodiesByType( "equipment" ) );
endfunction
