    ////////////////////////////////////////
   //        After by SnegirSoft         //
  //                                    //
 //  File: LastController.h            //
////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"

class AEntity;
class AItem;

DECLARE_DELEGATE_OneParam(FMovingDelegate, float);
DECLARE_DELEGATE(FRunningDelegate);
DECLARE_DELEGATE_RetVal_ThreeParams(bool, FAttackDelegate, AEntity*, bool, AItem*);
