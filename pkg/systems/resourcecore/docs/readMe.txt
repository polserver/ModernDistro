This package is designed to control the harvesting of resources such as sand, clay, trees etc.
It is a simpler resource management scheme that the built-in Harvest Resource system.
Currently it is only implemented for sand and clay but the plan is to expand it to control
all harvetable resources.

This system manages resources on a square by square basis. Each square has a limited amount
of the resorce associated with that terrain or object type, MaxYield, which is set in the
settings.cfg file. The regeneration time for a resource is set by the SecondsPerRegrow
setting. Each type of resource has its own independant settings.

One thing this system does not do is maintain an global total of a resource in each facet.
This means that the global availability of a resource is only limited by the terrain and
object types and the SecondsPerRegrow timer. So if a shard has 1,000 players all harvesting
clay, for example, there is no maximum amount of clay in a facet that could be reached after
which no more clay is available until it regenerates.
This is something that could be added in the future. 
The built-in Harvest Resource system does maintain a global resource limit.

Thanks to ThisIsMe for this package.