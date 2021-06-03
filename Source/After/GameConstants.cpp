    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: GameConstants.cpp           //
////////////////////////////////////////

#include "GameConstants.h"

		/* INPUT */

const float GameConstants::ZoomStep = 1.1f;
const float GameConstants::PlayerSpringArmLength = 250.f;
const float GameConstants::MinPlayerSpringArmLength = 100.f;
const float GameConstants::MaxPlayerSpringArmLength = 1500.f;

		/* WORLD */

const FVector GameConstants::TileSize = FVector(16.f, 16.f, 4.f);
const float GameConstants::LiquidValueInTile = 100.f;

		/* ENTITY */

const float GameConstants::CalcStatsInterval = 1.f;
const float GameConstants::EntityPushDecrement = 0.85f;
const float GameConstants::EntityOverlapOffsetMultiplier = 15.f;

		/* MOB */

const float GameConstants::MinMobChangeStateTime = 2.f;
const float GameConstants::MaxMobChangeStateTime = 5.f;
const float GameConstants::MinMobChangeDirectionTime = 0.25f;
const float GameConstants::MobUpdateDirectionTime = .3f;
const float GameConstants::MobPainTime = 5.f;

		/* UNIT */

const float GameConstants::UnitDamageInterval = 1.f;
const FVector GameConstants::DamageBoxDelta = FVector(1.f, 1.f, GameConstants::TileSize.Z / 2);

		/* ITEM */

const FVector GameConstants::ThrownItemSize = GameConstants::TileSize / 4.f;
const float GameConstants::ItemConditionDecrease = 1.f;
const float GameConstants::WrongItemConditionPenalty = 2.f;
