





function ShockwaveEffect( x, y, z, realm, color, range:=5 )

    var i := 0,
        new_z := 0;
    for( i:=0; i<=range;i+=1 )
        var coords := GetMidpointCircleCoords( x, y, i );
        foreach coord in ( coords )
            new_z := GetStandingHeight( coord.x, coord.y, z, realm );
            PlayStationaryEffectEx( coord.x, coord.y, new_z.z, realm, 0x3728, 1, 13, color, 4, 0 );
        endforeach

        SleepMS( 75 );
    endfor
endfunction