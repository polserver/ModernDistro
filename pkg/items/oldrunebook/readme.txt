=============================================================================
Runebook v3.98	08-MAY-2011	Yukiko hopelivesproject@yahoo.com
Original script by Shilohen (shilohen@hotmail.com)
POL 95 conversion done by Yukiko
=============================================================================


Features
=============================================================

	OSI style runebook.
	Add runes by dragging recall rune on it.
	Recharge by dragging recall scrolls on it.
	Can store up to 16 recall runes.
	Allow rune removal and default location.
	
=============================================================


Installation
=============================================================

	This package requires POL097 or greater.
	
	Remove any previous runebook package.
	
	Unzip the file in \pol\pkg\runebook
		
	
	The runebooks are newbied presently, if you don't want this,
	remove
	
		Newbie	1
		
	from the itemdesc.cfg
	
	
	To be able to use the default location feature, you'll have
	to alter recall and gate spell scripts this way:
	
	Find either of those lines in the script:
	
		if( cast_on.objtype != UOBJ_RUNE )
		or
		if( cast_on.objtype != 0x1F14 )
		
		
		
	Replace it with either, depanding if you use objtype.inc or not :
	
		if( cast_on.objtype != UOBJ_RUNE and cast_on.objtype != 0x6100 )
		or
		if( cast_on.objtype != 0x1F14 and cast_on.objtype != 0x6100 )
		

	You're ready to go :)
	
=============================================================