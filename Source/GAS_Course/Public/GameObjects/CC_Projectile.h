// Copyright 2020-2026 NiceBug Games All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CC_Projectile.generated.h"

class UGameplayEffect;
class UProjectileMovementComponent;

UCLASS()
class GAS_COURSE_API ACC_Projectile : public AActor
{
	GENERATED_BODY()

public:
	ACC_Projectile();
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", ClampMin = "0.0"))
	float Damage;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Course | Projectile")
	void SpawnImpactEffects();
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
	UPROPERTY(EditDefaultsOnly, Category= "Course | Damage")
	TSubclassOf<UGameplayEffect> DamageEffect;
	
		
};
