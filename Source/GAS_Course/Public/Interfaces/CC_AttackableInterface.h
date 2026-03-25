// Copyright 2020-2026 NiceBug Games All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CC_AttackableInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable, BlueprintType)
class UCC_AttackableInterface : public UInterface
{
	GENERATED_BODY()
};


class GAS_COURSE_API ICC_AttackableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Course | Combat")
	float GetAcceptanceRadius() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Course | Combat")
	void GetAttackDelay(float& OutMinDelay, float& OutMaxDelay) const;
	
};
