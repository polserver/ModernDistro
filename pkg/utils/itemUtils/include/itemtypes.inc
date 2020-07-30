//////////////////////////////////////////////////////////////////////////////////////////////////
//												//
// 	This Include contains the following Functions.						//
//												//
// 	IsDoor(door) 		IsHair(hair) 			IsFacialHair(beard)		//
// 	IsJewelry(jewelry) 	IsWeapon(weapon)		IsWoodenWeapon(weapon)		//
// 	IsMetalWeapon(weapon) 	IsBow(weapon) 			IsArmor(armor)			//
//	IsLeatherArmor(armor) 	IsMetalArmor(armor)		IsBoneArmor(armor		//
//	IsShield(shield)	IsMetalShield(shield) 		IsWoodenShield(Shield)		//
//	IsClothing(clothes)	IsMusicalInstrument(item)	IsStaff(staff)			//
//	IsLog(log)		IsPoinableArrows(arrow)		IsReagent(reagent)		//
//	IsOre(ore)		IsIngot(ingot)			IsStackableItem(item)		//
//	IsSteedContainer(cont)	IsBone(bone)			IsAKey(key)			//
//	IsEquippable(item)	IsAGem(gemobjtype)		IsABoard(use_on)		//
//												//
//////////////////////////////////////////////////////////////////////////////////////////////////

use cfgfile;

include "include/objtype";
include "include/client";

var tiles_cfg := ReadConfigFile("::tiles");

// New IsDoor
function IsDoor (byref item)
	if (item.isa (POLCLASS_DOOR))
		return 1;
	endif
	return 0;
endfunction


// Old IsDoor
/*function IsDoor(byref door)

	if (door.objtype >= 0xe8 && door.objtype <= 0xf7)
		return 1;
	elseif (door.objtype >= 0x314 && door.objtype <= 0x365)
		return 1;
	elseif (door.objtype >= 0x675 && door.objtype <= 0x6f4)
		return 1;
	elseif (door.objtype >= 0x824 && door.objtype <= 0x833)
		return 1;
	elseif (door.objtype >= 0x839 && door.objtype <= 0x848)
		return 1;
	elseif (door.objtype >= 0x84c && door.objtype <= 0x85b)
		return 1;
	elseif (door.objtype >= 0x866 && door.objtype <= 0x875)
		return 1;
	elseif (door.objtype >= 0x1fed && door.objtype <= 0x1ffc)
		return 1;
	endif

	return 0;

endfunction
*/
function IsHair (byref hair)

	case (hair.objtype)
		0x203b:	//Short Hair
		0x203c:	//Long Hair
		0x203d:	//Pony Tail
		0x2044:	//MoHawk
		0x2045:	//Page Boy
		0x2046:	//Buns
		0x2047:	//Afro
		0x2048:	//Receding
		0x2049:	//Pigtails
		0x204a:	//Krisna
		0x4258: //Male Horns 1
		0x4259: //Male Horns 2
		0x425a: //Male Horns 3
		0x425b: //Male Horns 4
		0x425c: //Male Horns 5
		0x425d: //Male Horns 6
		0x425e: //Male Horns 7
		0x425f: //Male Horns 8
		0x4261: //Female Horns 1
		0x4262: //Female Horns 2
		0x4273: //Female Horns 3
		0x4274: //Female Horns 4
		0x4275: //Female Horns 5
		0x42aa: //Female Horns 6
		0x42ab: //Female Horns 7
		0x42b1: //Female Horns 8
		return 1;
		default: return 0;
	endcase

endfunction

function IsFacialHair (byref beard)

	case (beard.objtype)
		0x203e:	//Long Beard
		0x203f:	//Short Beard
		0x2040:	//Goatee
		0x2041:	//Mustache
		0x204b:	//Short beard w/mustache
		0x204c:	//Long beard w/mustache
		0x204d:	//Vandyke
		0x42ad:	//Facial Horns 1
		0x42ae:	//Facial Horns 2
		0x42af:	//Facial Horns 3
		0x42b0:	//Facial Horns 4
		return 1;
	endcase
		return 0;
endfunction

function IsJewelry(byref jewelry)

	if (jewelry.objtype >= 0x1085 && jewelry.objtype <= 0x108a)
		return 1;
	elseif (jewelry.objtype >= 0x1f05 && jewelry.objtype <= 0x1f0a)
		return 1;
	endif
		return 0;
endfunction

function IsWeapon (byref weapon)

	if (weapon.graphic >= 0x08fd and weapon.graphic <= 0x090c)
		return 1;
	elseif (weapon.graphic >= 0x0DF0 and weapon.graphic <= 0x0DF5)
		return 1;
	elseif (weapon.graphic >= 0x0E81 and weapon.graphic <= 0x0E8A and weapon.graphic != 0x0E83)
		return 1;
	elseif (weapon.graphic >= 0x0EC2 and weapon.graphic <= 0x0EC5)
		return 1;
	elseif (weapon.graphic >= 0x0F43 and weapon.graphic <= 0x0F52)
		return 1;
	elseif (weapon.graphic >= 0x0F5C and weapon.graphic <= 0x0F63)
		return 1;
	elseif (weapon.graphic >= 0x0FB4 and weapon.graphic <= 0x0FB5)
		return 1;
	elseif (weapon.graphic >= 0x13AF and weapon.graphic <= 0x13BA)
		return 1;
	elseif (weapon.graphic >= 0x13E3 and weapon.graphic <= 0x13E4)
		return 1;
	elseif (weapon.graphic >= 0x13F4 and weapon.graphic <= 0x1407)
		return 1;
	elseif (weapon.graphic >= 0x1438 and weapon.graphic <= 0x1443)
		return 1;
	elseif (weapon.graphic >= 0x26BA and weapon.graphic <= 0x26CF)
		return 1;
	elseif (weapon.graphic >= 0x27A2 and weapon.graphic <= 0x27AF)
		return 1;
	elseif (weapon.graphic >= 0x27ED and weapon.graphic <= 0x27FA)
		return 1;
	elseif (weapon.graphic >= 0x2D1E and weapon.graphic <= 0x2D35)
		return 1;
	elseif (weapon.graphic >= 0x4067 and weapon.graphic <= 0x4076)
		return 1;
	elseif (weapon.graphic >= 0x48ae and weapon.graphic <= 0x48d1)
		return 1;
	endif
		return 0;
endfunction

