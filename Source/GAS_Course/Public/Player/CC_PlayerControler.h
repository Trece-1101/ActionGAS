// Copyright NiceBug

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "CC_PlayerControler.generated.h"

struct FInputActionInstance;
class ACC_BaseCharacter;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class GAS_COURSE_API ACC_PlayerControler : public APlayerController
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TObjectPtr<ACC_BaseCharacter> MyCharacter;
	
	UPROPERTY(EditAnywhere, Category = "Course|Input")
	TArray<TObjectPtr<UInputMappingContext>> InputMappingContexts;

	UPROPERTY(EditAnywhere, Category = "Course|Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "Course|Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Course|Input")
	TObjectPtr<UInputAction> LookAction;

protected:
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Course|Input|Abilities")
	TMap<UInputAction*, FGameplayTag> AbilityInputBindings;
	
private:
	void Jump();
	void StopJumping();
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void ActivateAbility(FGameplayTag GameplayTag);
	void OnAbilityInput(const FInputActionInstance& Instance);
};
