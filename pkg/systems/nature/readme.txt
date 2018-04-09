
Ok, here's version 2 of my nature package. Cleaned up, everything works that
I know of. See changes.txt for a list of what I did when.

Installing-

There are two includes you need from the PSF files section: std.inc and
arrays.inc. The URLs are:
http://groups.yahoo.com/group/pol-scriptforum/files/Includes/std.inc
http://groups.yahoo.com/group/pol-scriptforum/files/Includes/arrays.inc

You'll then need to either rename or comment out the GetTimeofDay function
in either std.inc or clock.inc. Doesn't matter which, I don't use it 
anywhere, but since there is a function of that name in both includes it
confuses ecompile.

Note also that this package uses the distro version of include/clock.inc.

VERY IMPORTANT- if you are using this package, you cannot have Raxxla's
daynightmoons&weather package enabled!!

Administrating-

Here's a list of things you can toggle and where:
There are two options that are in nature.inc and require a recompile of the
package if you change them: 

const NIGHTLIGHT := 27;
const DAWNTIME := 900;

NIGHTLIGHT is the maximum light level- the darkest things will get. I chose
27 to reserve 28-30 to the dungeons. If you want night to be lighter, lower
this number.

DAWNTIME is how many seconds after midnight the sun rises at.

In nature.cfg, there are a few more options you can toggle.

The first is seasons. Your options are: no seasons, all seasons, all seasons
except winter (the "autumn" season lasts twice as long as the others), and
all seasons except winter is replaced with desolation (the look on OSI's
Felucca facet). 
Seasons do not work in 1.26.4.

The second is clienttype- set this to whatever you have the client set to in
pol.cfg. This dictates which weather packet to send. (Sending the wrong packets
will cause the client to lock up.)

The third is broadcasts- this dictates whether or not the server announces 
"it is dawn" and the like as time marches on. It defaults to off, where the
changes in light level speak for themselves.

The last is resendtime. Basically, when it is raining, the client only shows
rain for a little while, then gives up. This is how often the server resends
the "make it rain!" packet to the client. A long interval means there will be
gaps in the rain. A short one, however, means the client announces "It begins
to rain" every 45 seconds or whatever you choose.

If you want to change the amount of time various things take- go into days.cfg,
which should live in your /config directory. By default, it has two useful
settings- WholeDay and Transition. WholeDay length 60 means a "24 hour" day in-
game will take one hour of real time. Transition length 5 means the gradual 
change from night to day will take 5 real minutes.
You can also add two flags to it, which change the ammount of time Felucca and
Trammel take to phase.

DayConfig Trammel
{
	Length  60
}

DayConfig Felucca
{
	Length 20
}

Those are the default settings- if you don't set them at all, those are used. 
With those values, Trammel phases once each real hour, and Felucca once
each real 20 minutes.

Those are the tweaks and frobs. Have fun, and if you have any bug reports or
feature requests, email them to PSF or to burra@alum.rpi.edu. 

Oh, the text commands. They are:

PLAYER- .lookup - Tells you the current weather and moonphases.
SEER- .setlocallight - sets the light level of people nearby.
.localthunderstorm - creates thunder and lightning. Does not at the moment also
    create rain. I'm waiting on gaining some info before implimenting that.
GM- .setday, .setmonth, and .setyear - changes the current day, month, or year.
    If you give it nonsense, it sets it to 1. If you give it a number outside
    the scope of the calendar, it accepts it but resets it to one next time that
    part of the calendar changes.
.resetcalendar - Resets it to 1/1/1.
.desolation - either use ".desolation on" or ".desolation off". Sets the world
    to desolate, overriding the current season.

 - Madman Across the Water
