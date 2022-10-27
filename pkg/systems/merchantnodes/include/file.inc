use uo;
use os;
use file;

include ":security:reports_ex";

function MN_OpenNodesXMLFile()

	var xml_file := OpenXMLFile( ":merchantnodes:backups/nodes.xml" );
	if( xml_file.errortext )
		VerifyDirectoryExists( ":merchantnodes:backups/" );
		xml_file := CreateXMLFile();
		xml_file.SetXMLDeclaration( "1.0", "UTF-8", "yes" );
		xml_file.AppendXMLNode( "Facets" );
	endif

	return xml_file;
endfunction

function MN_WriteXMLFile( info )

	var xml_file := MN_OpenNodesXMLFile(),
		alias := "",
		facet_node := 0,
		type_node := 0;
	foreach facet in ( info.Keys() )
		facet_node := xml_file["Facets"][facet];
		if( facet_node.errortext )
			case( facet )
				REALM_BRITANNIA:		alias:="Felucca"; break;
				REALM_BRITANNIA_ALT:	alias:="Trammel"; break;
				REALM_ILSHENAR:			alias:="Ilshenar"; break;
				REALM_MALAS:			alias:="Malas"; break;
				REALM_TOKUNO:			alias:="Tokuno"; break;
				REALM_TERMUR:			alias:="Ter_Mur"; break;
			endcase

			facet_node := xml_file["Facets"][alias];
			if( facet_node.errortext )
				facet_node := xml_file["Facets"].AppendXMLNode( alias, Struct{ "Realm":=facet } );
			endif
		endif

		foreach type in ( info[facet].Keys() )
			if( info[facet][type].Size() )
				type_node := facet_node[type];
				if( type_node.errortext )
					type_node := facet_node.AppendXMLNode( type );
				endif

				foreach node in ( info[facet][type].Keys() )
					type_node.AppendXMLNode( "Node", info[facet][type][node] );
				endforeach
			endif
		endforeach
	endforeach

	xml_file.SaveXML( ":merchantnodes:backups/nodes.xml" );
endfunction