function IsWoodenWeapon (byref weapon)

	if (weapon.graphic == 0x906)
		return 1;
	elseif (weapon.graphic >= 0x0DF0 and weapon.graphic <= 0x0DF1)
		return 1;
	elseif (weapon.graphic >= 0x0E81 and weapon.graphic <= 0x0E82)
		return 1;
	elseif (weapon.graphic >= 0x0E89 and weapon.graphic <= 0x0E8A)
		return 1;
	elseif (weapon.graphic >= 0x0F4F and weapon.graphic <= 0x0F50)
		return 1;
	elseif (weapon.graphic >= 0x13B1 and weapon.graphic <= 0x13B4)
		return 1;
	elseif (weapon.graphic >= 0x13F4 and weapon.graphic <= 0x13F5)
		return 1;
	elseif (weapon.graphic >= 0x13F8 and weapon.graphic <= 0x13F9)
		return 1;
	elseif (weapon.graphic >= 0x13FC and weapon.graphic <= 0x13FD)
		return 1;
	elseif (weapon.graphic >= 0x26C2 and weapon.graphic <= 0x26C3)
		return 1;
	elseif (weapon.graphic >= 0x26CC and weapon.graphic <= 0x26CD)
		return 1;
	elseif (weapon.graphic >= 0x27A5 and weapon.graphic <= 0x27A6)
		return 1;
	elseif (weapon.graphic == 0x27A8)
		return 1;
	elseif (weapon.graphic == 0x27AA)
		return 1;
	elseif (weapon.graphic == 0x27AE)
		return 1;
	elseif (weapon.graphic >= 0x27F0 and weapon.graphic <= 0x27F1)
		return 1;
	elseif (weapon.graphic == 0x27F3)
		return 1;
	elseif (weapon.graphic == 0x27F5)
		return 1;
	elseif (weapon.graphic == 0x27F9)
		return 1;
	elseif (weapon.graphic >= 0x2D1E and weapon.graphic <= 0x2D1F)
		return 1;
	elseif (weapon.graphic == 0x2D25)
		return 1;
	elseif (weapon.graphic >= 0x2D2A and weapon.graphic <= 0x2D2B)
		return 1;
	elseif (weapon.graphic == 0x2D31)
		return 1;
	elseif (weapon.graphic == 0x406F)
		return 1;
	elseif (weapon.graphic >= 0x48B8	and weapon.graphic <= 0x48B9)
		return 1;
	endif
		return 0;
endfunction

function IsStoneWeapon (byref weapon)
	if (weapon.graphic == 0x900)
		return 1;
	elseif (weapon.graphic == 0x4071)
		return 1;
	endif
	return 0;
endfunction

function IsGlassWeapon (byref weapon)
	if (weapon.graphic == 0x905)
		return 1;
	elseif (weapon.graphic == 0x90c)
		return 1;
	elseif (weapon.graphic == 0x4070)
		return 1;
	elseif (weapon.graphic == 0x4073)
		return 1;
	endif
		return 0;
endfunction

function IsMetalWeapon (byref weapon)

	if (weapon.graphic >= 0x08fd and weapon.graphic <= 0x08ff)
		return 1;
	elseif (weapon.graphic >= 0x0901 and weapon.graphic <= 0x0904)
		return 1;
	elseif (weapon.graphic >= 0x0907 and weapon.graphic <= 0x090b)
		return 1;
	elseif (weapon.graphic >= 0x0DF2 and weapon.graphic <= 0x0DF5)
		return 1;
	elseif (weapon.graphic >= 0x0E85 and weapon.graphic <= 0x0E88)
		return 1;
	elseif (weapon.graphic >= 0x0F43 and weapon.graphic <= 0x0F4E)
		return 1;
	elseif (weapon.graphic >= 0x0F51 and weapon.graphic <= 0x0F52)
		return 1;
	elseif (weapon.graphic >= 0x0F5C and weapon.graphic <= 0x0F63)
		return 1;
	elseif (weapon.graphic >= 0x0FB4 and weapon.graphic <= 0x0FB5)
		return 1;
	elseif (weapon.graphic >= 0x13AF and weapon.graphic <= 0x13B0)
		return 1;
	elseif (weapon.graphic >= 0x13B5 and weapon.graphic <= 0x13BA)
		return 1;
	elseif (weapon.graphic >= 0x13E3 and weapon.graphic <= 0x13E4)
		return 1;
	elseif (weapon.graphic >= 0x13F6 and weapon.graphic <= 0x13F7)
		return 1;
	elseif (weapon.graphic >= 0x13FA and weapon.graphic <= 0x13FB)
		return 1;
	elseif (weapon.graphic >= 0x13FE and weapon.graphic <= 0x1407)
		return 1;
	elseif (weapon.graphic >= 0x1438 and weapon.graphic <= 0x1443)
		return 1;
	elseif (weapon.graphic >= 0x26BA and weapon.graphic <= 0x26C1)
		return 1;
	elseif (weapon.graphic >= 0x26C4 and weapon.graphic <= 0x26CB)
		return 1;
	elseif (weapon.graphic >= 0x26CE and weapon.graphic <= 0x26CF)
		return 1;
	elseif (weapon.graphic >= 0x27a2 and weapon.graphic <= 0x27a4)
		return 1;
	elseif (weapon.graphic == 0x27a7)
		return 1;
	elseif (weapon.graphic == 0x27a9)
		return 1;
	elseif (weapon.graphic >= 0x27ab and weapon.graphic <= 0x27ad)
		return 1;
	elseif (weapon.graphic == 0x27af)
		return 1;
	elseif (weapon.graphic >= 0x27ed and weapon.graphic <= 0x27ef)
		return 1;
	elseif (weapon.graphic == 0x27f2)
		return 1;
	elseif (weapon.graphic == 0x27f4)
		return 1;
	elseif (weapon.graphic >= 0x27f6 and weapon.graphic <= 0x27f8)
		return 1;
	elseif (weapon.graphic == 0x27fa)
		return 1;
	elseif (weapon.graphic >= 0x2d20 and weapon.graphic <= 0x2d24)
		return 1;
	elseif (weapon.graphic >= 0x2d26 and weapon.graphic <= 0x2d29)
		return 1;
	elseif (weapon.graphic >= 0x2d2c and weapon.graphic <= 0x2d30)
		return 1;
	elseif (weapon.graphic >= 0x2d32 and weapon.graphic <= 0x2d35)
		return 1;
	elseif (weapon.graphic >= 0x4067 and weapon.graphic <= 0x406e)
		return 1;
	elseif (weapon.graphic == 0x4072)
		return 1;
	elseif (weapon.graphic >= 0x4074 and weapon.graphic <= 0x4076)
		return 1;
	elseif (weapon.graphic >= 0x48ae and weapon.graphic <= 0x48b7)
		return 1;
	elseif (weapon.graphic >= 0x48ba and weapon.graphic <= 0x48d1)
		return 1;
	endif
		return 0;
endfunction

function IsBow (byref weapon)
	if (weapon.graphic >= 0x0F4F and weapon.graphic  <= 0x0F50)
		return 1;
	elseif (weapon.graphic >= 0x13B1 and weapon.graphic <= 0x13B2)
		return 1;
	elseif (weapon.graphic >= 0x13FC and weapon.graphic <= 0x13FD)
		return 1;
	elseif (weapon.graphic >= 0x26C2 and weapon.graphic <= 0x26C3)
		return 1;
	elseif (weapon.graphic >= 0x26CC and weapon.graphic <= 0x26CD)
		return 1;
	elseif (weapon.graphic >= 0x2D2A and weapon.graphic <= 0x2D2B)
		return 1;
	elseif (weapon.graphic >= 0x2D1E and weapon.graphic <= 0x2D1F)
		return 1;
	elseif (weapon.graphic == 0x2790)
		return 1;
	elseif (weapon.graphic == 0x27A5)
		return 1;
	elseif (weapon.graphic == 0x27AA)
		return 1;
	elseif (weapon.graphic == 0x27DB)
		return 1;
	elseif (weapon.graphic == 0x27F0)
		return 1;
	elseif (weapon.graphic == 0x27F5)
		return 1;
	endif
		return 0;
