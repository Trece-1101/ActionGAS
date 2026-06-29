// Copyright 2020-2026 NiceBug Games All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CC_GameplayAbility.h"
#include "CC_HitReact.generated.h"


UCLASS()
class GAS_COURSE_API UCC_HitReact : public UCC_GameplayAbility
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable)
	void CacheHitDirectionVectors(AActor* Instigator);
	
	UPROPERTY(BlueprintReadOnly, Category = "Directions")
	FVector AvatarForward;
	
	UPROPERTY(BlueprintReadOnly, Category = "Directions")
	FVector ToInstigator;
	
};
