///////////////////
//
//  Functions for storing and retriving properties about accounts from
//  a datafile ("character_data")
//
///////////////////

use uo;
use os;
use datafile;




///////////////////
//  Gets the information stored in the given property name for the given accountname
///////////////////

function GetAccountProperty (acctname, property)
	set_critical (1);
	UnloadDataFile ("character_data");
	var acctinfodf := OpenDataFile ("character_data");
	if (!acctinfodf)
		acctinfodf := CreateDataFile ("character_data");
		if (!acctinfodf)
			Syslog ("Error opening character data file!");
			set_critical (0);
			return 0;
		endif
	endif

	var entry := acctinfodf.FindElement (acctname);
	if (!entry)
		acctinfodf.CreateElement (acctname);
		entry := acctinfodf.FindElement (acctname);
	endif
	var retvalue := entry.GetProp (property);
	set_critical (0);
	return retvalue;
endfunction




///////////////////
//  Sets the information stored in the given property name for the given accountname
///////////////////

function SetAccountProperty (acctname, property, value)
	set_critical (1);
	var acctinfodf := OpenDataFile ("character_data");
	if (!acctinfodf)
		acctinfodf := CreateDataFile ("character_data");
		if (!acctinfodf)
			Syslog ("Error opening character data file!");
			set_critical (0);
			return 0;
		endif
	endif

	var entry := acctinfodf.FindElement (acctname);
	if (!entry)
		acctinfodf.CreateElement (acctname);
		entry := acctinfodf.FindElement (acctname);
	endif
	
	entry.SetProp (property, value);
	UnloadDataFile ("character_data");
	set_critical (0);
endfunction




///////////////////
//  Erases the information stored in the given property name for the given accountname
///////////////////

function EraseAccountProperty (acctname, property)
	set_critical (1);
	var acctinfodf := OpenDataFile ("character_data");
	if (!acctinfodf)
		acctinfodf := CreateDataFile ("character_data");
		if (!acctinfodf)
			Syslog ("Error opening character data file!");
			set_critical (0);
			return 0;
		endif
	endif

	var entry := acctinfodf.FindElement (acctname);
	if (!entry)
		acctinfodf.CreateElement (acctname);
		entry := acctinfodf.FindElement (acctname);
	endif
	
	entry.EraseProp (property);
	UnloadDataFile ("character_data");
	set_critical (0);
endfunction




///////////////////
//  Used to figure out ranking in playtime information
///////////////////

function GetMonthlyOnlineTimeRanking (acctname, monthstring)
	UnloadDataFile ("character_data");

	var acctinfodf := OpenDataFile ("character_data");
	if (!acctinfodf)
		acctinfodf := CreateDataFile ("character_data");
		if (!acctinfodf)
			Syslog ("Error opening character data file!");
			set_critical (0);
			return 0;
		endif
	endif

	var entry := acctinfodf.FindElement (acctname);
	if (!entry)
		acctinfodf.CreateElement (acctname);
		entry := acctinfodf.FindElement (acctname);
	endif
	var totalplaytime := entry.GetProp (monthstring);
	if (!totalplaytime)
		totalplaytime := 0;
	endif

	var searchonlinetime, total_place := 1;
	foreach searchacctname in ListAccounts ()
		if (searchacctname != acctname)
			entry := acctinfodf.FindElement (searchacctname);
			if (entry)
				searchonlinetime := entry.GetProp (monthstring);
				if (searchonlinetime and searchonlinetime > totalplaytime)
					total_place := total_place + 1;
				endif
			endif
		endif
	endforeach

	return total_place;
endfunction



