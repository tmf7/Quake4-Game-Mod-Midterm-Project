#include "../../idlib/precompiled.h"
#pragma hdrstop

#include "../Game_local.h"
#include "../Weapon.h"

class rvWeaponMachinegun : public rvWeapon {
public:

	CLASS_PROTOTYPE( rvWeaponMachinegun );

	rvWeaponMachinegun ( void );

	virtual void		Spawn				( void );
	virtual void		Think				( void );
	void				Save				( idSaveGame *savefile ) const;
	void				Restore				( idRestoreGame *savefile );
	void					PreSave				( void );
	void					PostSave			( void );

protected:

	float				spreadZoom;
	bool				fireHeld;

	bool				UpdateFlashlight	( void );
	void				Flashlight			( bool on );

private:

	stateResult_t		State_Idle			( const stateParms_t& parms );
	stateResult_t		State_Fire			( const stateParms_t& parms );
	stateResult_t		State_Reload		( const stateParms_t& parms );
	stateResult_t		State_Flashlight	( const stateParms_t& parms );
//TMF7 BEGIN
	bool				Check_OtherPlayers	( void );
//TMF7 END
	CLASS_STATES_PROTOTYPE ( rvWeaponMachinegun );
};

CLASS_DECLARATION( rvWeapon, rvWeaponMachinegun )
END_CLASS

/*
================
rvWeaponMachinegun::rvWeaponMachinegun
================
*/
rvWeaponMachinegun::rvWeaponMachinegun ( void ) {
}

/*
================
rvWeaponMachinegun::Spawn
================
*/
void rvWeaponMachinegun::Spawn ( void ) {
	spreadZoom = spawnArgs.GetFloat ( "spreadZoom" );
	fireHeld   = false;
		
	SetState ( "Raise", 0 );	
	
	Flashlight ( owner->IsFlashlightOn() );
}

/*
================
rvWeaponMachinegun::Save
================
*/
void rvWeaponMachinegun::Save ( idSaveGame *savefile ) const {
	savefile->WriteFloat ( spreadZoom );
	savefile->WriteBool ( fireHeld );
}

/*
================
rvWeaponMachinegun::Restore
================
*/
void rvWeaponMachinegun::Restore ( idRestoreGame *savefile ) {
	savefile->ReadFloat ( spreadZoom );
	savefile->ReadBool ( fireHeld );
}

/*
================
rvWeaponMachinegun::PreSave
================
*/
void rvWeaponMachinegun::PreSave ( void ) {
}

/*
================
rvWeaponMachinegun::PostSave
================
*/
void rvWeaponMachinegun::PostSave ( void ) {
}


/*
================
rvWeaponMachinegun::Think
================
*/
void rvWeaponMachinegun::Think()
{
	rvWeapon::Think();
	if ( zoomGui && owner == gameLocal.GetLocalPlayer( ) ) {
		zoomGui->SetStateFloat( "playerYaw", playerViewAxis.ToAngles().yaw );
	}
}

/*
================
rvWeaponMachinegun::UpdateFlashlight
================
*/
bool rvWeaponMachinegun::UpdateFlashlight ( void ) {
	if ( !wsfl.flashlight ) {
		return false;
	}
	
	SetState ( "Flashlight", 0 );
	return true;		
}

/*
================
rvWeaponMachinegun::Flashlight
================
*/
void rvWeaponMachinegun::Flashlight ( bool on ) {
	owner->Flashlight ( on );
	
	if ( on ) {
		viewModel->ShowSurface ( "models/weapons/blaster/flare" );
		worldModel->ShowSurface ( "models/weapons/blaster/flare" );
	} else {
		viewModel->HideSurface ( "models/weapons/blaster/flare" );
		worldModel->HideSurface ( "models/weapons/blaster/flare" );
	}
}

/*
===============================================================================

	States 

===============================================================================
*/

CLASS_STATES_DECLARATION ( rvWeaponMachinegun )
	STATE ( "Idle",				rvWeaponMachinegun::State_Idle)
	STATE ( "Fire",				rvWeaponMachinegun::State_Fire )
	STATE ( "Reload",			rvWeaponMachinegun::State_Reload )
	STATE ( "Flashlight",		rvWeaponMachinegun::State_Flashlight )
END_CLASS_STATES


