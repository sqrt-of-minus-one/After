    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: GameConstants.h             //
////////////////////////////////////////

#include "CoreMinimal.h"

namespace GameConstants
{

		/* INPUT */

extern const float ZoomStep; // Distance between the player and his camera is multiplied by this value
extern const float PlayerSpringArmLength; // Default zoom
extern const float MinPlayerSpringArmLength; // Minimum zoom
extern const float MaxPlayerSpringArmLength; // Maximum zoom

		/* WORLD */

extern const FVector TileSize; // The size of one game tile
extern const float LiquidValueInTile; // How much liquid can be contained in one tile

		/* ENTITY */

extern const float CalcStatsInterval; // How often entities calculate their stats
extern const float EntityPushDecrement; // How quickly knockback decreases

		/* MOB */

extern const float MinMobChangeStateTime; // Minimum time for mobs to change state
extern const float MaxMobChangeStateTime; // Maximum time for mobs to change state
extern const float MinMobChangeDirectionTime; // Mobs cannot change their direction more often

		/* UNIT */

extern const float UnitDamageInterval; // How often units attack entities
extern const float UnitDangerInterval; // How often units tell to entities, "I'm dangerous!"
extern const FVector DamageBoxDelta; // The distance between unit edge and its damage box edge

}
