// Copyright 2020-2026 NiceBug Games All Rights Reserved.


#include "GAS_Course/Public/Characters/CC_EnemyCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/CC_AbilitySystemComponent.h"
#include "AbilitySystem/CC_AttributeSet.h"
#include "GameplayTags/CC_Tags.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Utils/CC_BlueprintFunctionLib.h"


ACC_EnemyCharacter::ACC_EnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	AbilitySystemComponent = CreateDefaultSubobject<UCC_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UCC_AttributeSet>(FName("AttributeSet"));
}

UAbilitySystemComponent* ACC_EnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* ACC_EnemyCharacter::GetAttributeSet() const
{
	return AttributeSet;
}

void ACC_EnemyCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, bIsBeingLaunched);
}

float ACC_EnemyCharacter::GetAcceptanceRadius_Implementation() const
{
	return AcceptanceRadius;
}

void ACC_EnemyCharacter::GetAttackDelay_Implementation(float& OutMinDelay, float& OutMaxDelay) const
{
	OutMinDelay = MinAttackDelay;
	OutMaxDelay = MaxAttackDelay;
}

void ACC_EnemyCharacter::StopMovementUntilLanded()
{
	bIsBeingLaunched = true;
	AAIController* AIController = GetController<AAIController>();	
	if (!AIController) { return; }	
	AIController->StopMovement();
	
	if (!LandedDelegate.IsAlreadyBound(this, &ThisClass::EnableMovementOnLanded))
	{
		LandedDelegate.AddDynamic(this, &ThisClass::EnableMovementOnLanded);
	}
}

void ACC_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsValid(AbilitySystemComponent)) { return; }
	
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	OnAbilitySystemComponentInitialized.Broadcast(GetAbilitySystemComponent(), GetAttributeSet());
	
	if (HasAuthority())
	{
		GiveStartupAbilities();
		InitializeAttributes();

		const UCC_AttributeSet* AS = Cast<UCC_AttributeSet>(GetAttributeSet());
		if (AS)
		{
			GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
		}
	}
}

void ACC_EnemyCharacter::HandleDeath()
{
	Super::HandleDeath();
	
	AAIController* AIController = GetController<AAIController>();	
	if (!AIController) { return; }	
	AIController->StopMovement();
}

void ACC_EnemyCharacter::HandleRespawn()
{
	Super::HandleRespawn();
	
	bIsBeingLaunched = false;
}

void ACC_EnemyCharacter::EnableMovementOnLanded(const FHitResult& Hit)
{
	bIsBeingLaunched = false;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, CCTags::Events::Enemy::EndAttack, FGameplayEventData());
	LandedDelegate.RemoveAll(this);
}

