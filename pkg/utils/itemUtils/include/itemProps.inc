/* $Id: itemInfo.inc 375 2006-06-17 19:26:32Z austinheilman $
 *
 */
use uo;
use os;
use cfgfile;
use polsys;


/*
 * IU_TransferItemProps(byref from_item, to_item)
 *
 * Purpose
 * Transfers "special" Item Property member's values to an item's modifiers.
 * For example to_item.fire_damage_mod := from_item.FireDamage
 *
 * Parameters
 * from_item - The item containing the members whose values are to be transferred.
 * to_item - The item whose modifiers will receive the member's values.
 *
 * Return value
 * Returns 1 on success.
 * Returns an errortext on failure.
 *
 */
function IU_TransferItemProps(byref from_item, to_item)
	// Damages
	if(to_item.IsA(POLCLASS_WEAPON))
		to_item.damage_physical_mod := to_item.damage_physical_mod + CInt(from_item.damage_physical);
		to_item.damage_fire_mod := to_item.damage_fire_mod + CInt(from_item.damage_fire);
		to_item.damage_cold_mod := to_item.damage_cold_mod + CInt(from_item.ColdDamage);
		to_item.damage_poison_mod := to_item.damage_poison_mod + CInt(from_item.damage_cold);
		to_item.damage_energy_mod := to_item.damage_energy_mod + CInt(from_item.damage_energy);
		to_item.lower_reagent_cost_mod := to_item.lower_reagent_cost_mod + from_item.lower_reagent_cost;
		to_item.spell_damage_increase_mod := to_item.spell_damage_increase_mod + from_item.spell_damage_increase;
		to_item.faster_casting_mod := to_item.faster_casting_mod + from_item.faster_casting;
		to_item.faster_cast_recovery_mod := to_item.faster_cast_recovery_mod + from_item.faster_cast_recovery;
		to_item.defence_increase_mod := to_item.defence_increase_mod + from_item.defence_increase;
		to_item.lower_mana_cost_mod := to_item.lower_mana_cost_mod + from_item.lower_mana_cost;
		to_item.hit_chance_mod := to_item.hit_chance_mod + from_item.hit_chance;
		to_item.swing_speed_mod := to_item.swing_speed_mod + from_item.swing_speed;
		to_item.damage_increase_mod := to_item.damage_increase_mod + from_item.damage_increase;
		to_item.luck_mod := to_item.luck_mod + from_item.luck;
	endif

	// Resistances
	if(to_item.IsA(POLCLASS_ARMOR))
		to_item.resist_physical_mod := to_item.resist_physical_mod + CInt(from_item.resist_physical);
		to_item.resist_fire_mod := to_item.resist_fire_mod + CInt(from_item.resist_fire);
		to_item.resist_cold_mod := to_item.resist_cold_mod + CInt(from_item.resist_cold);
		to_item.resist_poison_mod := to_item.resist_poison_mod + CInt(from_item.resist_poison);
		to_item.resist_energy_mod := to_item.resist_energy_mod + CInt(from_item.resist_energy);
		to_item.lower_reagent_cost_mod := to_item.lower_reagent_cost_mod + from_item.lower_reagent_cost;
		to_item.spell_damage_increase_mod := to_item.spell_damage_increase_mod + from_item.spell_damage_increase;
		to_item.faster_casting_mod := to_item.faster_casting_mod + from_item.faster_casting;
		to_item.faster_cast_recovery_mod := to_item.faster_cast_recovery_mod + from_item.faster_cast_recovery;
		to_item.defence_increase_mod := to_item.defence_increase_mod + from_item.defence_increase;
		to_item.lower_mana_cost_mod := to_item.lower_mana_cost_mod + from_item.lower_mana_cost;
		to_item.hit_chance_mod := to_item.hit_chance_mod + from_item.hit_chance;
		to_item.swing_speed_mod := to_item.swing_speed_mod + from_item.swing_speed;
		to_item.damage_increase_mod := to_item.damage_increase_mod + from_item.damage_increase;
		to_item.luck_mod := to_item.luck_mod + from_item.luck;
	endif



endfunction

