/* $Id: goText.inc 375 2006-06-17 19:26:32Z austinheilman $
 *
 */


function Go_Parse_Text(who, text)
	if ( !text )
		return 0;
	endif

	var params := SplitWords(text);

	case ( Lower(params[1]) )
		"refresh":
			return Go_RefreshMyGumps(who);
			break;
		"xyz":
			return Go_GoXYZ(who, params);
			break;
		default:
			return Go_FindLocationByText(who, params, text);
			break;
	endcase

	return 0;
endfunction


function Go_GoXYZ(who, params)
	var num_params := params.Size();

	case ( num_params )
		1: 2:
			SendSysMessage(who, "Error. Insufficient data supplied. \".go xyz x y [z] [string realm name]\"");
			break;
		3:
			params[4] := GetWorldHeight(CInt(params[2]), CInt(params[3]), who.realm);
			params[5] := who.realm;
			break;
		4:
			params[5] := who.realm;
			break;
		default:
			break;
	endcase

	if ( Realms().Exists(Lower(params[5])) )
		var result := MoveObjectToLocation(who, CInt(params[2]), CInt(params[3]), CInt(params[4]), params[5], MOVEOBJECT_FORCELOCATION);
		if( result.errortext )
			GumpPrompt(who, "Error moving: X["+params[2]+"] Y["+params[3]+"] Z["+params[4]+"] - "+result.errortext);
			return 1;
		endif
	else
		GumpPrompt(who, "Invalid Realm name in xyz!");
		return 1;
	endif


	return 1;
endfunction


function Go_FindLocationByText(who, params, text)
	case (Lower(params[params.Size()]))
		"private":
			Go_ByPrivateLocation(who, params, text);
			break;
		"public":
			Go_ByPublicLocation(who, params, text);
			break;
		default:
			Go_ByDefaultLocation(who, params, text);
			break;
	endcase

	return 1;
endfunction

function Go_ByPrivateLocation(who, params, text)

//	if( go_prop.Exists( Lower(params[1]) ) )
//		var temp := go_prop[Lower(params[1])];
//		SendSysMessage(who, "Going to private location ["+params[1]+"]");
	if( go_prop.Exists( Lower(text) ) )
		var temp := go_prop[Lower(text)];
		SendSysMessage(who, "Going to private location ["+text+"]");
		var result := MoveObjectToLocation(who, CInt(temp.X), CInt(temp.Y), CInt(temp.Z), temp.Realm, MOVEOBJECT_FORCELOCATION);
		if ( result.errortext )
			GumpPrompt(who, "Error moving: X["+temp.X+"] Y["+temp.Y+"] Z["+temp.Z+"] - "+result.errortext);
			return 0;
		endif
	else
		GumpPrompt(who, "Private Location ["+params[1]+"] does not exist.");
		return 0;
	endif

	return 0;

endfunction


function Go_ByPublicLocation(who, params, text)

//	if(go_prop_base.Exists( Lower(params[1]) ) )
//		var temp := go_prop_base[Lower(params[1])];
//		SendSysMessage(who, "Going to public location ["+params[1]+"]");
	if(go_prop_base.Exists( Lower(text) ) )
		var temp := go_prop_base[Lower(text)];
		SendSysMessage(who, "Going to public location ["+text+"]");
		var result := MoveObjectToLocation(who, CInt(temp.X), CInt(temp.Y), CInt(temp.Z), temp.Realm, MOVEOBJECT_FORCELOCATION);
		if ( result.errortext )
			GumpPrompt(who, "Error moving: X["+temp.X+"] Y["+temp.Y+"] Z["+temp.Z+"] - "+result.errortext);
			return 0;
		endif
	else
		GumpPrompt(who, "Public Location ["+params[1]+"] does not exist.");
		return 0;
	endif

	return 0;

endfunction


function Go_ByDefaultLocation(who, params, text)
//	if ( go_prop.Exists( Lower(params[1]) ) )
//		var temp := go_prop[Lower(params[1])];
//		SendSysMessage(who, "Going to private location ["+params[1]+"]");
	if ( go_prop.Exists( Lower(text) ) )
		var temp := go_prop[Lower(text)];
		SendSysMessage(who, "Going to private location ["+text+"]");
		var result := MoveObjectToLocation(who, CInt(temp.X), CInt(temp.Y), CInt(temp.Z), temp.Realm, MOVEOBJECT_FORCELOCATION);

		if ( result.errortext )
			GumpPrompt(who, "Error moving: X["+temp.X+"] Y["+temp.Y+"] Z["+temp.Z+"] - "+result.errortext);
			return 0;
		endif
//	elseif( go_prop_base.Exists( Lower(params[1]) ) )
//		var temp := go_prop_base[Lower(params[1])];
//		SendSysMessage(who, "Going to public location["+params[1]+"]");
	elseif( go_prop_base.Exists( Lower(text) ) )
		var temp := go_prop_base[Lower(text)];
		SendSysMessage(who, "Going to public location["+text+"]");
		var result := MoveObjectToLocation(who, CInt(temp.X), CInt(temp.Y), CInt(temp.Z), temp.Realm, MOVEOBJECT_FORCELOCATION);

		if ( result.errortext )
			GumpPrompt(who, "Error moving: X["+temp.X+"] Y["+temp.Y+"] Z["+temp.Z+"] - "+result.errortext);
			return 0;
		endif
	else
		GumpPrompt(who, "Location ["+params[1]+"] does not exist.");
		return 0;
	endif
endfunction
