use uo;
use os;

include ":gumps:gumps";

function AddBlueBack( byref gump, width, height )

	AddBackground( 0, 0, width, height, 0x2422 );
	AddImageTiled( 13, 13, width - 26, height - 26, 0xBBC	);
	AddAlphaRegion( 15, 14, width - 29, height );
endfunction