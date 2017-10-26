<% 
   use uo;
   use os; 
   set_priority(200);
%>
<html>

<head>
<title>POL Server Performance Center - NPC Grouping</title>
<meta name="GENERATOR" content="Microsoft FrontPage 3.0">
</head>

<body>

<% 
  include header; 
  PrintHeader( "NPC Grouping" );
%>

<table border="1" width="100%">
  <tr>
    <td width="50%">(x,y) center</td>
    <td width="50%">NPC Count</td>
  </tr>
<% foreach zone in (top_ten_npc_zones()) %>
  <tr>
    <td><%=zone.xmid%>,<%=zone.ymid%></td>
	<td><%=zone.npc_count%></td>
  </tr>
<% endforeach %>
</table>
</body>
</html>

<%
use os;

function top_ten_npc_zones()
    var zones := {};
	var zone := {};
	zone.+xmid := 0;
	zone.+ymid := 0;
	zone.+npc_count := 0;
	zones[11] := zone;
	foreach z in zones
	    z := zone;
	endforeach;

	var xmid, ymid, lowest;
	lowest := 0;
	for( xmid := 31; xmid < 6144; xmid := xmid + 64 )
	    // print( CStr(xmid) );
	    for( ymid := 31; ymid < 4096; ymid := ymid + 64 )
			var npc_count := GetNpcCount( xmid, ymid, 32 );
			if (npc_count > lowest)
			    var i;
				for( i := 1; i <= 10; i := i + 1)
				    if (npc_count > zones[i].npc_count)
				        // print( "npc count for zone index " + i + " is " + npc_count );
						zone.xmid := xmid;
						zone.ymid := ymid;
						zone.npc_count := npc_count;
						zones.insert( i, zone );
						zones.erase( 11 );
						// print( zones );
						break;
					endif
				endfor
				lowest := zones[10].npc_count;
			endif
         	sleepms(1);
		endfor
	endfor
	zones.erase( 11 );
	return zones;
endfunction

function GetNpcCount( xmid, ymid, range )
    var count := 0;
	foreach obj in ListObjectsInBox( xmid-range, ymid-range, -128,
	                                 xmid+range, ymid+range, +127 )
	    if (obj.npctemplate)
		    count := count + 1;
		endif
	endforeach
	return count;
endfunction
%>
