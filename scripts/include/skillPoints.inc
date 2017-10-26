use cfgfile;

function get_default_points(skillid)
        var cfg := ReadConfigFile("skills");

        var default_points := cfg[skillid].default_points;
       
        if (!default_points)
                return 0;
        else
                return default_points;

        endif

endfunction