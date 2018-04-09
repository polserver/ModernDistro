README.TXT
================================
Package: gumps
Version: 2.2
Require: 090 (core)
Creator: TJ Houston (Myrathi)
Contact: tjh@myrathi.co.uk
Created: 24-Jan-2000
Updated: 15-Aug-2001
================================

Files included in this archive:

* /gumps/docs/cmd_overview.txt   -- Overview of core "gump commands"
* /gumps/docs/gumps_inc.html     -- Help file
-
* /gumps/examples/areyousure.src -- Example script 1: "Are You Sure?" gump
* /gumps/examples/radiogroup.src -- Example script 2: Radio-button groups
-
* /gumps/textcmd/gm/gfchart.src  -- Color-chart text-command
-
* /gumps/colorchart.zip          -- ZIP'd bitmap for reference
* /gumps/ctrl_reg.src            -- Registration script for the ICP package
* /gumps/gumps.inc               -- The main 'include' file (see below)
-
* /gumps/README.TXT              -- <this file>

I don't pass shard-virii scripts but if you're in the least bit dubious about
the nature of any ECL files within this package, feel free to recompile them!
You should really do that with all supplied .SRC files, anyway! :-) Also, do
note that the scripts within this package are now compiled for an 092 core.

I, also, made sure that this package is as shard-friendly as possible and is,
initially, **DISABLED** in 'pkg.cfg'. To enable it, simply alter the entry in
'pkg.cfg' from "Enabled 0" to "Enabled 1" and restart your server. :-)

- This package requires at least core 090 due to its use of the 'struct' type.
- The ICP package is now supplied as part of the default distro.

To use the 'gumps.inc' file from another script, you must:

1) ENSURE that this package is installed to your main /pkg/std directory and
   *NOT* under the /opt package directory. If you insist on installing it to
   the /opt directory then you must alter the following "include" line
   appropriately.
2) Ensure this package is enabled (see previous paragraph)
3) Use the following line in your script:

  include "../pkg/std/gumps/gumps";


Alternatively, you can copy the "gumps.inc" file to your /scripts/include
folder and use:

  include "include/gumps";


Personally, I prefer the first one; keeps things tidy :)

( Hopefully this will be able to be changed to ":gumps:gumps" when Syzygy )
( gets the package-relative include references complete :-) *grin*        )


I hope you find this package extremely useful and that it saves you a lot
of hair! :-)  (hardcore gump-building tends to make you pull it out...)

Any comments, ideas, flames or general "stuff" should go, by email, to:

   tjh@myrathi.co.uk
   
Happy scripting!

- Myrathi

=============================================================================
All files within this archive are (OS) TJ Houston (Myrathi), 15-Aug-2001
and are, as such, freely editable. Please give due credit, ok? :-)