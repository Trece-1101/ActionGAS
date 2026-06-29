// Copyright NiceBug


#include "Player/CC_PlayerState.h"
#include "AbilitySystem/CC_AbilitySystemComponent.h"
#include "AbilitySystem/CC_AttributeSet.h"

ACC_PlayerState::ACC_PlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCC_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	SetNetUpdateFrequency(100.0f);
	
	AttributeSet = CreateDefaultSubobject<UCC_AttributeSet>("AttributeSet");
	
}

UAbilitySystemComponent* ACC_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
