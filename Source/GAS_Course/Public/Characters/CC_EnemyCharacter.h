// Copyright 2020-2026 NiceBug Games All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CC_BaseCharacter.h"
#include "Interfaces/CC_AttackableInterface.h"
#include "CC_EnemyCharacter.generated.h"

class UAttributeSet;

UCLASS()
class GAS_COURSE_API ACC_EnemyCharacter : public ACC_BaseCharacter, public ICC_AttackableInterface
{
	GENERATED_BODY()

public:
	ACC_EnemyCharacter();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAttributeSet* GetAttributeSet() const override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual float GetAcceptanceRadius_Implementation() const override;
	virtual void GetAttackDelay_Implementation(float& OutMinDelay, float& OutMaxDelay) const override;
		
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void RotateToTarget(AActor* Target);
	
	void StopMovementUntilLanded();

protected:
	virtual void BeginPlay() override;
	virtual void HandleDeath() override;
	virtual void HandleRespawn() override;

	UPROPERTY(EditDefaultsOnly, Category = "Course | Combat")
	float AcceptanceRadius = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Course | Combat")
	float MinAttackDelay = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Course | Combat")
	float MaxAttackDelay = 2.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	bool bIsBeingLaunched = false;

private:
	UPROPERTY(VisibleAnywhere, Category = "Course|Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UFUNCTION()
	void EnableMovementOnLanded(const FHitResult& Hit);
};
