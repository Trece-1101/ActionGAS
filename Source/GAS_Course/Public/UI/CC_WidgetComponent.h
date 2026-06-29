// Copyright 2020-2026 NiceBug Games All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Components/WidgetComponent.h"
#include "CC_WidgetComponent.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;
class UCC_AttributeSet;
class UCC_AbilitySystemComponent;
class ACC_BaseCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAS_COURSE_API UCC_WidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UCC_WidgetComponent();
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Course | Attributes")
	TMap<FGameplayAttribute, FGameplayAttribute> AttributeMap;
	
private:
	TWeakObjectPtr<ACC_BaseCharacter> BaseCharacter;
	TWeakObjectPtr<UCC_AbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<UCC_AttributeSet> AttributeSet;
	
	void InitAbilitySystemData();
	
	UFUNCTION()
	void OnAbilitySystemComponentInitialized(UAbilitySystemComponent* ASC, UAttributeSet* AS);
	
	bool IsAbilitySystemComponentInitialized() const;
	void InitializeAttributeDelegate();
	
	UFUNCTION()
	void BindToAttributeChanges();
	
	void BindWidgetToAttributeChanges(UWidget* WidgetObject, const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair) const;
	
};
