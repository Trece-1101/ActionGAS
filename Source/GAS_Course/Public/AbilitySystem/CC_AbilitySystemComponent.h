// Copyright 2020-2026 NiceBug Games All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CC_AbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAS_COURSE_API UCC_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UCC_AbilitySystemComponent();
	
	UFUNCTION(BlueprintCallable, Category = "Course | Abilities")
	void SetAbilityLevel(const TSubclassOf<UGameplayAbility> AbilityClass, int32 NewLevel);
	
	UFUNCTION(BlueprintCallable, Category = "Course | Abilities")
	void AddToAbilityLevel(const TSubclassOf<UGameplayAbility> AbilityClass, int32 ValueToAdd = 1);
	
	UFUNCTION(BlueprintPure, Category = "Course | Abilities")
	int32 GetAbilityLevelByClass(TSubclassOf<UGameplayAbility> AbilityClass) const;
	
protected:
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;
	virtual void OnRep_ActivateAbilities() override;
	
private:
	void HandleAutoActivatedAbility(const FGameplayAbilitySpec& AbilitySpec);
};
