///////////////////
//  changes the graphic of the housesign
///////////////////
function ChangeHouseSign (who, sign)
	var offset := 0;
	if (sign.objtype == 0x0bd0 || sign.objtype == 0x0bd2)
		offset := 1;
	elseif (GetObjProperty (sign, "originalgraphic") == 0x0bd2)
		offset := 1;
	elseif (GetObjProperty (sign, "originalgraphic") == 0x0bd0)
		offset := 1;
	endif

	SendSysMessage( who, "Select a sign deed to change your house's sign.", 0x3, 0x42 );
	var targ := Target( who );
	if( !targ )
			SendSysMessageCL( who, 1042021, color := 33 ); //Cancelled
			return 0;
	elseif( targ.objtype != 0x2B020 )
			SendSysMessage( who, "This is not a sign deed!", 0x3, 0x21 );
			return 0;
	elseif(!ReserveItem(targ))
			SendSysMessage(who, "You cannot use that.", 0x3, 0x21);
			return 0;
	endif
	ReserveItem(targ);
	var signlist := array { 0x0bcf, 0x0bd1, 0x0b95, 0x0ba3, 0x0ba5, 0x0ba7, 0x0ba9, 0x0bab, 0x0bad, 
							0x0baf, 0x0bb1, 0x0bb3, 0x0bb5, 0x0bb7, 0x0bb9, 0x0bbb, 0x0bbd, 0x0bbf, 
							0x0bc1, 0x0bc3, 0x0bc5, 0x0bc7, 0x0bc9, 0x0bcb, 0x0bcd, 0x0bd3, 0x0bd5,
							0x0bd7, 0x0bd9, 0x0bdb, 0x0bdd, 0x0bdf, 0x0be1, 0x0be3, 0x0be5, 0x0be7,
							0x0be9, 0x0beb, 0x0bed, 0x0bef, 0x0bf1, 0x0bf3, 0x0bf5, 0x0bf7, 0x0bf9,
							0x0bfb, 0x0bfd, 0x0bff, 0x0c01, 0x0c03, 0x0c05, 0x0c07, 0x0c09, 0x0c0b,
							0x0c0d, 0x0c43 };
	var selection := BuildSignGump(who, signlist);
	if (!selection)
		SendSysMessageCL( who, 1042021, color := 33 ); //Cancelled
		return;
	endif
	selection := selection[0];
	sign.graphic := (selection + offset);
	DestroyItem(targ);
endfunction

function BuildSignGump(who, signlist)
	var num_signs := CInt(signlist.Size());
	var multiplier, ItemX, ItemY := 20;
	var gump := GFCreateGump(50, 50);
	GFPage(gump, 0);
	GFResizePic(gump, 0, 0, 9250, 392, 274);
	GFPicTiled(gump, 12, 12, 368, 250, 2624);
	GFAddAlphaRegion(gump, 12, 12, 368, 250);
	GFPage(gump, 1);
	foreach entry in ( signlist )
		entry := Hex(entry);
		multiplier := CInt( _entry_iter - 1 );
		
		ItemX := CInt( 20 + ( 90 * ( multiplier % 4 )));
		GFAddButton(gump, ItemX, ItemY, 2328, 2329, GF_CLOSE_BTN, entry);
		GFTilePic(gump, ItemX + 15, ItemY + 12, entry, 0);

		if( _entry_iter < num_signs && ItemX == 290 && ItemY == 160 )
				GFAddButton(gump, 346, 236, 4005, 4007, GF_PAGE_BTN, gump.cur_page+1);
				GFAddHTMLLocalized(gump, 283, 236, 60, 20, 1011066, 0, 0, LabelColor); //Next Page
				GFPage( gump,  gump.cur_page+1 );
				GFAddButton(gump, 16, 236, 4014, 4016, GF_PAGE_BTN, gump.cur_page-1);
				GFAddHTMLLocalized(gump, 46, 236, 140, 20, 1011067, 0, 0, LabelColor); //Previos Page
		endif
			SleepMS( 2 );
		if ( ItemX == 290 && ItemY == 20 )
			ItemY := CInt(ItemY + 70);
		elseif ( ItemX == 290 && ItemY == 90 )
			ItemY := CInt(ItemY + 70);
		elseif ( ItemX == 290 && ItemY == 160 )
			ItemY := CInt(20);
		endif
	endforeach
	return GFSendGump(who, gump);
endfunction