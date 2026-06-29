// Copyright 2020-2026 NiceBug Games All Rights Reserved.


#include "AbilitySystem/CC_AbilitySystemComponent.h"

#include "GameplayTags/CC_Tags.h"


UCC_AbilitySystemComponent::UCC_AbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UCC_AbilitySystemComponent::SetAbilityLevel(const TSubclassOf<UGameplayAbility> AbilityClass, int32 NewLevel)
{
	if (!IsOwnerActorAuthoritative()) { return; }
	
	FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromClass(AbilityClass);
	
	if (!AbilitySpec) { return; }
	
	AbilitySpec->Level = NewLevel;
	MarkAbilitySpecDirty(*AbilitySpec);
}

void UCC_AbilitySystemComponent::AddToAbilityLevel(const TSubclassOf<UGameplayAbility> AbilityClass, int32 ValueToAdd)
{
	if (!IsOwnerActorAuthoritative()) { return; }
	
	FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromClass(AbilityClass);
	
	if (!AbilitySpec) { return; }
	
	AbilitySpec->Level += ValueToAdd;
	MarkAbilitySpecDirty(*AbilitySpec);
}

int32 UCC_AbilitySystemComponent::GetAbilityLevelByClass(TSubclassOf<UGameplayAbility> AbilityClass) const
{
	if (!IsValid(AbilityClass))
	{
		return 0;
	}

	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.Ability && Spec.Ability->GetClass() == AbilityClass)
		{
			return Spec.Level;
		}
	}

	return 0;
}

void UCC_AbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnGiveAbility(AbilitySpec);
	
	HandleAutoActivatedAbility(AbilitySpec);
}

void UCC_AbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	
	FScopedAbilityListLock ScopedLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		HandleAutoActivatedAbility(AbilitySpec);
	}
}

void UCC_AbilitySystemComponent::HandleAutoActivatedAbility(const FGameplayAbilitySpec& AbilitySpec)
{
	if (!AbilitySpec.Ability) { return; }
	
	if (AbilitySpec.Ability->GetAssetTags().HasTagExact(CCTags::CCAbilities::ActivateOnGiven))
	{
		TryActivateAbility(AbilitySpec.Handle);
	}
}