endfunction

function IsThrowingPouch (byref weapon)
	if (weapon.graphic == 0x27aa)
		return 1;
	elseif (weapon.graphic == 0x27f5)
		return 1;
	endif
	return 0;
endfunction

function IsThrowingWeapon (byref weapon)
	if (weapon.graphic == 0x8ff)
		return 1;
	elseif (weapon.graphic == 0x901)
		return 1;
	elseif (weapon.graphic == 0x90a)
		return 1;
	elseif (weapon.graphic == 0x4067)
		return 1;
	elseif (weapon.graphic >= 0x406b and weapon.graphic <= 0x406c)
		return 1;
	endif
	return 0;
endfunction

function IsArmor (byref armor)

	if (armor.graphic >= 0x283 and armor.graphic <= 0x28a)
		return 1;
	elseif (armor.graphic >= 0x301 and armor.graphic <= 0x30e)
		return 1;
	elseif (armor.graphic >= 0x310 and armor.graphic <= 0x311)
		return 1;
	elseif (armor.graphic >= 0x403 and armor.graphic <= 0x40a)
		return 1;
	elseif (armor.graphic == 0x13BB)
		return 1;
	elseif (armor.graphic >= 0x13BE and armor.graphic <= 0x13C0)
		return 1;
	elseif (armor.graphic >= 0x13C3 and armor.graphic <= 0x13C7)
		return 1;
	elseif (armor.graphic >= 0x13CB and armor.graphic <= 0x13CE)
		return 1;
	elseif (armor.graphic >= 0x13D2 and armor.graphic <= 0x13D6)
		return 1;
	elseif (armor.graphic >= 0x13DA and armor.graphic <= 0x13DD)
		return 1;
	elseif (armor.graphic >= 0x13E1 and armor.graphic <= 0x13E2)
		return 1;
	elseif (armor.graphic >= 0x13EB and armor.graphic <= 0x13F2)
		return 1;
	elseif (armor.graphic >= 0x1408 and armor.graphic <= 0x141A)
		return 1;
	elseif (armor.graphic >= 0x144E and armor.graphic <= 0x1457)
		return 1;
	elseif (armor.graphic >= 0x1C00 and armor.graphic <= 0x1C0D)
		return 1;
	elseif (armor.graphic >= 0x1F0B and armor.graphic <= 0x1F0C)
		return 1;
	elseif (armor.graphic >= 0x2641 and armor.graphic <= 0x2648)
		return 1;
	elseif (armor.graphic >= 0x264b and armor.graphic <= 0x264c)
		return 1;
	elseif (armor.graphic >= 0x2657 and armor.graphic <= 0x2658)
		return 1;
	elseif (armor.graphic >= 0x2689 and armor.graphic <= 0x268a)
		return 1;
	elseif (armor.graphic >= 0x2774 and armor.graphic <= 0x2781)
		return 1;
	elseif (armor.graphic >= 0x2784 and armor.graphic <= 0x2794)
		return 1;
	elseif (armor.graphic == 0x279d)
		return 1;
	elseif (armor.graphic >= 0x27bf and armor.graphic <= 0x27cc)
		return 1;
	elseif (armor.graphic >= 0x27cf and armor.graphic <= 0x27df)
		return 1;
	elseif (armor.graphic == 0x27e8)
		return 1;
	elseif (armor.graphic >= 0x2b67 and armor.graphic <= 0x2b79)
		return 1;
	elseif (armor.graphic == 0x2fb8)
		return 1;
	elseif (armor.graphic >= 0x2fc5 and armor.graphic <= 0x2fcb)
		return 1;
	elseif (armor.graphic >= 0x315e and armor.graphic <= 0x3170)
		return 1;
	elseif (armor.graphic >= 0x317b and armor.graphic <= 0x3181)
		return 1;
	elseif (armor.graphic >= 0x4047 and armor.graphic <= 0x4066)
		return 1;
	elseif (armor.graphic >= 0x457e and armor.graphic <= 0x457f)
		return 1;
	elseif (armor.graphic >= 0x45a4 and armor.graphic <= 0x45a5)
		return 1;
	endif
		return 0;
endfunction

function IsLeatherArmor (byref armor)

	if (armor.graphic >= 0x301 and armor.graphic <= 0x306)
		return 1;
	elseif (armor.graphic >= 0x310 and armor.graphic <= 0x311)
		return 1;
	elseif (armor.graphic >= 0x13c5 and armor.graphic <= 0x13c7)
		return 1;
	elseif (armor.graphic >= 0x13CB and armor.graphic <= 0x13CE)
		return 1;
	elseif (armor.graphic >= 0x13D2 and armor.graphic <= 0x13D6)
		return 1;
	elseif (armor.graphic >= 0x13DA and armor.graphic <= 0x13DD)
		return 1;
	elseif (armor.graphic >= 0x13E1 and armor.graphic <= 0x13E2)
		return 1;
	elseif (armor.graphic >= 0x144E and armor.graphic <= 0x1457)
		return 1;
	elseif (armor.graphic >= 0x1C00 and armor.graphic <= 0x1C03)
		return 1;
	elseif (armor.graphic >= 0x1C06 and armor.graphic <= 0x1C0D)
		return 1;
	elseif (armor.graphic == 0x1DB9 or armor.graphic == 0x1DBA)
		return 1;
	elseif (armor.graphic == 0x2776)
		return 1;
	elseif (armor.graphic == 0x2778)
		return 1;
	elseif (armor.graphic >= 0x277a and armor.graphic <= 0x277c)
		return 1;
	elseif (armor.graphic >= 0x277e and armor.graphic <= 0x277f)
		return 1;
	elseif (armor.graphic == 0x2784)
		return 1;
	elseif (armor.graphic >= 0x2786 and armor.graphic <= 0x2787)
		return 1;
	elseif (armor.graphic >= 0x2789 and armor.graphic <= 0x278b)
		return 1;
	elseif (armor.graphic == 0x278e)
		return 1;
	elseif (armor.graphic >= 0x2790 and armor.graphic <= 0x2793)
		return 1;
	elseif (armor.graphic == 0x279d)
		return 1;
	elseif (armor.graphic == 0x27c1)
		return 1;
	elseif (armor.graphic == 0x27c3)
		return 1;
	elseif (armor.graphic >= 0x27c5 and armor.graphic <= 0x27c7)
		return 1;
	elseif (armor.graphic >= 0x27c9 and armor.graphic <= 0x27ca)
		return 1;
	elseif (armor.graphic == 0x27cf)
		return 1;
	elseif (armor.graphic >= 0x27d1 and armor.graphic <= 0x27d2)
		return 1;
	elseif (armor.graphic >= 0x27d4 and armor.graphic <= 0x27d6)
		return 1;
	elseif (armor.graphic == 0x27d9)
		return 1;
	elseif (armor.graphic >= 0x27db and armor.graphic <= 0x27de)
		return 1;
	elseif (armor.graphic == 0x27e8)
		return 1;
	elseif (armor.graphic >= 0x2b74 and armor.graphic <= 0x2b79)
		return 1;
	elseif (armor.graphic >= 0x2fc5 and armor.graphic <= 0x2fcb)
		return 1;
	elseif (armor.graphic == 0x315f)
		return 1;
	elseif (armor.graphic >= 0x316b and armor.graphic <= 0x3170)
		return 1;
	elseif (armor.graphic >= 0x317b and armor.graphic <= 0x3181)
		return 1;
	elseif (armor.graphic >= 0x4047 and armor.graphic <= 0x404e)
		return 1;
	elseif (armor.graphic >= 0x457e and armor.graphic <= 0x457f)
		return 1;
	endif
	return 0;
