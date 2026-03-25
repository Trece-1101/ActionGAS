// Copyright 2020-2026 NiceBug Games All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CC_BlueprintFunctionLib.generated.h"

class ACC_BaseCharacter;
struct FGameplayTag;
struct FGameplayEventData;
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FClosestActorWithTagResult
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ACC_BaseCharacter> Target = nullptr; 
	
	UPROPERTY(BlueprintReadWrite)
	float Distance = 0.0f;
};


UENUM(BlueprintType)
enum class EHitDirection : uint8
{
	Front,
	Back,
	Left,
	Right,
};

UCLASS()
class GAS_COURSE_API UCC_BlueprintFunctionLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure)
	static EHitDirection GetHitDirection(const FVector& TargetForward, const FVector& ToInstigator);
	
	UFUNCTION(BlueprintPure)
	static FName GetHitDirectionName(EHitDirection Direction);
	
	UFUNCTION(BlueprintCallable)
	static FClosestActorWithTagResult FindClosestActorWithTag(const UObject* WorldContext, const FVector& Origin, const FName& Tag, float SearchRange);
	
	UFUNCTION(BlueprintCallable)
	static void SendDamageEventToPlayer(AActor* Target, const TSubclassOf<UGameplayEffect>& DamageEffect, UPARAM(ref) FGameplayEventData& Payload, const FGameplayTag& DataTag, float Damage, const FGameplayTag& EventTagOverride, UObject* OptionalParticleSystem = nullptr);
	
	UFUNCTION(BlueprintCallable)
	static void SendDamageEventToPlayers(TArray<AActor*> Targets, const TSubclassOf<UGameplayEffect>& DamageEffect, UPARAM(ref) FGameplayEventData& Payload, const FGameplayTag& DataTag, float Damage, const FGameplayTag& EventTagOverride, UObject* OptionalParticleSystem = nullptr);
	
	UFUNCTION(BlueprintCallable)
	static TArray<AActor*> HitBoxOverlapTest(AActor* AvatarActor, const float HitBoxForwardOffset = 0.0f,const float HitBoxUpOffset = 0.0f, const float HitBoxRadius = 100.0f, const bool bDrawDebug = false);
	
	static void DrawHitBoxOverlapDebugs(const AActor* ContextActor, TArray<FOverlapResult>& OverlapResults, const FVector& HitBoxLocation, float HitBoxRadius);
	
	UFUNCTION(BlueprintCallable)
	static TArray<AActor*> ApplyKnockback(AActor* AvatarActor, const TArray<AActor*>& HitActors, const float InnerRadius, const float OuterRadius, const float LaunchForceMagnitude, const float RotationAngle, const bool bDrawDebugs);
};