//TMF7 BEGIN
/*
================
rvWeaponMachinegun::Check_OtherPlayers
================
*/
/*
PRECONDITION:	This function is only called when a player is attempting to go from State_Idle to State_Fire (not if already State_Fire)
OUTPUT:			Returns TRUE if the player weilding this machine gun is attempting to fire while any other player is holding their fire button
				AND that firing player is weilding a machine gun, otherwise returns FALSE

IMPROVEMENTS:	(1) Fire failure does an animation
				(2) The FIRST player to hold the fire button locks out all others (instead of a stalemate lockout)
				(3) only award the holy shit award once every time a second player tries
					to fire while the primary gunner is firing (ie NOT every frame while the trigger is held down)
				(4) display a direct chat message for whose stopping the owner's firing attempt
FINISHED 2/29/2016 11:56pm
IMPROVED 3/1/2016 10:40pm
*/
bool rvWeaponMachinegun::Check_OtherPlayers( void )
{
	idEntity* ent = NULL;
	for ( int i = 0; i < gameLocal.numClients; i++ ) 
	{
		ent = gameLocal.entities[ i ];
		
		//make sure the entity is a player
		if ( !ent || !ent->IsType( idPlayer::GetClassType() ) )	{continue;}
		
		//cast the entity that's been verified as an idPlayer
		idPlayer* player = (idPlayer*)ent;

		//make sure the player being checked isn't the one trying to fire
		if ( player == owner ) {continue;}
		
		//all of these flags work more or less the same for this check
		//pfl.weaponFired is only set to true when a player's weapon has ALREADY called the Attack function
		//pfl.attackHeld is set true immedialty PRIOR to the FireWeapon() function call in weapon.cpp
		//wsfl.attack is set to true the instant an attack is initiated in rvWeapon::BeginAttack()
		if ( ( player->weapon->IsType( rvWeaponMachinegun::GetClassType() ) ) && player->pfl.attackHeld ) 
		{
			//make a fun noise/award when it turns out someone else is firing
			//If the owner is still holding the attack button on a NEW frame, then dont provide any awards/animations
			if( !(owner->oldButtons & BUTTON_ATTACK) )
			{	
				//owner "fumbles" with its gun
				//ODD: "reload" animation with blendFrames set to the usual 4 skips the rest of this function
				//but using blendFrames at 0 works just fine i think
				PlayAnim ( ANIMCHANNEL_ALL, "reload", 0 );
				//the person  attempting to fire (owner) gets the award
				statManager->GiveInGameAward( IGA_HOLY_SHIT, owner->entityNumber );
				//print a chat message to the owner of who stopped them from firing (in a disgustingly ineffecient way)
				//BUG: for some reason this prints twice for the second player being blocked by the first player
				//or just a client being blocked by the server-client, but it behaves as expected for the server-client
				idStr msg = player->GetName();
				msg.Append( " stopped ");
				msg.Append( owner->GetName() );
				msg.Append( " from firing.");
				gameLocal.mpGame.PrintMessage( owner->entityNumber, msg.c_str() );
			}
			return true; 
		}
	}
	return false;
}
//TMF7 END

/*
================
rvWeaponMachinegun::State_Idle
================
*/
stateResult_t rvWeaponMachinegun::State_Idle( const stateParms_t& parms ) {
	enum {
		STAGE_INIT,
		STAGE_WAIT,
	};	
	switch ( parms.stage ) {
		case STAGE_INIT:
			if ( !AmmoAvailable ( ) ) {
				SetStatus ( WP_OUTOFAMMO );
			} else {
				SetStatus ( WP_READY );
			}
		
			PlayCycle( ANIMCHANNEL_ALL, "idle", parms.blendFrames );
			return SRESULT_STAGE ( STAGE_WAIT );
		
		case STAGE_WAIT:			
			if ( wsfl.lowerWeapon ) {
				SetState ( "Lower", 4 );
				return SRESULT_DONE;
			}		
			if ( UpdateFlashlight ( ) ) {
				return SRESULT_DONE;
			}

			if ( fireHeld && !wsfl.attack ) {
				fireHeld = false;
			}
			if ( !clipSize ) {
				//TMF7 BEGIN
				if ( !fireHeld && gameLocal.time > nextAttackTime && wsfl.attack && AmmoAvailable ( ) && !Check_OtherPlayers() ) {
				//TMF7 END
					SetState ( "Fire", 0 );
					return SRESULT_DONE;
				}
			} else {
				//TMF7 BEGIN
				if ( !fireHeld && gameLocal.time > nextAttackTime && wsfl.attack && AmmoInClip ( ) && !Check_OtherPlayers() ) {
				//TMF7 END
					SetState ( "Fire", 0 );
					return SRESULT_DONE;
				}  
				if ( wsfl.attack && AutoReload() && !AmmoInClip ( ) && AmmoAvailable () ) {
					SetState ( "Reload", 4 );
					return SRESULT_DONE;			
				}
				if ( wsfl.netReload || (wsfl.reload && AmmoInClip() < ClipSize() && AmmoAvailable()>AmmoInClip()) ) {
					SetState ( "Reload", 4 );
					return SRESULT_DONE;			
				}				
			}
			return SRESULT_WAIT;
	}
	return SRESULT_ERROR;
}

