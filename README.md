# About BlackThunder

This is a Quake 4 mod that includes edits to several weapon definitions and gui, as well as gamex86.dll code changes that add new functionality to said-modified weapons. 

## Weapon mods:

| Weapon           | Modification                                                 |
| ---------------- | ------------------------------------------------------------ |
| Grenade Launcher | Launches bombs that stick to the environment, enemies, and NPCs. The player remotely detonates all sticky bombs by pressing 'b'. |
| Rocket Launcher  | Launches smoke bombs that paralyze all enemies within its area of affect. Enemies will ragdoll and remain paralyzed as long as the gas persists, then will pop back up fully active. *NPCs are not affected except to act annoyed.* |
| Dark Matter Gun  | Launches pairs of portals that strike the environment and rest on the nearest horizontal surface. A player, enemy, or NPC can step onto one portal and will appear at the other portal instantly. ***Note: Only selectable by pressing '0' (not scrollwheel accessible).*** This modification is compatible with save/load so portals persist between loads. |
| Hyperblaster     | Launches flashgangs that explode and blind any player, enemy, or NPC looking at it. Look away so you don't blind yourself, you'll still see the reflected light and hear it explode. Blinded enemies will be unable to locate the player, even in front of their face, until their blindness wears off. |
| Lightning Gun    | Functions as a magic wand, see below for an explanation of the magic system. |

*Note: All weapons are set to infinite ammo, so no ammo counter will appear for any weapon.*



## Magic System:

1. HUD Mana bars integrated into ammo bar's background ammo percentage ( visible when lightning gun equipped and spell selected )
2. Currently equipped spell name visible in Mana bar ( with lightning gun equipped )
3. Each spell has a maximum mana and casting cost
   * Mana for a spell only recharges once fully spent
   * Cooldowns, casting is disabled while mana is recharging
4. Toggle between four spells (Necromancer, Telekinesis, BlackThunder, Firespout) 
   * *Only Necromancer and Firespout have unique new effects. Telekinesis and BlackThunder do not do anything (not even damage), they only demonstrate unique mana usage and cooldowns.*

| Spell       | Effect                                                       |
| ----------- | ------------------------------------------------------------ |
| No Spell    | If the Lightning Gun's ammo indicator is empty then the normal lightning gun is selected and will function as normal |
| Necromancer | Resurrect a dead enemy as a zombie teammate to follow and help the player. The dead enemy must be shot for a duration proportional to its maximum health, and spell must occur before enemy burnaway begins. Burnaway is disabled while dead enemy is being shot/resurrected. |
| Firespout   | Ignites the enemy, non-persistent/non-sticky flame           |



## GUI Mods (and light-based sneak mechanic):

| GUI       | Modification                                                 |
| --------- | ------------------------------------------------------------ |
| Main Menu | Localized text changed from "New Game" to "Snoop Dogg", and from "Start Game" to "Start Flobble" |
| HUD       | New Light-levels HUD element at screen top-left indicating **Hidden/Visible status based on the user's light levels**. If the UI displays **Hidden** then enemies will not be able to track the player past **last known location**, allowing the player to sneak around in shadow. The enemy will spot the player's new location if the player touches the enemy. |



# Run BlackThunder

1. Unzip **Freehill_BlackThunder_Quake4_Mod.zip**, it contains one folder: **BlackThunder**

2. Copy and paste the **BlackThunder** folder into your legitimate install of Quake 4 at the root directory.

3. Run **Quake4.exe**

