// Copyright 2020-2026 NiceBug Games All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CC_GameplayAbility.h"
#include "CC_Primary.generated.h"

UCLASS()
class GAS_COURSE_API UCC_Primary : public UCC_GameplayAbility
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Course | HitBox")
	float HitBoxRadius = 100.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Course | HitBox")
	float HitBoxForwardOffset = 200.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Course | HitBox")
	float HitBoxUpOffset = 20.0f;
	
private:
	// UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	// TArray<AActor*> HitBoxOverlapTest();
	
	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void SendHitEventToActors(const TArray<AActor*>& ActorsHit);
	
	// void DrawHitBoxOverlapDebugs(const UWorld* World, TArray<FOverlapResult>& OverlapResults, const FVector& HitBoxLocation) const;
};