/*
================
rvWeaponMachinegun::State_Fire
================
*/
stateResult_t rvWeaponMachinegun::State_Fire ( const stateParms_t& parms ) {
	enum {
		STAGE_INIT,
		STAGE_WAIT,
	};	
	switch ( parms.stage ) {
		case STAGE_INIT:
			if ( wsfl.zoom ) {
				nextAttackTime = gameLocal.time + (altFireRate * owner->PowerUpModifier ( PMOD_FIRERATE ));
				Attack ( true, 1, spreadZoom, 0, 1.0f );
				fireHeld = true;
			} else {
				nextAttackTime = gameLocal.time + (fireRate * owner->PowerUpModifier ( PMOD_FIRERATE ));
				Attack ( false, 1, spread, 0, 1.0f );
			}
			PlayAnim ( ANIMCHANNEL_ALL, "fire", 0 );	
			return SRESULT_STAGE ( STAGE_WAIT );
	
		case STAGE_WAIT:	
			if ( !fireHeld && wsfl.attack && gameLocal.time >= nextAttackTime && AmmoInClip() && !wsfl.lowerWeapon ) {
				SetState ( "Fire", 0 );
				return SRESULT_DONE;
			}
			if ( AnimDone ( ANIMCHANNEL_ALL, 0 ) ) {
				SetState ( "Idle", 0 );
				return SRESULT_DONE;
			}		
			if ( UpdateFlashlight ( ) ) {
				return SRESULT_DONE;
			}			
			return SRESULT_WAIT;
	}
	return SRESULT_ERROR;
}

/*
================
rvWeaponMachinegun::State_Reload
================
*/
stateResult_t rvWeaponMachinegun::State_Reload ( const stateParms_t& parms ) {
	enum {
		STAGE_INIT,
		STAGE_WAIT,
	};	
	switch ( parms.stage ) {
		case STAGE_INIT:
			if ( wsfl.netReload ) {
				wsfl.netReload = false;
			} else {
				NetReload ( );
			}
			
			SetStatus ( WP_RELOAD );
			PlayAnim ( ANIMCHANNEL_ALL, "reload", parms.blendFrames );
			return SRESULT_STAGE ( STAGE_WAIT );
			
		case STAGE_WAIT:
			if ( AnimDone ( ANIMCHANNEL_ALL, 4 ) ) {
				AddToClip ( ClipSize() );
				SetState ( "Idle", 4 );
				return SRESULT_DONE;
			}
			if ( wsfl.lowerWeapon ) {
				SetState ( "Lower", 4 );
				return SRESULT_DONE;
			}
			return SRESULT_WAIT;
	}
	return SRESULT_ERROR;
}
			

/*
================
rvWeaponMachinegun::State_Flashlight
================
*/
stateResult_t rvWeaponMachinegun::State_Flashlight ( const stateParms_t& parms ) {
	enum {
		FLASHLIGHT_INIT,
		FLASHLIGHT_WAIT,
	};	
	switch ( parms.stage ) {
		case FLASHLIGHT_INIT:			
			SetStatus ( WP_FLASHLIGHT );
			// Wait for the flashlight anim to play		
			PlayAnim( ANIMCHANNEL_ALL, "flashlight", 0 );
			return SRESULT_STAGE ( FLASHLIGHT_WAIT );
			
		case FLASHLIGHT_WAIT:
			if ( !AnimDone ( ANIMCHANNEL_ALL, 4 ) ) {
				return SRESULT_WAIT;
			}
			
			if ( owner->IsFlashlightOn() ) {
				Flashlight ( false );
			} else {
				Flashlight ( true );
			}
			
			SetState ( "Idle", 4 );
			return SRESULT_DONE;
	}
	return SRESULT_ERROR;
}