endfunction

function IsMetalArmor (byref armor)
	if (armor.graphic >= 0x307 and armor.graphic <= 0x30e)
		return 1;
	elseif (armor.graphic == 0x13BB)
		return 1;
	elseif (armor.graphic >= 0x13BE and armor.graphic <= 0x13C0)
		return 1;
	elseif (armor.graphic >= 0x13C3 and armor.graphic <= 0x13C4)
		return 1;
	elseif (armor.graphic >= 0x13EB and armor.graphic <= 0x13F2)
		return 1;
	elseif (armor.graphic >= 0x1408 and armor.graphic <= 0x141A)
		return 1;
	elseif (armor.graphic >= 0x1c04 and armor.graphic <= 0x1c05)
		return 1;
	elseif (armor.objtype == UOBJ_CHAINMAIL_GLOVES and armor.graphic == 0x13c6)
		return 1;
	elseif (armor.graphic >= 0x2641 and armor.graphic <= 0x2648)
		return 1;
	elseif (armor.graphic >= 0x2657 and armor.graphic <= 0x2658)
		return 1;
	elseif (armor.graphic >= 0x2689 and armor.graphic <= 0x268a)
		return 1;
	elseif (armor.graphic >= 0x2774 and armor.graphic <= 0x2775)
		return 1;
	elseif (armor.graphic == 0x2777)
		return 1;
	elseif (armor.graphic == 0x2779)
		return 1;
	elseif (armor.graphic == 0x277d)
		return 1;
	elseif (armor.graphic >= 0x2780 and armor.graphic <= 0x2781)
		return 1;
	elseif (armor.graphic == 0x2785)
		return 1;
	elseif (armor.graphic == 0x2788)
		return 1;
	elseif (armor.graphic == 0x278d)
		return 1;
	elseif (armor.graphic >= 0x27bf and armor.graphic <= 0x27c0)
		return 1;
	elseif (armor.graphic == 0x27c2)
		return 1;
	elseif (armor.graphic == 0x27c4)
		return 1;
	elseif (armor.graphic == 0x27c8)
		return 1;
	elseif (armor.graphic >= 0x27cb and armor.graphic <= 0x27cc)
		return 1;
	elseif (armor.graphic == 0x27d0)
		return 1;
	elseif (armor.graphic == 0x27d3)
		return 1;
	elseif (armor.graphic == 0x27d8)
		return 1;
	elseif (armor.graphic >= 0x404f and armor.graphic <= 0x4056)
		return 1;
	endif
	return 0;
endfunction

function IsHideArmor (byref armor)
	if (armor.graphic >= 0x2b74 and armor.graphic <= 0x2b79)
		return 1;
	elseif (armor.graphic >= 0x316b and armor.graphic <= 0x3170)
		return 1;
	endif
	return 0;
endfunction

function IsWoodenArmor (byref armor)
	if (armor.graphic >= 0x2b67 and armor.graphic <= 0x2b6d)
		return 1;
	elseif (armor.graphic >= 0x2b71 and armor.graphic <= 0x2b73)
		return 1;
	elseif (armor.graphic == 0x315e)
		return 1;
	elseif (armor.graphic >= 0x3160 and armor.graphic <= 0x3164)
		return 1;
	elseif (armor.graphic >= 0x3168 and armor.graphic <= 0x316a)
		return 1;
	endif
	return 0;
endfunction

function IsBoneArmour(byref armor)

	if (armor.objtype >= 0x144e && armor.objtype <= 0x1452)
		return 1;
        endif
	return 0;

endfunction


function IsClothArmor (byref armor)
	if (armor.graphic >= 0x403 and armor.graphic <= 0x40a)
		return 1;
	elseif (armor.graphic >= 0x405f and armor.graphic <= 0x4064)
		return 1;
	endif
	return 0;
endfunction

function IsStoneArmor (byref armor)
	if (armor.graphic >= 0x283 and armor.graphic <= 0x28a)
		return 1;
	elseif (armor.graphic >= 0x4057 and armor.graphic <= 0x405e)
		return 1;
	endif
	return 0;
endfunction

function IsStoneShield (byref shield)
	case (shield.graphic)
		0x4205:
		0x420B:
			return 1;
	endcase
	return 0;
endfunction

function IsShield(byref shield)

	if (shield.objtype >= 0x1b72 && shield.objtype <= 0x1b7b)
		return 1;
	elseif (shield.objtype >= 0x1bc3 && shield.objtype <= 0x1bc5)
		return 1;
	endif
		return 0;
endfunction

function IsMetalShield(byref shield)

	if (shield.objtype >= 0x1b72 && shield.objtype <= 0x1b77)
		return 1;
	elseif (shield.objtype == 0x1b7b)
		return 1;
	elseif (shield.objtype >= 0x1bc3 && shield.objtype <= 0x1bc5)
		return 1;
	endif
		return 0;
endfunction

function IsWoodenShield(byref shield)

	if (shield.objtype  >= 0x1b78 && shield.objtype <= 0x1b7a)
		return 1;
	endif
		return 0;
endfunction

function IsClothing(byref clothes)

	if (clothes.objtype >= 0x1515 && clothes.objtype <= 0x1518)
 		return 1;
	elseif (clothes.objtype >= 0x152e && clothes.objtype <= 0x1531)
 		return 1;
	elseif (clothes.objtype >= 0x1537 && clothes.objtype <= 0x1544)
		return 1;
	elseif (clothes.objtype >= 0x1708 && clothes.objtype <= 0x171c)
		return 1;
	elseif (clothes.objtype >= 0x1efd && clothes.objtype <= 0x1f07)
		return 1;
	elseif (clothes.objtype >= 0x1f7b && clothes.objtype <= 0x1f7c)
		return 1;
	elseif (clothes.objtype >= 0x1f9f && clothes.objtype <= 0x1fa2)
		return 1;
	elseif (clothes.objtype >= 0x1ffd && clothes.objtype <= 0x1ffe)
		return 1;
	elseif (clothes.objtype >= 0x204e && clothes.objtype <= 0x204f)
		return 1;
	elseif (clothes.graphic >= 0x2305 and clothes.graphic <= 0x2310)
		return 1;
	endif
	case (clothes.graphic)
		0x2782:
		0x2783:
		0x278f:
		0x2794:
		0x2796:
		0x2797:
		0x2798:
		0x2799:
		0x279a:
		0x279b:
		0x279c:
		0x27a0:
		0x27a1:
		0x27cd:
		0x27ce:
		0x27da:
		0x27df:
		0x27e1:
		0x27e2:
		0x27e3:
		0x27e4:
		0x27e5:
		0x27e6:
		0x27e7:
		0x27eb:
		0x27ec:
		0x2fb9:
		0x2fba:
		0x2fc3:
		0x2fc4:
		0x3173:
		0x3174:
		0x3175:
		0x3176:
		0x3177:
		0x3178:
		0x3179:
		0x317a:
		0x4000:
		0x4001:
		0x4002:
		0x4003:
		0x405f:
		0x4060:
		0x4061:
		0x4062:
		0x4063:
		0x4064:
		0x4065:
		0x4066:
		0x41d8:
		0x41d9:
		0x42de:
		0x42df:
		0x450d:
		0x450e:
		0x457e:
		0x457f:
		0x45a4:
		0x45a5:
		0x45b1:
		0x45b3:
		0x4644:
		0x4645:
		0x46aa:
		0x46ab:
		0x46b4:
		0x46b5:
		0x50d8:
		0x50d9:
			return 1;
	endcase
	return 0;
