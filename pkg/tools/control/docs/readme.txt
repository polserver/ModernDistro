ICP/Control

Email: Austin@tsse.net
Shard: http://www.tsse.net

Background: This is based on Myrathi's original control package but
includes significantly more features and is made from completely new code.
This newest system features - script starting, command listing, and itemdesc.cfg
listing. It has gump cache for faster load times and SIGNIFICANTLY reduced CPU load.

Installation: 
Extract the files to where ever you have your packages at. I have found that
ever shard seems to have its own unique directory structure. (Standardize!)

If you're using the distro, my recommendation would be to put it over the existing
control package which can be found under foundations (As of the .94 distro)

You will need to have my datafile package as well. It can be found in the includes section
of the POL Script Forum. The datafile package is needed so that checkpackages.src
can compile.

You will also need to make sure you have the latest gump system. This requires gumps 1.5
by Melanius and myself.

This new ICP system no longer uses the critical scripts. It instead uses a config
file called "icp.cfg" so if you have any of the older "ctrlReg.src" scripts you can
remove them. 

The ICP.cfg files belong in a folder called "configs" within the package.
Example: X:/pol/pkg/control/config/icp.cfg

The current format for an icp.cfg file is:

ICP Register
{
	Name		<Package Name>
	Version		<x.x>
	Description	<Description of the package>
	
	Creator		<Creator Name>
	C_Email		<Creator's Email>
	
	Maintainer	<Maintainer's Name>
	M_Email		<Maintainer's Email>
	
	Script		<cmdlevel> <path>
	
	TextCmd		<cmdlevel> <path>
}

Script and TextCmd lines can be defined as many times as needed.
The command level parameter is optional but can be used to make it so
that counselors can not run seer commands, seers can not run GM commands, 
GMs can't run admin commands etc. It is entirely up to who ever sets up the 
config file for that package.

For those who are unfamiliar with how the path would work for text commands,
see how they are done in the icp.cfg for this package.


To use this system ingame, type .control

