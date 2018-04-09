// $Id: desc.inc 1329 2011-11-07 20:24:44Z AustinHeilman $
//
//

use uo;

include ":itemutils:itemdesc";
include ":itemutils:itemInfo";

/*
 * FormatDescForObjectType(source, plural:=0)
 *
 * Purpose
 * Retrieves the DESC entry for an object type (or from the item's object type) and parses it.
 *
 * Parameters
 * source:	Desc string to parse or an object.
 *              If it is an item or integer it will retrieve its itemdesc.cfg 'Desc' entry.
 *		If it is a string, it will parse that.
 * plural:	0 - Will parse the item description in singular form.
 *		1 - Will parse the item in plural form.
 *
 * Return value
 * Returns a string
 *
 */
function FormatDescForObjectType(source, plural:=0)
	var desc := GetObjTypeDesc(source);
	if ( desc.errortext )
		return desc;
	endif
	
	desc := ParseDescString(desc, plural);
	
	return desc;
endfunction

/*
 * GetObjTypeDesc(source)
 *
 * Purpose
 * Retrieves the DESC entry from an itemdesc.cfg or falls back on tiles.cfg
 *
 * Parameters
 * source:	Desc string for an object.
 *              If it is an item or integer it will retrieve its itemdesc.cfg 'Desc' entry
 *              or Desc from tiles.cfg for its graphic.
 *
 * Return value
 * Returns a string on success
 * Returns an error on failure
 *
 *
function GetObjTypeDesc(source)
	var obj_type := 0;
	if ( source.IsA(POLCLASS_ITEM) )
		obj_type := source.objtype;
	else
		obj_type := CInt(source);
	endif

	if ( !obj_type )
		return error{"errortext":="Invalid object type Id. ["+source+"]"};
	endif
	
	var desc;
	
	var itemdesc_elem := GetItemDescInfo(obj_type);	
	if ( itemdesc_elem )
		desc := GetConfigString(itemdesc_elem, "Desc");
		if ( desc )
			return desc;
		endif
	
		desc := GetConfigString(itemdesc_elem, "Name");
		if ( desc )
			return desc;
		endif
	endif
	
	var graphic := GetObjTypeGraphic(obj_type);
	var tile_elem := IU_GetTileInfo(graphic);
	desc := GetConfigString(tile_elem, "Desc");
	
	if ( desc )
		return desc;
	endif
endfunction
*/
/*
 * ParseDescString(desc_text, plural)
 *
 * Purpose
 * Takes in a desc string and parses it to be singular or plural.
 * e.g. bread loa%ves/f% could be "bread loaf" or "bread loaves"
 *
 * Parameters
 * desc_text:	Desc string to parse or an object.
 *
 * plural:	0 - Will parse the item description in singular form.
 *		1 - Will parse the item in plural form.
 *
 * Return value
 * Returns a string
 *
 */
function ParseDescString(desc_text, plural:=0)
	if ( !desc_text["%"] ) // Plain desc line - easy enough to deal with.
		if ( plural == 0 )
			return desc_text;
		elseif ( desc_text[Len(desc_text)] in array{"s"} )
			return desc_text;
		elseif ( desc_text[Len(desc_text)] in array{"h","j","x","z"} )
			return desc_text+"es";
		else
			return desc_text+"s";
		endif
	endif

	/* This section will deal with:
	 	Bread Stick%s
		Bread Loa%ves/f%
		Lump%s% of Coal
		Loa%ves/f% of Bread
	*/
	var first := Find(desc_text, "%", 1);
	var slash := Find(desc_text, "/", first);
	var second := Find(desc_text, "%", first + 1);
	
	if ( !second )
		// Force-append a % if a '/' exists!
		desc_text += "%";
		second := Len(desc_text);
	endif
	if ( !slash )
		slash := second;
	endif

	// wwwww%xx/yy%zzzzz
	
	var s_plur := desc_text[first+1, slash-(first+1)];
	var s_sing := "";
	if ( slash != first )
		s_sing := desc_text[slash+1, second-(slash+1)];
	endif

	var s_first := desc_text[1, first-1];
	var s_middle := s_plur;
	var s_last := "";
	if ( Len(desc_text) > second )
		s_last := desc_text[second+1, Len(desc_text)-second];
	endif

	if ( plural == 0 )
		s_middle := s_sing;
		s_last := "";
	endif

	if ( !s_first )
		s_first := "";
	endif
	if ( !s_middle )
		s_middle := "";
	endif
	if ( !s_last )
		s_last := "";
	endif

	return s_first + s_middle + s_last;
endfunction
