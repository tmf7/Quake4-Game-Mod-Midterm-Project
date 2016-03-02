Code changes that have been verified in-game:

2/25/2016
-player.def from pak22.pk4
	->changed starting weapon to 50rounds of machine gun only
	->added line for pm_thirdperson 1, due to some other file overriding the .cpp default of 1
-changed SysCVar.cpp to make pm_thirdperson 1 (as well as clipping)