use util;

include "include/shapes";
include ":timedScripts:timedScripts";

CONST FIELD_SHAPE_VERTICAL := 1;
CONST FIELD_SHAPE_HORIZONTAL := 2;

function GetDirectionByFacing(mobile)
	case ( mobile.facing )
		0: return FIELD_SHAPE_HORIZONTAL; break; //N
		2: return FIELD_SHAPE_VERTICAL; break;   //E
		4: return FIELD_SHAPE_HORIZONTAL; break; //S
		6: return FIELD_SHAPE_VERTICAL; break;   //W
	endcase

	return (RandomInt(2)+1); // Diagonal - pick a random direction
endfunction

function GetCoordinatesForField(x, y, direction, size)
	var x2 := x, y2 := y;

	if ( direction == FIELD_SHAPE_VERTICAL )
		y -= size;
		y2 += size;
	elseif ( direction == FIELD_SHAPE_HORIZONTAL )
		x -= size;
		x2 += size;
	else
		x -= size;
		y -= size;
		x2 += size;
		y2 += size;
	endif

	return GetCoordsInRect(x, y, x2, y2);
endfunction

function CreateField(targ, shape, objtype, size, duration, cprops:="", mobile:="", check_los:=1, delay:=2)
	var start_cord, stop_cord, object;
	var field := array{};
	var manual_duration := 0;
        mobile := mobile;

	case ( shape )
		FIELD_SHAPE_VERTICAL:
			if ( size % 2 )
				start_cord := targ.x - ((size - 1) / 2);
				stop_cord := targ.x + CInt(size / 2);
			else
				start_cord := targ.x - (size / 2);
				stop_cord := targ.x + (size / 2) + 1;
			endif

			for i := start_cord to stop_cord
				object := CreateItemAtLocation(i, targ.y, targ.z, objtype, 1, targ.realm);
				if ( object )
					// If there is no SetDuration method, we'll have to handle it manually
					if ( object.SetDuration(duration) == error )
						manual_duration := 1;
					endif
					field.Append(object);
				endif

				SleepMS(delay);
			endfor
			break;
		FIELD_SHAPE_HORIZONTAL:
			if ( size % 2 )
				start_cord := targ.y - ((size - 1) / 2);
				stop_cord := targ.y + CInt(size / 2);
			else
				start_cord := targ.y - (size / 2);
				stop_cord := targ.y + (size / 2) + 1;
			endif

			for i := start_cord to stop_cord
				object := CreateItemAtLocation(targ.x, i, targ.z, objtype, 1, targ.realm);
				if ( object )
					// If there is no SetDuration method, we'll have to handle it manually
					if ( object.SetDuration(duration) == error )
						manual_duration := 1;
					endif
					field.Append(object);
				endif

				SleepMS(delay);
			endfor
			break;
	endcase

	if ( field )
		if ( check_los )
			// object type doesn't matter, it'll be deleted soon
			check_los := CreateItemAtLocation(targ.x, targ.y, targ.z, 0x002, 1, targ.realm);
			check_los.invisible := 1;

			foreach object in field
				if ( !CheckLineOfSight(check_los, object) )
					DestroyItem(object);
					continue;
				endif
			endforeach

			DestroyItem(check_los);
		endif

		// Set cprops
		foreach object in field
			if ( cprops )
				if ( Lower(TypeOf(cprops) == "array") )
					foreach prop_struct in cprops
						SetObjProperty(object, prop_struct.name, prop_struct.value);
					endforeach
				elseif ( Lower(TypeOf(cprops) == "struct") )
					SetObjProperty(object, cprops.name, cprops.value);
				endif
			endif

			SetObjProperty(object, "SummonedField", 1);
		endforeach
	endif

	if ( manual_duration )
		Sleep(duration);

		foreach object in field
			DestroyItem(object);
		endforeach
	endif

	return 1;
endfunction