endfunction

///////////////////
//  Determines if the item passed is a hat
///////////////////

function IsAHat (item)
	case (item.graphic)
		0x153F:
		0x1543:
		0x1713:
		0x1714:
		0x1715:
		0x1716:
		0x1717:
		0x1718:
		0x1719:
		0x171A:
		0x171B:
		0x171C:
		UOBJ_ORC_MASK:
		UOBJ_ORC_MASK2:
		UOBJ_BEAR_MASK:
		UOBJ_BEAR_MASK2:
		UOBJ_DEER_MASK:
		UOBJ_DEER_MASK2:
		UOBJ_TRIBAL_MASK:
		UOBJ_TRIBAL_MASK2:
		UOBJ_TRIBAL_MASK3:
		UOBJ_TRIBAL_MASK4:
		0x278f:
		0x2798:
		0x27da:
		0x27e3:
			return 1;
	endcase

	return 0;
endfunction




///////////////////
//  Determines if the item passed is a mask
///////////////////

function IsAMask (item)
	case (item.graphic)
		UOBJ_ORC_MASK:
		UOBJ_ORC_MASK2:
		UOBJ_BEAR_MASK:
		UOBJ_BEAR_MASK2:
		UOBJ_DEER_MASK:
		UOBJ_DEER_MASK2:
		UOBJ_TRIBAL_MASK:
		UOBJ_TRIBAL_MASK2:
		UOBJ_TRIBAL_MASK3:
		UOBJ_TRIBAL_MASK4:
			return 1;
	endcase

	return 0;
endfunction

function IsMusicalInstrument(byref item)

	case(item.objtype)
		0x27067:    //FLUTE
		0x0e9d:    //TAMBORINE
		0x0e9e:    //TAMBORINE2
		0x0eb3:    //LUTE
		0x0eb4:    //LUTE2
		0x0e9c:    //DRUM
		0x0eb2:    //HARP
		0x0eb1:    //STANDING HARP
		return 1;
		default: return 0;
	endcase
endfunction

function IsStaff(byref item)

	case(item.objtype)
		0x13f9:          //club
                0x0e81:          //shepherd
                0x0e8a:          //Quater
                0x13f9:          //gnarled
                0x27065:          //mage
                0x0df0:          //black
                0x2706a:          //lich/nerco
		0x26bf:		// Double Bladed
		0x26bd:		// Bladed
		0x26ba:		// Scythe (grim reaper's staff)
		return 1;
		default: return 0;
	endcase
endfunction

function IsLog(byref item)

	if (item.objtype >= 0x26351 && item.objtype <= 0x26367)
		return 1;
        elseif(item.objtype == 0x1BDD)
	        return 1;
        else
                return 0;
	endif
endfunction

function IsPoisonAbleArrows(byref item)

	case(item.objtype)
		0x0f3f:    //Arrows
                0x1bfb:    //Crossbow bolts
		return 1;
		default: return 0;
	endcase
endfunction

///////////////////////////Needs work////////////////////////
/////////////////////////////////////////////////////////////
//////////////////////Finish me NOW!/////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
function IsReagent(byref item)

	case(item.objtype)
////////////////// 8 Original Reagents ///////////////
		0x0f7a:	//BLACK PEARL
		0x0f7b:	//BLOOD MOSS
		0x0f84:	//GARLIC
		0x0f85:	//GINSENG
		0x0f86:	//MANDRAKE ROOT
		0x0f88:	//NIGHTSHADE
		0x0f8c:	//SULFUROUS ASH
		0x0f8d:	//SPIDER SILK

//////////////////// Pagan Reagents //////////////////
		0x0f78:	//BATWING
		0x0f79:	//BLACKMOOR
		0x0f7c:	//BLOODSPAWN
		0x0f7e:	//BONE
		0x0f7f:	//BRIMSTONE
		0x0f80:	//DAEMON BONE
		0x0f90:	//DEAD WOOD
		0x0f82:	//DRAGONS BLOOD
		0x0f83:	//EXECUTIONERS CAP
		0x0f81:	//FERTILE DIRT
		0x0f8e:	//SERPENT SCALE
		0x0f7d:	//VIAL OF BLOOD
		0x0f8f:	//VOLCANIC ASH
		0x0f91:	//WYRMS HEART
              return 1;
		default: return 0;
	endcase
endfunction

///////////////////
//  Figures out if the item is a type of gem
///////////////////

function IsGem (byref use_on)
	case (use_on)
		UOBJ_GEM_AMBER:		return 1;
		UOBJ_GEM_STAR_SAPPHIRE:	return 1;
		UOBJ_GEM_EMERALD:		return 1;
		UOBJ_GEM_SAPPHIRE:		return 1;
		UOBJ_GEM_RUBY:			return 1;
		UOBJ_GEM_CITRINE:		return 1;
		UOBJ_GEM_AMETHYST:		return 1;
		UOBJ_GEM_TOURMALINE:	return 1;
		UOBJ_GEM_DIAMOND:		return 1;
		default:				return 0;
	endcase;
	return 0;
endfunction




///////////////////
//  Determines if the item passed is a type of metal ore
///////////////////

function IsOre(byref use_on)
  if ((use_on.objtype >= 0x26300 && use_on.objtype <= 0x26308 ||
  use_on.objtype >= 0x26205 && use_on.objtype <= 0x26213))
    return 1;
  else
    return 0;
  endif
endfunction

///////////////////
//  Determines if the item passed is a type of metal ingot
///////////////////

function IsIngot(use_on)
  if ((use_on.objtype >= 0x26309 && use_on.objtype <= 0x26312 ||
  use_on.objtype >= 0x26245 && use_on.objtype <= 0x26254 || use_on.objtype == 0x26216))
    return 1;
  else
    return 0;
  endif
endfunction


///////////////////
//  Determines if the integer passed matches the objtype number of ingots.
//  Needed this for resmelting.
///////////////////

function IsIngotObjtype(use_on)
  if ((use_on >= 0x26309 && use_on <= 0x26312 ||
  use_on >= 0x26245 && use_on <= 0x26254 || use_on == 0x26216))
    return 1;
  else
    return 0;
  endif
endfunction




///////////////////
//  determines if the item can stack with other items that the character already has
///////////////////

