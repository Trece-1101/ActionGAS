// Copyright NiceBug


#include "GAS_Course/Public/Characters/CC_PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/CC_AttributeSet.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/CC_PlayerState.h"


ACC_PlayerCharacter::ACC_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	Tags.Add(GameTags::Player);

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 600.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
}

UAbilitySystemComponent* ACC_PlayerCharacter::GetAbilitySystemComponent() const
{
	const ACC_PlayerState* CCPlayerState = GetPlayerState<ACC_PlayerState>();
	
	if (!CCPlayerState) { return nullptr; }
	
	return CCPlayerState->GetAbilitySystemComponent();	
}

UAttributeSet* ACC_PlayerCharacter::GetAttributeSet() const
{
	const ACC_PlayerState* CCPlayerState = GetPlayerState<ACC_PlayerState>();

	if (!CCPlayerState) { return nullptr; }
	
	return CCPlayerState->GetAttributeSet();
}

void ACC_PlayerCharacter::PossessedBy(AController* PlayerController)
{
	Super::PossessedBy(PlayerController);
	
	if (!IsValid(GetAbilitySystemComponent()) || !HasAuthority()){ return; }

	GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);
	OnAbilitySystemComponentInitialized.Broadcast(GetAbilitySystemComponent(), GetAttributeSet());
	GiveStartupAbilities();
	InitializeAttributes();
	
	const UCC_AttributeSet* AS = Cast<UCC_AttributeSet>(GetAttributeSet());
	if (AS)
	{
		GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
	}
}

void ACC_PlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (!IsValid(GetAbilitySystemComponent())){ return; }

	GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);
	OnAbilitySystemComponentInitialized.Broadcast(GetAbilitySystemComponent(), GetAttributeSet());
	
	const UCC_AttributeSet* AS = Cast<UCC_AttributeSet>(GetAttributeSet());
	if (AS)
	{
		GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
	}
}

