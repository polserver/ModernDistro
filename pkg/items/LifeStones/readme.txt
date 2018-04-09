Here is my Life Stones Package.

There are three different stones that allow the user to store "life force" in them, Mana Stones, Health Stones and Stamina Stones.

I think this is the first script package I wrote almost all from scratch.

Usually I "borrow" from other scripts and paste things together and make them all work but this one is almost all mine


Well, I did steal from the "own" command in the World of Dreams scripts and you will see that when you peruse the scripts.


The following describes the way the Mana Stones work. The procedure is the same for all stones.

/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //
// Mana Stone script                                                                               //  
// Double clicking a Mana Stone will use it.                                                       //
// If it is not "linked" to someone, ie. never used before, it will be linked to the user and the  //
// user will get a target.                                                                         //
// If it has been "linked" then there is a check to verify if the user is the one linked to the    //
// stone. If so then the user will get a target. If not then the user will get an error message.   //
// If the user targets the stone her mana will charge the stone. Her mana will be drained until    //
// either she reaches 1 or the stone is fully charged. If she targets herself then mana will       // 
// be transferred from the stone to her until either her mana is full or the stone is empty.       //
//                                                                                                 //
/////////////////////////////////////////////////////////////////////////////////////////////////////


Just drop the Life Stones package into your "pkg" directory. Be sure the object type numbers in the itemdes.cfg file do not conflict with any others you may have.

I have included the "include" files that are referenced in the package. Be sure to back-up any files you replace by adding these to your server just in case there are functions that are in your includes that you may need.

If you wish to use the added CProps I have scripted onto the Stones that allow players to see how "worn" the Stones are and how much stored life energy they have then add the following to your Item ID script somewhere after the item was targetted.

You'll have to decide what level of item id your players will need in order to be able to identify Life Stones properly.

		var msg1;
		if( SkillCheck( character, ATTRIBUTEID_ITEMID, 65, 50) )
                // If the item is a mana stone
                    if (the_item.objtype == 0x7093)
                        msg1 := "The stone seems to be " + GetObjProperty(the_item, "Condition") + " and appears to have " +
                        GetObjProperty(the_item, "Quantity") + " Mana ";
                        SendSysMessage( character, msg1, color := 88 );
                        return 1;
                    endif
            
                // If the item is a health stone
                    if (the_item.objtype == 0x7094)
                        msg1 := "The stone seems to be " + GetObjProperty(the_item, "Condition") + " and appears to have " +
                        GetObjProperty(the_item, "Quantity") + " health";
                        SendSysMessage( character, msg1, color := 88 );
                        return 1;
                    endif
            
                // If the item is a stamina stone
                    if (the_item.objtype == 0x7095)
                        msg1 := "The stone seems to be " + GetObjProperty(the_item, "Condition") + " and appears to have " +
                        GetObjProperty(the_item, "Quantity") + " stamina";
                        SendSysMessage( character, msg1, color := 88 );
                        return 1;
                    endif
		else
			SendSysMessage (character, "You can't seem to sense anything about this object.", color := 33);
			return 0;
		endif

Remember if you changed the objtype numbers for the stones to make the necessary adjustments in the code above.

This is given freely to the POL community to use as you wish. The only thing I ask is that if you add to this and redistribute it that you give me credit for the original package and that you include a link to my website in the documentation you include in your distribution. Also, please send me a copy of the modified package. If you use this on your shard I would appreciate you sending me the link to your website.

My shard's website is : www.hopelives.us
My email address is : hopelivesproject@yahoo.com

I hope you find this package useful.

Send any bug reports my way if you wish. Especially if you fix the bugs.


Sincerely,
Yukiko
Titus 2:13