function IsStackableItem(byref item)
	if (item.amount > 1)
		return 1;
	endif

        if (IsIngot(item))
        		return 1;
	endif

        if (IsReagent(item))
        		return 1;
	endif

        if (IsOre(item))
        		return 1;
	endif

	case (item.objtype)
		0x25001:			// Snow
		0x2EE18:			// Sand
		0x2EE19:			// Clay
		0x2EE20:			// Glass
		0x2EE21:			// Porcelain
		UOBJ_GOLD_COIN:
		UOBJ_BATWING:
		UOBJ_BLACKMOOR:
		UOBJ_BLACK_PEARL:
		UOBJ_BLOOD_MOSS:
		UOBJ_BLOODSPAWN:
		UOBJ_VIAL_OF_BLOOD:
		UOBJ_BONE:
		UOBJ_BRIMSTONE:
		UOBJ_DAEMON_BONE:
		UOBJ_FERTILE_DIRT:
		UOBJ_DRAGONS_BLOOD:
		UOBJ_EXECUTIONERS_CAP:
		UOBJ_GARLIC:
		UOBJ_GINSENG:
		UOBJ_MANDRAKE_ROOT:
		UOBJ_EYE_OF_NEWT:
		UOBJ_NIGHTSHADE:
		UOBJ_OBSIDIAN:
		UOBJ_PIG_IRON:
		UOBJ_PUMICE:
		UOBJ_SULFUROUS_ASH:
		UOBJ_SPIDER_SILK:
		UOBJ_SERPENT_SCALE:
		UOBJ_VOLCANIC_ASH:
		UOBJ_DEAD_WOOD:
		UOBJ_WYRMS_HEART:
		UOBJ_GEM_AMBER:
		UOBJ_GEM_STAR_SAPPHIRE:
		UOBJ_GEM_EMERALD:
		UOBJ_GEM_SAPPHIRE:
		UOBJ_GEM_RUBY:
		UOBJ_GEM_CITRINE:
		UOBJ_GEM_AMETHYST:
		UOBJ_GEM_TOURMALINE:
		UOBJ_GEM_DIAMOND:
		UOBJ_FEATHER:
		UOBJ_EGG:
    UOBJ_HIDE:
    UOBJ_BROWN_HIDE:
    UOBJ_RED_HIDE:
    UOBJ_BLUE_HIDE:
    UOBJ_GREEN_HIDE:
		UOBJ_SILVER_HIDE:
		UOBJ_BANDAGE:
		UOBJ_ARROW:
		UOBJ_BOLT:
		UOBJ_FIRE_ARROW:
		UOBJ_REAPER_HEARTWOOD:
		UOBJ_ENT_HEARTWOOD:
		UOBJ_ENCHANTED_BONE:
    UOBJ_RAW_RIB:
    UOBJ_RAW_FISHSTEAK:
    UOBJ_RAW_BIRD:
    UOBJ_RAW_HAM:
    UOBJ_BACON_SLAB:
    UOBJ_RAW_SAUSAGE:
    UOBJ_HONEY:
    UOBJ_CARROT:
    UOBJ_APPLE:
    UOBJ_PEACH:
    UOBJ_LEMON:
    UOBJ_LIME:
    UOBJ_BANANA:
    UOBJ_PEAR:
    UOBJ_GRAPES:
    UOBJ_DATES:
    UOBJ_PUMPKIN:
    UOBJ_LETTUCE:
    UOBJ_CABBAGE:
    UOBJ_ONION:
    UOBJ_TURNIP:
    UOBJ_COCONUT:
    UOBJ_RAW_CHICKEN_LEG:
    UOBJ_RAW_LEG_OF_LAMB:
                            return 1;
	endcase

	//Already went through the common stackable items and still haven't been able to figure out if its stackable or not, so
	//we'll see if we can create 2 of the item as a last resort.
	set_critical(1);
	var newitem := CreateItemAtLocation(0, 0, 0, item.objtype, 2, "britannia_alt");
	if (newitem.amount == 2)
		DestroyItem(newitem);
		set_critical(0);
		return 1;
	endif
	set_critical(0);
	return 0;
endfunction

function IsSteedContainer(item)

	if (item.objtype == 0x0efa)                      //Spellbook
		return 1;
	elseif (item.objtype == 0x0e40)                  //MetalChest
		return 1;
	elseif (item.objtype >= 0x0e73 && item.objtype <= 0x0e76)    //Backpack-packhorseBackpack-GMBackpack-Bag
		return 1;
	elseif (item.objtype >= 0x0a2d && item.objtype <= 0x0a53)    //Furniture
		return 1;
	elseif (item.objtype >= 0x1e80 && item.objtype <= 0x1e81)    //Furniture
		return 1;
	elseif (item.objtype == 0x0e79)                  //Pouch
		return 1;
	elseif (item.objtype == 0x0e7c)                  //SilverChest
		return 1;
	elseif (item.objtype == 0x0e80)                  //BrassBox
		return 1;
	elseif (item.objtype == 0x09a8)                  //BrassBox1
		return 1;
	elseif (item.objtype == 0x09ab)                  //SilverChest1
		return 1;
	elseif (item.objtype == 0x09b0)                  //Pouch1
		return 1;
	elseif (item.objtype == 0x0e41)                  //MetalChest1
		return 1;
	elseif (item.objtype == 0x0e7a)                  //PicnicBasket
		return 1;
	elseif (item.objtype == 0x0e83)                  //EmptyTub
		return 1;
	elseif (item.objtype == 0x0fae)                  //Barrel1
		return 1;
	elseif (item.objtype == 0x09ac)                  //LargeBasket
		return 1;
	elseif (item.objtype == 0x09b1)                  //SmallBasket
		return 1;
	elseif (item.objtype == 0x26100)                  //Runebook
		return 1;
	elseif (item.objtype == 0x09aa)                  //WoodenBox
		return 1;
	elseif (item.objtype == 0x0e7d)                  //WoodenBox2
		return 1;
	elseif (item.objtype == 0x0e42)                  //WoodenChest
		return 1;
	elseif (item.objtype == 0x0e43)                  //WoodenChest2
		return 1;
	elseif (item.objtype == 0x09a9)                  //SmallCrate
		return 1;
	elseif (item.objtype == 0x0e7e)                  //SmallCrate2
		return 1;
	elseif (item.objtype == 0x0e3f)                  //MediumCrate
		return 1;
	elseif (item.objtype == 0x0e3e)                  //MediumCrate2
		return 1;
	elseif (item.objtype == 0x0e3c)                  //LargeCrate
		return 1;
	elseif (item.objtype == 0x0e3d)                  //LargeCrate2
		return 1;
	elseif (item.objtype == 0x0e77)                  //Barrel
		return 1;
	elseif (item.objtype == 0x0e7f)                  //Keg
		return 1;
	elseif (item.objtype >= 0x0a97 && item.objtype <= 0x0a9e)    //EmptyBookShelf
		return 1;
        endif

	return 0;

endfunction

function IsBone(byref item)

/*    if (item.objtype == 0x0f7e or item.objtype == 0x0f80 or item.objtype == 0xa153 or (item.objtype >= 0x0ed3 && item.objtype <= 0x0ed7))
        return 1;
    else
        return 0;
    endif*/
	case(item.objtype)
		0x0f7e:	//BONE
		0x0f80:	//DAEMON
    0xa153: //ENCHANTED BONES
    0x0ed3: //liche
    0x0ed4: //scortched
    0x0ed5: //frostbitten
    0x0ed6: //drake
    0x0ed7: //dragon
            return 1;
		default: return 0;
	endcase
