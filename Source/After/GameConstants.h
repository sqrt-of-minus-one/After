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

extern const float GameTickLength; // The length of game tick
extern const FVector TileSize; // The size of one game tile
extern const float LiquidValueInTile; // How much liquid can be contained in one tile

		/* ENTITY */

extern const float EntityPushDecrement; // How quickly knockback decreases
extern const float EntityOverlapOffsetMultiplier; // Entities repel each other when they overlap
extern const FVector EntitySizeDelta; // The difference between tile size and entity size

		/* MOB */

extern const float MinMobChangeStateTime; // Minimum time for mobs to change state
extern const float MaxMobChangeStateTime; // Maximum time for mobs to change state
extern const float MinMobChangeDirectionTime; // Mobs cannot change their direction more often
extern const float MobUpdateDirectionTime; // How often mobs updates their direction when running away or pursuing
extern const float MobPainTime; // How long mobs feel pain

		/* UNIT */

extern const float UnitDamageInterval; // How often units attack entities
extern const FVector DamageBoxDelta; // The distance between unit edge and its damage box edge

		/* ITEM */

extern const FVector ThrownItemSize; // The size of thrown item
extern const float ItemConditionDecrease; // Decreasing of condition of item that is being used
extern const float WrongItemConditionPenalty; // Condition of items that is used for destroying wrong units is multiplied by this value

}
