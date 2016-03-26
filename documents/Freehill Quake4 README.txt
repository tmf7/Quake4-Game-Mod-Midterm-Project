Pull brach "BlackThunder"

MOD INSTRUCTIONS:
1-place the contents of the attached .zip in the quake4 directory alongside q4base
2-place the Quake4Config.cfg from the attached .zip file in the q4base directory ( it contains my two additional key bindings )


weapon mods:
1-Sticky Bombs		( grenade launcher, 'b' key detonates all player bombs )
2-Paralysis Bombs	( rocket launcher, ragdolls enemies in radius as long as gas persists )
3-Portal Gun		( dark matter gun, two portals persistently active, compatible with save/load )
4-Flash bang Bombs	( hyperblaster, any AI or player that can see the detonation loses target tracking
					 for 20 seconds, player screen flashes white and fades over time )
5-
6-
7-
8-


spells ( 'v' key switches the equipped spell, spells only selectable with lightning gun equipped )
0-NO_SPELL		( lightning gun, normal lightning gun damgage and fx )
1-Necromancer		( lightning gun, spawns a zombie teammate if a dead enemy is shot for a duration
				proportional to its maximum health, must occur before enemy burnaway begins )
2-
3-
4-FIRESPOUT		( lightning gun, ignites enemy, non-persistent/non-sticky flame )


Magic System:
1-HUD Mana bars integrated into ammo bar's background ammo percentage ( visible when lightning gun equipped and spell selected )
2-Currently equipped spell name visible in Mana bar ( with lightning gun equipped )
3-Each spell has a maximum mana and casting cost, 
	mana for a spell only recharges once its fully spent, 
	cannot cast while recharging
4-System in place to operate four spells (Necromancer, Telekinesis, BlackThunder, Firespout) 


Enemy response to Player Light Levels:
1-HUD shows current visibility and illumination level
2-Enemies unable to track "HIDDEN" player past last visible position (unless player touches them)


General:
1-All weapons currently have infinite ammo
2-Third person hud and crossair visible (camera position and world clipping adjusted)
