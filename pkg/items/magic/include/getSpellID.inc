///////////////////////////////////////////////////
//
// GetSpellId - SpellId is stored in itemdesc.cfg
//
///////////////////////////////////////////////////
use cfgfile;

function GetSpellId( magicitem )
    var itemdesc_cfg := ReadConfigFile( "itemdesc" );
    return itemdesc_cfg[ magicitem.objtype ].spellid;
endfunction
