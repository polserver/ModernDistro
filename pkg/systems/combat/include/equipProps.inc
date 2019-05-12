/*
 *
 * These functions are probably obsolete because the Core will transfer thes mods to the character :(

 function CS_AddEquipmentPropsToChar(character, byref equipment)

    character.lower_reagent_cost_mod := character.lower_reagent_cost_mod + equipment.lower_reagent_cost;
    character.spell_damage_increase_mod := character.spell_damage_increase_mod + equipment.spell_damage_increase;
    character.faster_casting_mod := character.faster_casting_mod + equipment.faster_casting;
    character.faster_cast_recovery_mod := character.faster_cast_recovery_mod + equipment.faster_cast_recovery;
    character.defence_increase_mod := character.defence_increase_mod + equipment.defence_increase;
    character.lower_mana_cost_mod := character.lower_mana_cost_mod + equipment.lower_mana_cost;
    character.hit_chance_mod := character.hit_chance_mod + equipment.hit_chance;
    character.swing_speed_mod := character.swing_speed_mod + equipment.swing_speed;
    character.damage_increase_mod := character.damage_increase_mod + equipment.damage_increase;
    character.luck_mod := character.luck_mod + equipment.luck;

endfunction

function CS_SubtractEquipmentPropsFromChar(character, byref equipment)

    character.lower_reagent_cost_mod := character.lower_reagent_cost_mod - equipment.lower_reagent_cost;
    character.spell_damage_increase_mod := character.spell_damage_increase_mod - equipment.spell_damage_increase;
    character.faster_casting_mod := character.faster_casting_mod - equipment.faster_casting;
    character.faster_cast_recovery_mod := character.faster_cast_recovery_mod - equipment.faster_cast_recovery;
    character.defence_increase_mod := character.defence_increase_mod - equipment.defence_increase;
    character.lower_mana_cost_mod := character.lower_mana_cost_mod - equipment.lower_mana_cost;
    character.hit_chance_mod := character.hit_chance_mod - equipment.hit_chance;
    character.swing_speed_mod := character.swing_speed_mod - equipment.swing_speed;
    character.damage_increase_mod := character.damage_increase_mod - equipment.damage_increase;
    character.luck_mod := character.luck_mod - equipment.luck;

endfunction

 *
 *
*/