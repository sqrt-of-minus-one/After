    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: GameConstants.h             //
////////////////////////////////////////

#include "CoreMinimal.h"

namespace GameConstants
{

		/* INPUT */

extern const float ZoomStep;
extern const float PlayerSpringArmLength;
extern const float MinPlayerSpringArmLength;
extern const float MaxPlayerSpringArmLength;

		/* WORLD */

extern const FVector TileSize;
extern const float LiquidValueInTile;

		/* ENTITY */

extern const float CalcStatsInterval;
extern const float EntityPushDecrement;

		/* MOB */

extern const float MinMobChangeStateTime;
extern const float MaxMobChangeStateTime;

		/* UNIT */

extern const float UnitDamageInterval;
extern const FVector DamageBoxDelta;

}
