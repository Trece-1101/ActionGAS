// Copyright NiceBug


#include "Player/CC_PlayerControler.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Characters/CC_BaseCharacter.h"

void ACC_PlayerControler::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!IsValid(InputSubsystem)) { return; } // If AI or in dedicated server no need to continue

	for (const UInputMappingContext* Context : InputMappingContexts)
	{
		InputSubsystem->AddMappingContext(Context, 0);
	}

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACC_PlayerControler::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACC_PlayerControler::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACC_PlayerControler::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACC_PlayerControler::Look);
	
	// Actions
	for (const auto& InputActionPair : AbilityInputBindings)
	{
		if (InputActionPair.Key)
		{
			EnhancedInputComponent->BindAction(
				InputActionPair.Key,
				ETriggerEvent::Started,
				this,
				&ACC_PlayerControler::OnAbilityInput
				);
		}
	}
}

void ACC_PlayerControler::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	MyCharacter = Cast<ACC_BaseCharacter>(InPawn);
}

void ACC_PlayerControler::OnUnPossess()
{
	Super::OnUnPossess();
	MyCharacter = nullptr;
}

void ACC_PlayerControler::Jump()
{
	if (!IsValid(MyCharacter) || !MyCharacter->GetIsAlive()) { return; }
	MyCharacter->Jump();
}

void ACC_PlayerControler::StopJumping()
{
	if (!IsValid(MyCharacter) || !MyCharacter->GetIsAlive()) { return; }
	MyCharacter->StopJumping();
}

void ACC_PlayerControler::Move(const FInputActionValue& Value)
{
	if (!IsValid(MyCharacter) || !MyCharacter->GetIsAlive()) { return; }
	
	const FVector2D MoveVector = Value.Get<FVector2D>();
	
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
	const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	MyCharacter->AddMovementInput(ForwardDir, MoveVector.Y);
	MyCharacter->AddMovementInput(RightDir, MoveVector.X);
}

void ACC_PlayerControler::Look(const FInputActionValue& Value)
{
	if (!IsValid(MyCharacter) || !MyCharacter->GetIsAlive()) { return; }

	const FVector2D LookVector = Value.Get<FVector2D>();
	AddYawInput(LookVector.X);
	AddPitchInput(LookVector.Y);
}

void ACC_PlayerControler::OnAbilityInput(const FInputActionInstance& Instance)
{
	const UInputAction* Action = Instance.GetSourceAction();
	
	if (const FGameplayTag* Tag = AbilityInputBindings.Find(Action))
	{
		ActivateAbility(*Tag);
	}
}

void ACC_PlayerControler::ActivateAbility(FGameplayTag GameplayTag)
{
	if (!IsValid(MyCharacter) || !MyCharacter->GetIsAlive()) { return; }
	
	if (UAbilitySystemComponent* ASC = MyCharacter->GetAbilitySystemComponent())
	{
		ASC->TryActivateAbilitiesByTag(GameplayTag.GetSingleTagContainer());
	}
}
