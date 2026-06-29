// Copyright 2020-2026 NiceBug Games All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Blueprint/UserWidget.h"
#include "UCC_PlayerHUD.generated.h"

class UCC_AttributeSet;
class UCC_AbilitySystemComponent;

UCLASS()
class GAS_COURSE_API UUCC_PlayerHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void InitializeHUD(UCC_AbilitySystemComponent* ASC, UCC_AttributeSet* AS);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Course | UI")
	TMap<FGameplayAttribute, FGameplayAttribute> AttributeMap;
	
	TWeakObjectPtr<UCC_AbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<UCC_AttributeSet> AttributeSet;

	UFUNCTION()
	void BindToAttributeChanges();
	void BindWidgetToAttributeChanges(UWidget* WidgetObject, const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair) const;
	
private:
	UFUNCTION()
	void OnAbilitySystemComponentInitialized(UAbilitySystemComponent* ASC, UAttributeSet* AS);
	
	bool IsAbilitySystemComponentInitialized() const;
	void InitializeAttributeDelegate();
};
