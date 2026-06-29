// Copyright NiceBug

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "CC_BaseCharacter.generated.h"

namespace GameTags
{
	extern GAS_COURSE_API const FName Player;
}

struct FOnAttributeChangeData;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FASCInitialized, UAbilitySystemComponent*, AbilitySystemComponent, UAttributeSet*, AttributeSet);

UCLASS(Abstract)
class GAS_COURSE_API ACC_BaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ACC_BaseCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UFUNCTION(BlueprintPure, Category = "Course|Attributes")
	virtual UAttributeSet* GetAttributeSet() const { return nullptr; };

	UPROPERTY(BlueprintAssignable, Category = "Course|Effects")
	FASCInitialized OnAbilitySystemComponentInitialized;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	bool GetIsAlive() const { return bIsAlive; }
	void SetIsAlive(const bool bAlive) { bIsAlive = bAlive; }
	
	UPROPERTY(EditAnywhere)
	bool bHasGodMode = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crash AI")
	float SearchRange = 2000.0f;

protected:	
	UPROPERTY(EditDefaultsOnly, Category = "Course|Effects")
	TSubclassOf<UGameplayEffect> InitializeAttributesEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Course|Stats")
	bool bShouldRespawn = false;
	
	void GiveStartupAbilities();
	void InitializeAttributes();
	void OnHealthChanged(const FOnAttributeChangeData& AttributeChangeData);
	
	UFUNCTION(BlueprintCallable)
	virtual void HandleDeath();
	
	UFUNCTION(BlueprintCallable)
	virtual void HandleRespawn();

	UFUNCTION(BlueprintCallable)
	void ResetAttributes();

private:
	void ApplyGameplayEffect(const TSubclassOf<UGameplayEffect>& GE) const;
	
	UPROPERTY(EditDefaultsOnly, Category = "Course|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
	UPROPERTY(EditDefaultsOnly, Category = "Course|Abilities")
	TSubclassOf<UGameplayEffect> ResetAttributesEffect;
	
	UPROPERTY(BlueprintReadOnly, Replicated, meta = (AllowPrivateAccess))
	bool bIsAlive = true;
};