endfunction

function IsAKey (byref item)
    case (item.objtype)
        0x100E:
        0x100F:
        0x1010:
        0x1012:
        0x1013:
                return 1;
    endcase
    return 0;
endfunction


function IsEquippable(byref item)
	var elem;
	var item_number := item.objtype;
	item_number := CInt(item_number);
	elem:=FindConfigElem(tiles_cfg,item_number);
	if(GetConfigString(elem,"Equippable"))
	    return 1;
	else
	    return 0;
	endif
endfunction

///////////////////////////////////////////////
//  Determines if the item is a standard gem
///////////////////////////////////////////////

function IsAGem(gemobjtype)

	case (gemobjtype)
		UOBJ_GEM_DIAMOND:
		UOBJ_GEM_SAPPHIRE:
		UOBJ_GEM_STAR_SAPPHIRE:
		UOBJ_GEM_EMERALD:
		UOBJ_GEM_RUBY:
		UOBJ_GEM_AMETHYST:
		UOBJ_GEM_CITRINE:
		UOBJ_GEM_TOURMALINE:
		UOBJ_GEM_AMBER:
                    return 1;
		default:
			return 0;
	endcase

endfunction

////////////////////////////////////////
// Returns a 1 if the item is a board //
////////////////////////////////////////

function IsABoard(byref use_on)
    if (use_on.objtype == 0x1bd7)
          return 1;
    endif
    if ((use_on.objtype >= 0x26370 && use_on.objtype <= 0x26386))
          return 1;
    else
          return 0;
    endif
endfunction

function IsSign (item)
	case (item.objtype)
		0xb95:
		0xb96:
		0xbd0:
		0xbd2:
		0xc43:
		0xc44:
		0x1f28:
		0x1f29:
		0x1600E:
		0x1600F:
		0x16010:
		0x16011:
		0x17060:
		0x1708a:	return 1;
	endcase
	if (item.objtype >= 0xba3 and item.objtype <= 0xc0e)
		return 1;
	elseif (item.objtype >= 0x1297 and item.objtype <= 0x129e)
		return 1;
	endif
	return 0;
endfunction


//////////////////////////////////////////////////////////////////
//                                                              //
//  Returns the layer for an equippable item after first        //
//  determining if it's equippable.                             //
//                                                              //
//  Returns an integer value equal to the laqyer assigned to    //
//  or 0 if the item is not equippable or has no layer          //
//  assigned.                                                   //
//                                                              //
//////////////////////////////////////////////////////////////////

function GetLayer(object)

	var tile_config_file := ReadConfigFile("::tiles");
	var thing := FindConfigElem( tile_config_file, object.objtype );
	var layr := GetConfigInt( thing, "Layer" );
  return CInt(layr);

endfunction

function IsGargishItem(byref item)
	if(item.graphic >= 0x0283 and item.graphic <= 0x028a)
		return 1;
	elseif(item.graphic >= 0x0301 and item.graphic <= 0x0311)
		return 1;
	elseif(item.graphic >= 0x08fd and item.graphic <= 0x090c)
		return 1;
	elseif(item.graphic >= 0x4000 and item.graphic <= 0x4003)
		return 1;
	elseif(item.graphic >= 0x4047 and item.graphic <= 0x4076)
		return 1;
	elseif(item.graphic >= 0x41d8 and item.graphic <= 0x41d9)
		return 1;
	elseif(item.graphic >= 0x4200 and item.graphic <= 0x420b)
		return 1;
	elseif(item.graphic >= 0x4210 and item.graphic <= 0x4213)
		return 1;
	elseif(item.graphic >= 0x4228 and item.graphic <= 0x422c)
		return 1;
	elseif(item.graphic >= 0x42de and item.graphic <= 0x42df)
		return 1;
	elseif(item.graphic >= 0x450d and item.graphic <= 0x450e)
		return 1;
	elseif(item.graphic >= 0x457e and item.graphic <= 0x457f)
		return 1;
	elseif(item.graphic >= 0x45a4 and item.graphic <= 0x45a5)
		return 1;
	elseif(item.graphic == 0x45b1)
		return 1;
	elseif(item.graphic == 0x45b3)
		return 1;
	elseif(item.graphic >= 0x4644 and item.graphic <= 0x4645)
		return 1;
	elseif(item.graphic >= 0x46aa and item.graphic <= 0x46ab)
		return 1;
	elseif(item.graphic >= 0x46b4 and item.graphic <= 0x46b5)
		return 1;
	elseif(item.graphic >= 0x48ae and item.graphic <= 0x48d1)
		return 1;
	elseif(item.graphic >= 0x50d8 and item.graphic <= 0x50d9)
		return 1;
	endif
	return 0;
endfunction

function IsMedableArmor (byref armor)
	var cfg := ReadConfigFile (":*:itemdesc");
	if ((cfg[armor.objtype].blockcircle) && (cfg[armor.objtype].blockcircle < 6))
		return 0;
	endif
	return 1;
endfunction

function IsForAnyRace (byref item)
	if (IsJewelry(item))
		return 1;
	elseif (IsWand(item))
		return 1;
	elseif (item.graphic >= 0x1f03 and item.graphic <= 0x1f04)
		return 1;
	elseif (item.graphic >= 0x0dbf and item.graphic <= 0x0dc0)
		return 1;
	elseif (item.graphic >= 0x13e3 and item.graphic <= 0x13e4)
		return 1;
	elseif (item.graphic >= 0xe81 and item.graphic <= 0xe82)
		return 1;
	elseif (item.graphic >= 0xe85 and item.graphic <= 0xe86)
		return 1;
	elseif (item.graphic >= 0xfb4 and item.graphic <= 0xfb5)
		return 1;
	endif
	return 0;
endfunction

function IsWand(byref item)
	if(item.graphic >= 0x0df2 and item.graphic <= 0x0df5)
		return 1;
	endif
	return 0;
endfunction

function IsOfAppropriateRace(byref who, byref item)
	if (who.npctemplate)
		return 1;
	elseif (who.race == RACE_GARGOYLE)
		if (IsGargishItem(item))
			return 1;
		elseif (IsJewelry(item))
			return 1;
		elseif (IsWand(item))
			return 1;
		elseif (item.graphic >= 0x1f03 and item.graphic <= 0x1f04)
			return 1;
		elseif (item.graphic >= 0x0dbf and item.graphic <= 0x0dc0)
			return 1;
		elseif (item.graphic >= 0x13e3 and item.graphic <= 0x13e4)
			return 1;
		elseif (item.graphic >= 0xe81 and item.graphic <= 0xe82)
			return 1;
		elseif (item.graphic >= 0xe85 and item.graphic <= 0xe86)
			return 1;
		elseif (item.graphic >= 0xfb4 and item.graphic <= 0xfb5)
			return 1;
		endif
		return 0;
	elseif (!IsGargishItem(item))
		return 1;
	endif
	return 0;
endfunction

function IsLeatherBoots (byref item)
	if (item.graphic >= 0x170b and item.graphic <= 0x1712)
		return 1;
	elseif (item.graphic >= 0x2307 and item.graphic <= 0x2308)
		return 1;
	elseif (item.graphic == 0x2fc4)
		return 1;
	elseif (item.graphic == 0x317a)
		return 1;
	endif
	return 0;
