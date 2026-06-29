// Copyright 2020-2026 NiceBug Games All Rights Reserved.


#include "GameObjects/CC_Projectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Characters/CC_PlayerCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameplayTags/CC_Tags.h"
#include "Utils/CC_BlueprintFunctionLib.h"


ACC_Projectile::ACC_Projectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
}

void ACC_Projectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	ACC_PlayerCharacter* PlayerCharacter = Cast<ACC_PlayerCharacter>(OtherActor);
	if (!IsValid(PlayerCharacter)){ return; }
	if (!PlayerCharacter->GetIsAlive()) { return; }
	UAbilitySystemComponent* AbilitySystemComponent = PlayerCharacter->GetAbilitySystemComponent();
	if (!IsValid(AbilitySystemComponent)) { return; }
	
	FGameplayEventData Payload;
	Payload.Instigator = GetOwner();
	Payload.Target = PlayerCharacter;
	
	UCC_BlueprintFunctionLib::SendDamageEventToPlayer(
		PlayerCharacter,
		DamageEffect,
		Payload,
		CCTags::SetByCaller::Projectile,
		Damage,
		CCTags::None
		);
	
	SpawnImpactEffects();
	Destroy();
}


