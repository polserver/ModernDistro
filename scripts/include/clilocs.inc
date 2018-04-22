use cliloc;

enum CRAFT_GENERAL_CLILOCS
	CLI_BROKEN_TOOL         	  :=  1044038, // You have worn out your tool!
	CLI_TOOL_IN_PACK         	  :=  1044263, // The tool must be on your person to use.
	CLI_TOOL_EQUIPPED         	:=  1048146, // If you have a tool equipped, you must use that tool.
	CLI_FAILED_WASTAGE         	:=  1044043, // You failed to create the item, and some of your materials are lost. 
	CLI_FAILED_NO_WASTAGE       :=  1044157, // You failed to create the item, but no materials were lost. 
	CLI_SUCCESS_BARELY         	:=  502785,  // You were barely able to make this item.  It's quality is below average. 
	CLI_SUCCESS         		    :=  1044154, // You create the item.
	CLI_SUCCESS_EXCEPTIONAL     :=  1044155, // You create an exceptional quality item.
	CLI_SUCCESS_EXCEPTIONAL_MM  :=  1044156  // You create an exceptional quality item and affix your maker's mark. 
endenum

enum ALCHEMY_CLILOCS
  CLI_
endenum

enum BLACKSMITH_CLILOCS
	CLI_NEAR_FORGE				      := 1044265, //You must be near a forge.
	CLI_NEAR_ANVIL				      := 1044266, //You must be near an anvil.
	CLI_NEAR_ANVILFORGE_SMITH	  := 1044267, //You must be near an anvil and a forge to smith items.
	CLI_STRANGE_METAL			      := 1044268, //You cannot work this trange and unusual metal.
	CLI_CANNOT_WORK_METAL		    := 1044269, //You have no idea how to work this metal.
	CLI_SMELT_ITEMS				      := 1044270, //You melt the item down into ingots.
	CLI_CANNOT_SMELT_TOOL		    := 1044271, //You can't melt down the too you are working with!
	CLI_CANNOT_SMELT_ITEM		    := 1044272, //You can't melt that down into ingots.
	CLI_TARGET_ITEM_SMELT		    := 1044273, //Target an item to recycle.
	CLI_ITEM_IN_PACK_SMELT		  := 1044274, //The item must be in your backpack to recycle it.
	CLI_ITEM_IN_PACK_REPAIR		  := 1044275, //The item must be in your backpack to repair it.
	CLI_TARGET_ITEM_REPAIR		  := 1044276, //Target an item to repair.
	CLI_CANNOT_REPAIR_ITEM		  := 1044277, //That item cannot be repaired.
	CLI_TOO_MANY_REPAIRS		    := 1044278, //That item has been repaired many times, and will break if repairs are attempted again.
	CLI_REPAIR_SUCCESS			    := 1044279, //You repair the item.
	CLI_REPAIR_FAIL				      := 1044280, //You fail to repair the item.
	CLI_FULL_REPAIR				      := 1044281, //That item is in full repair.
	CLI_NEAR_ANVILFORGE_REPAIR	:= 1044282, //You must be near a forge and an anvil to repair items.
	CLI_CANNOT_REPAIR_THAT		  := 1044283  //You cannot repair that.
endenum

enum CARTOGRAPHY_CLILOCS
	CLI_LVL_1_NEEDED			      := 1073495, //You need more level 1 treasure maps that have been completed by you.
	CLI_LVL_2_NEEDED			      := 1073497, //You need more level 2 treasure maps that have been completed by you.
	CLI_LVL_3_NEEDED			      := 1073499, //You need more level 3 treasure maps that have been completed by you.
	CLI_LVL_4_NEEDED			      := 1073501, //You need more level 4 treasure maps that have been completed by you.
	CLI_LVL_5_NEEDED			      := 1073503  //You need more level 5 treasure maps that have been completed by you.
endenum

enum CARPENTRY_CLILOCS
	
endenum

enum GLASSBLOWING_CLILOCS
	CLI_GLASSBLOW_NOT_LEARNED	  := 1044634, // You havent learned glassblowing.
	CLI_GLASSBLOW_FORGE			    := 1044628  // You must be near a forge to blow glass.
endenum

enum MASONRY_CLILOCS
	CLI_MASONRY_NOT_LEARNED		  := 1044633  // You havent learned stonecraft.
endenum

enum INSCRIPTION_CLILOCS
	CLI_INSCRIPT_NEED_SPELL		  := 1042404, // You don't have that spell!
	CLI_INSCRIPT_FAILED			    := 501630,  // You fail to inscribe the scroll, and the scroll is ruined.
	CLI_INSCRIPT_SUCCESS		    := 501629   // You inscribe the spell and put the scroll in your backpack.
endenum

enum EAT_CLILOCS
  CLI_EAT_OWNED               := 500866, //You can't eat that, it belongs to someone else.
  CLI_EAT_TOO_FULL            := 500867, //You are simply too full to eat any more!
  CLI_EAT_EXTREME_HUNGER      := 500868, //You eat the food, but are still extremely hungry.
  CLI_EAT_SATIATED            := 500869, //You eat the food, and begin to feel more satiated.
  CLI_EAT_LESS_HUNGRY         := 500870, //After eating the food, you feel much less hungry.
  CLI_EAT_QUITE_FULL          := 500871, //You feel quite full after consuming the food.
  CLI_EAT_STUFFED             := 500872  //You manage to eat the food, but you are stuffed!
endenum
