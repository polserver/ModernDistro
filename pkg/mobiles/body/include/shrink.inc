use uo;
use os;
use cfgfile;
use datafile;

function SHRINK_GetCfgFile()

	var cfg_file := ReadConfigFile( ":body:config/shrink" );
	if( cfg_file.errortext )
		SysLog( "Error::SHRINK_GetShrinkCfgFile() - Unable to open shrink.cfg ->"+cfg_file.errortext );
	endif

	return cfg_file;
endfunction

function SHRINK_GetDataFile()
	
	var datafile := OpenDataFile( ":body:shrink" );
	if( !datafile )
		CreateDataFile( ":body:shrink", DF_KEYTYPE_STRING );
		datafile := OpenDataFile( ":body:shrink" );
	endif

	if( datafile )
		return datafile;
	else
		return error{"errortext":="Could not open ':body:shrink' : "+datafile.errortext};
	endif
endfunction

function SHRINK_GetDatafileElem( elem_name )

	var datafile := SHRINK_GetDataFile();
	var data_elem := datafile.FindElement( CStr( elem_name ) );
	if( !data_elem )
		data_elem := datafile.CreateElement( CStr( elem_name ) );
	endif

	if( data_elem.errortext )
		SysLog( "Error::SHRINK_GetDatafileElem() - Unable to find elem ["+elem_name+"] -> "+data_elem.errortext );
		return error{"errortext":="Error::SHRINK_GetDatafileElem() - Unable to find elem ["+elem_name+"] -> "+data_elem.errortext};
	endif

	return data_elem;
endfunction

function SHRINK_GetObjtype( graphic )

	var df_elem := SHRINK_GetDatafileElem( "Categories" ),
		df_prop := 0,
		val := 0;

	if( df_elem.errortext )
		return df_elem.errortext;
	endif

	df_prop := df_elem.GetProp( "ShrinkList" );
	if( df_prop.errortext )
		return df_prop.errortext;
	endif

	if( df_prop.Exists( graphic ) )
		val := df_prop[ graphic ];
	else
		val := error{"errortext":="Error::Could not find shrinkable in dictionary."};
	endif

	return val;
endfunction
