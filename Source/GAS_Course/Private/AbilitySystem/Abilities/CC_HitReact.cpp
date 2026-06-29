// Copyright 2020-2026 NiceBug Games All Rights Reserved.


#include "AbilitySystem/Abilities/CC_HitReact.h"

void UCC_HitReact::CacheHitDirectionVectors(AActor* Instigator)
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	
	AvatarForward = Avatar->GetActorForwardVector();
	
	ToInstigator = Instigator->GetActorLocation() - Avatar->GetActorLocation();
	ToInstigator.Normalize();
}