4. Once the main menu loads there will be an option called **mods** in the bottom left of the screen, select that option.

   ![](https://github.com/tmf7/Quake4-Game-Mod-Midterm-Project/readme_images/Quake4_ModsOption.png)

 	5. Select **blackthunder** (Quake sets the mod folder name to lowercase), then **Load Mod**

![](https://github.com/tmf7/Quake4-Game-Mod-Midterm-Project/readme_images/Quake4_ModsList.png)

6. You'll know the mod is loaded when you see the main menu again and "New Game" has been replaced with "Snoop Dogg", and "Start Game" has been replaced with "Start Flobble"

​                                                                 ![](https://github.com/tmf7/Quake4-Game-Mod-Midterm-Project/readme_images/BlackThunderLoaded_small.png)             ![](https://github.com/tmf7/Quake4-Game-Mod-Midterm-Project/readme_images/BlackThunderStartGame_small.png) 



# BlackThunder Controls

The keybindings listed here are in addition to Quake 4's vanilla keybindings. For example, the mouse scroll wheel will quickly swap to the next/prev weapon with ammo.

| Keybinding | Function                                                     |
| ---------- | ------------------------------------------------------------ |
| 4          | Select Flash Bang Launcher (replaces the Hyperblaster)       |
| 5          | Select Sticky Bomb Launcher (replaces the Grenade Launcher)  |
| 7          | Select Paralysis Bomb Launcher (replaces the Rocket Launcher) |
| 9          | Select Magic Shit (replaces the Lightning Gun)               |
| 0          | Select Portal Gun (replaces the Dark Matter Gun)             |
| v          | **<span style="color:red">If Magic Shit (Lightning Gun) is currently selected</span>** swap active spell between *No Spell (normal gun)*, *Necromancer*, *Telekinesis*, *BlackThunder*, and *Firespout* |
| b          | **<span style="color:red">If the player has launched any sticky bombs</span>**, detonates all sticky bombs simultaneously |



# Moding my mod in Visual Studio

1. *(optional)* Make your code edits, if you like, just know that building the BlackThunder branch of my mod will not output vanilla Quake 4. It will output my dll-side code modifications as used by my BlackThunder mod. 
   - *(optional)* You can also edit the various resource files (.def, .lang, .gui, etc) I've created, or create your own mod from Quake's vanilla copies.

2. Set the Configuration to **Release** and the Platform to **Win32**

3. Either build the entire solution or just the **game** project (the **idlib** project will build)

   * If there are build errors its likely caused by updates to Visual Studio.
   * Some troubleshooting:
     * **Add spaces** around all S_COLOR_RED, S_COLOR_WHITE, etc code to ensure the macros can resolve. EG: idLib::common->Printf( "   simd->Memcpy() " S_COLOR_RED "X\n" );
     * Change all **"TypeInfo.h"** to <typeinfo> so the includes can resolve
   * Make sure have the **MFC** that matches the project build **target**, you can get MFC (Microsoft Foundation Classes) from the Visual Studio Installer.

4. Navigate to the Output directory, which should be one level above the project as **Win32/Release**
   * Identify the **gamex86.dll** file that the **game** project build outputs, that new **gamex86.dll** contains the relevant code changes for this mod and will be used in the following steps.

5. Navigate to the root directory of your legitimate install of Quake 4 and **create a new folder**, name it whatever you like. 
   * The name is not important except that it will help you identify your mod if loading from within Quake 4. For example, my mod is called "BlackThunder" so that's the name of my mod folder.

6. Within your legitimate Quake 4 install root directory, navigate into the **q4base** directory and copy the **game000.pk4** file into your new **mod folder** (eg: BlackThunder)
   * game000.pk4 is an idTech4 formatted zip file, it contains the main gamex86.dll loaded at runtime by Quake4.exe, as well as some important metadata

7. Rename the copy of **game000.pk4** to **game000.zip**, this will allow Windows read/write access to the file.

8. Open **game000.zip**, delete the existing **gamex86.dll** from the zip, insert your newly built copy of **gamex86.dll** into the zip. Close the file

9. Rename **game000.zip** to **game000.pk4**

10. Either copy-paste all contents of my **BlackThunder** folder, except the **game000.pk4** you've just made, or make your own defs, strings, guis, and configs from the vanilla Quake 4 files found in **q4base/pak001.pk4**.

    * If you choose to make your own def files, etc, then remember that pak001.pk4 can be renamed to pak001.zip in order to access its contents.

11. Ensure the mod folder structure and naming is as follows (your mod  root folder can be whatever you like, but adhere to the rest):

    ![Quake4Mod_FolderStructure](C:\Users\Tom Freehill\Desktop\Quake4Mod_FolderStructure.png)

    * If you have trouble with the folder structure, or are adding other modifications, refer to the folder structure found within q4base/pak001.pk4 and follow that for your mod folder. This will allow Quake 4 to find all your mod files.

12. **Run your mod**, as described from Step 3 onward under the above section named "Run BlackThunder".
13. **Document** your mod and **share** it! **Have fun!**



## List of DLL and Def Changes

* All changes are found in the **game** and **definitions** folders of this repository.

* All code changes are bound by **//TMF7 BEGIN** and **//TMF7 END** to clearly demarcate what I've added/changed from the base game code.

* Use **ctrl+shift+f**  in Visual Studio to search for all my changes in a navigable list

### Quake 4 source game file changes

AI.h
Entity.h
Game_local.h
Light.h
Player.h
Projectile.h
UsercmdGen.h
Actor.cpp
AI.cpp
AI_events.cpp
Entity.cpp
Game_local.cpp
Light.cpp
Player.cpp
PlayerView.cpp
Projectile.cpp
SysCvar.cpp
Wearpon.cpp
WeaponDarkMatterGun.cpp
WeaponLightningGun.cpp

### q4base/pak001.pk4 file changes
dmg.def
grenadelauncher.def
hyperblaster.def
lightninggun.def
player.def
rocketlauncher.def
hud.gui
english_code.lang
Quake4Config.cfg
