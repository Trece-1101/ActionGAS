// Copyright 2020-2026 NiceBug Games All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Utils/CC_BlueprintFunctionLib.h"
#include "CC_DamageableInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable, BlueprintType)
class UCC_DamageableInterface : public UInterface
{
	GENERATED_BODY()
};


class GAS_COURSE_API ICC_DamageableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Course | Combat")
	void SetLastHitDirection(EHitDirection HitDirection);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Course | Combat")
	EHitDirection GetLastHitDirection() const;
};
