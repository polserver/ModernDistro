use polsys;








//////////////////////////////////////////////////
//
// GetFacetNames()
// Purpose: Returns an array names of all active
// facets (realms).
//
// Parameters: None.
//
// Returns: An array of strings containing all
// active facets (realms).
//
/////////////////////////////////////////////////
function GetFacetNames()

	var AllFacets := Dictionary;
	var AllFacetsKeys := array;
	AllFacets := Realms();
	AllFacetsKeys := AllFacets.keys();
	return AllFacetsKeys;
	
endfunction

/////////////////////////////////////////////////
//
// GetFacetInfo( FacetName := "" )
//
// Purpose: Returns a Dictionary structs or a
// single struct. Essentially duplicates
// the polsys.em function Realms(). Included
// for the purist who refers to 'realms' as
// they are known properly in UO
// parlance, 'facets'.
//
// Parameters: facet name or none.
//
// Returns: If no facet name is passed it
// returns a dictionary of structs for all
// active facets (realms). If a string is
// passed returns a struct with the facet's
// specifications or an error if the facet
// does not exist.
// "realm_name" -> "width"
//                 "height"
//                 "mapid"
//                 "toplevel_item_count"
//                 "mobile_count"
//                 "season"
//
/////////////////////////////////////////////////
function GetFacetInfo( FacetName := "" )

	var Facets := Dictionary;
	var EachFacet := Dictionary;
	var AllFacetsKeys := array;

	if(!FacetName)
		return Realms();
	else
		EachFacet := Realms(FacetName);
		if(!EachFacet)
			EachFacet.errortext := "Facet does not exist";
		endif
		return EachFacet;
	endif

endfunction

//////////////////////////////////////////////////
//
// GetFacetDiemensions(FacetName)
// Purpose: Find the dimensions of a facet. 
//
// Parameters: String containing the name of
// the facet..
//
// Returns: An array of 2 integers
// containing the width (X) and height (Y) of
// the facet.
//
/////////////////////////////////////////////////
function GetFacetDimensions(FacetName)

	var Dimensions := array;
	var Facet := struct;
	
	if(!FacetName)
		return 0;
	endif
	Facet := GetFacetInfo(FacetName);
	Dimensions := {Facet.width, Facet.height};
	return Dimensions;

endfunction

