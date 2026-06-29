// Copyright NiceBug


#include "AbilitySystem/Abilities/CC_GameplayAbility.h"

void UCC_GameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (bShowDebug && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::Printf(TEXT("%s activated"), *GetName()));
	}
}