endfunction

function IsOneHandedWeapon (byref item)
	if (!item.isa (POLCLASS_WEAPON))
		return 0;
	endif
	var weaponcfgfile := ReadConfigFile (":*:itemdesc");
	var weapon_elem := FindConfigElem (weaponcfgfile, item.objtype);
	if (weapon_elem)
		if (weapon_elem.TwoHanded)
			return 0;
		else
			return 1;
		endif
	endif
	return 0;
endfunction

function IsTwoHandedWeapon (byref item)
	if (!item.isa (POLCLASS_WEAPON))
		return 0;
	endif
	var weaponcfgfile := ReadConfigFile (":*:itemdesc");
	var weapon_elem := FindConfigElem (weaponcfgfile, item.objtype);
	if (weapon_elem)
		if (weapon_elem.TwoHanded)
			return 1;
		endif
	endif
	return 0;
endfunction

function GetLayers()

enum LAYERS
 LAYER_NONE        :=  0x0,   // Not used?
 LAYER_HAND1       :=  0x1,   // One handed weapon.
 LAYER_HAND2       :=  0x2,   // Two handed weapon, shield or misc.
 LAYER_SHOES       :=  0x3,   // Shoes.
 LAYER_PANTS       :=  0x4,   // Pants.
 LAYER_SHIRT       :=  0x5,   // Shirt.
 LAYER_HELM        :=  0x6,   // Helm or Hat.
 LAYER_GLOVES      :=  0x7,   // Gloves.
 LAYER_RING        :=  0x8,   // Ring.
 LAYER_UNUSED9     :=  0x9,   // Unused.
 LAYER_NECK        :=  0xA,   // Neck.
 LAYER_HAIR        :=  0xB,   // Hair
 LAYER_WAIST       :=  0xC,   // Waist (half apron).
 LAYER_CHEST       :=  0xD,   // Torso (inner) (chest armor).
 LAYER_WRIST       :=  0xE,   // Bracelet.
 LAYER_PACK2       :=  0xF,   // Unused (backpack, but ord. bp is 0x15).
 LAYER_BEARD       :=  0x10,   // Facial hair.
 LAYER_TUNIC       :=  0x11,   // Torso (middle) (tunic, sash etc.).
 LAYER_EARS        :=  0x12,   // Earrings.
 LAYER_ARMS        :=  0x13,   // Arms.
 LAYER_CAPE        :=  0x14,   // Back (cloak).
 LAYER_PACK        :=  0x15,   // Backpack
 LAYER_ROBE        :=  0x16,   // Torso (outer) (robe)
 LAYER_SKIRT       :=  0x17,   // Legs (outer) (skirt/robe).
 LAYER_LEGS        :=  0x18,   // Legs (inner) (leg armor).
 LAYER_MOUNT       :=  0x19,   // Mount (horse, ostard etc.).
 LAYER_VENDORSTOCK :=  0x1A,   // This vendor will sell and restock.
 LAYER_VENDOREXTRA :=  0x1B,   // This vendor will resell to players but not restock.
 LAYER_VENDORBUY   :=  0x1C,   // This vendor can buy from players but does not stock.
 LAYER_BANKBOX     :=  0x1D,   // Contents of bankbox
 LAYER_TRADE       :=  0x1E    // Can be multiple of these, do not use directly.
endenum

return LAYERS;

endfunction

//
//ArmorZone
//    Name    Body
//    Chance  44
//	ProtCoverage	3
//
//ArmorZone
//    Name    Arms
//    Chance  14
//	ProtCoverage	1.5
//
//ArmorZone
//    Name    Head
//    Chance  14
//	ProtCoverage	1.5
//
//ArmorZone
//    Name    Legs/feet
//    Chance  14
//	ProtCoverage	2
//
//ArmorZone
//    Name    Legs
//    Chance  9
//	ProtCoverage	1
//
//ArmorZone
//    Name    feet
//    Chance  5
//	ProtCoverage	1
//
//ArmorZone
//    Name    Neck
//    Chance  7
//	ProtCoverage	1
//
//ArmorZone
//    Name    Hands
//    Chance  7
//	ProtCoverage	1
//
//
// The following table was compiled using the CLENT.INC file. It is here for reference use and contains ideas for possible changes to armorzones to further
// divide them into more precise areas of coverage.
//
// Namne                  Number	Examples                                  Information
// ==================================================================================================================================================================
// LAYER_HAND1		        0x01;   // One handed weapon                        special damage types will be applied to items equipped on layer 0x01
// LAYER_HAND2		        0x02;   // Two handed weapon, shield or misc		  	no armour zone for shields - should they receive protections too?
// LAYER_SHOES		        0x03;   // Shoes									                 	feet
// LAYER_PANTS		        0x04;   // Pants									                 	upper legs - if long pants lower legs as well
// LAYER_SHIRT		        0x05;   // Shirt									                 	upper torso - if sleeved arms as well
// LAYER_HELM 		        0x06;   // Helm or Hat					          			  	head
// LAYER_GLOVES		        0x07;   // Gloves								              	  	hands
// LAYER_RING 		        0x08;   // Ring									              	  	no armour zone coverage but these can have special imbuements
// LAYER_UNUSED9	        0x09;   // Unused.                                  Unused
// LAYER_NECK 		        0x0a;   // Neck										                	neck
// LAYER_HAIR 		        0x0b;   // Hair                                     Hair
// LAYER_WAIST		        0x0c;   // Waist (half apron)			        			  	lower torso
// LAYER_CHEST		        0x0d;   // Torso (inner) (chest armor)    			  	upper torso
// LAYER_WRIST		        0x0e;   // Bracelet									              	no armour zone coverage but these can have special imbuements
// LAYER_PACK2		        0x0f;   // Unused                                   Unused (backpack, but ord. bp is 0x15).
// LAYER_BEARD		        0x10;   // Facial hair.                             Facial hair
// LAYER_TUNIC		        0x11;   // Torso (middle) (tunic, sash etc.)        upper torso - not sure how to handle sash
// LAYER_EARS 		        0x12;   // Earrings                                 no armour zone coverage but these can have special imbuements
// LAYER_ARMS 		        0x13;   // Arms                                     arms
// LAYER_CAPE 		        0x14;   // Back (cloak)                             no armour zone coverage but these can have special imbuements
// LAYER_PACK		          0x15;   // Backpack                                 Backpack
// LAYER_ROBE		          0x16;   // Torso (outer) (robe)			        				upper torso, lower torso, legs, arms (maybe include neck and head depending on robe type)
// LAYER_SKIRT		        0x17;   // Legs (outer) (skirt/robe)	      				legs
// LAYER_LEGS		          0x18;   // Legs (inner) (leg armor)		      				legs
// LAYER_MOUNT            0x19,   // Mount (horse, ostard etc.).
// LAYER_VENDORSTOCK      0x1A,   // This vendor will sell and restock.
// LAYER_VENDOREXTRA      0x1B,   // This vendor will resell to players but not restock.
// LAYER_VENDORBUY        0x1C,   // This vendor can buy from players but does not stock.
// LAYER_BANKBOX          0x1D,   // Contents of bankbox
// LAYER_TRADE            0x1E    // Can be multiple of these, do not use directly.

