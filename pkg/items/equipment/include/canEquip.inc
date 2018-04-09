/*
        Maintened by *Edwards

        2009-08-20
        
        Last Modifications 2009-08-29
             * Added ProhibitedMod()
             * Added ProhibitedProtection()

        Last Modifications 2009-11-16
             * No classe except mage can equip psychic/astral weapons
        
        To-Do Deal with poss mod on items
*/
use uo;

include ":attributes:attributes";
//include ":classes:classes";
include ":equipment:equipment";

function ProhibitedByClasse( mobile, item )
  /*      
        var classe := Get_Mobile_Classe_Name( mobile );
        if( !classe || classe == PP )
                // No classed or PP, can equip
                return 1;
        endif

        if( !ProhibitedObjType( item, classe ))
                return 0;
        elseif( !ProhibitedMod( item, classe ))
                return 0;
        elseif( !ProhibitedProtection( item, classe ))
                return 0;
        elseif( !ProhibitedByItem( item, classe ))
                return 0;
        endif
*/        
        return 1;
endfunction

function ProhibitedObjType( item, classe )
/*
	case( classe )
		BARD:       if( item.IsPlatemailArmor() )
                                    return 0;
                            elseif( item.IsShield() )
                                    return 0;
                            elseif( item.IsMagicalWeapon() )
                                    return 0;
                            endif
                            break;

		CRAFTER:    if( item.IsMagicalWeapon() )
                                    return 0;
                            endif
                            break;

		MAGE:       if( item.IsStuddedLeatherArmor() )
                                    return 0;
                            elseif( item.IsBoneArmor() )
                                    return 0;
                            elseif( item.IsRingmailArmor() )
                                    return 0;
                            elseif( item.IsChainmailArmor() )
                                    return 0;
                            elseif( item.IsPlatemailArmor() )
                                    return 0;
                            elseif( item.IsShield() )
                                    return 0;
                            endif
                            break;

		RANGER:     if( item.IsPlatemailArmor() )
                                    return 0;
                            elseif( item.IsShield() )
                                    return 0;
                            elseif( item.IsMagicalWeapon() )
                                    return 0;
                            endif
                            break;
                            
		THIEF:      if( item.IsBoneArmor() )
                                    return 0;
                            elseif( item.IsRingmailArmor() )
                                    return 0;
                            elseif( item.IsChainmailArmor() )
                                    return 0;
                            elseif( item.IsPlatemailArmor() )
                                    return 0;
                            elseif( item.IsShield() )
                                    return 0;
                            elseif( item.IsMagicalWeapon() )
                                    return 0;
                            endif
                            break;
                            
		WARRIOR:
                            if( item.IsMagicalWeapon() )
                                    return 0;
                            endif
                            break;
	endcase
*/
        return 1;
endfunction

function ProhibitedMod( item, classe )

        var skill_mod := GetEquipmentModifiers( item );
        if( !skill_mod.Size() )
                return 1;
        endif

        var skill_keys := skill_mod.Keys();
        foreach modifier in skill_keys
                SleepMS(5);
                if( modifier == FORENSIC_EVALUATION )
                        continue;
                elseif( modifier in AP_ListAttributesByType( "Skill" ))
                        if( modifier in GetClasseSkills( classe ))
                                continue;
                        endif
                        return 0;
                endif
        endforeach

        return 1;
endfunction

function ProhibitedProtection( item, classe )

/*
	case( classe )
		MAGE:		if( GetObjProperty( item, "PhysicalProtection" ))
					return 0;
				elseif( GetObjProperty( item, "HealingIncrease" ))
					return 0;
                                elseif( GetObjProperty( item, "ArBonus" ))
                                        return 0;
                                endif
                                break;
                                
		WARRIOR:	if( GetObjProperty( item, "MagicProtection" ))
					return 0;
				elseif( GetObjProperty( item, "PermMagicProtection" ))
					return 0;
				elseif( GetObjProperty( item, "SpellReflection" ))
					return 0;
				elseif( GetObjProperty( item, "PermSpellReflection" ))
					return 0;
				elseif( CInt( GetObjProperty( item, "material" )) == 25382 ) //Ebon
					return 0;
				elseif( CInt( GetObjProperty( item, "material" )) == 25383 ) //Ruby
					return 0;
				elseif( CInt( GetObjProperty( item, "material" )) == 25384 ) //Nimbus
					return 0;
				endif
				break;
	endcase
*/
	return 1;
endfunction

function ProhibitedByItem( item, classe )
/*
        if( item.BookType() )
                if( item.BookType() == "Song" && classe != BARD )
                        return 0;
                elseif( classe != MAGE )
                        return 0;
                endif
        endif
*/        
	return 1;
endfunction
