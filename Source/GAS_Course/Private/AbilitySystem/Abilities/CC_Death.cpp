// Copyright 2020-2026 NiceBug Games All Rights Reserved.


#include "AbilitySystem/Abilities/CC_Death.h"

UCC_Death::UCC_Death()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}
