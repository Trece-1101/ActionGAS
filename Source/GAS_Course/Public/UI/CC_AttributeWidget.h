// Copyright 2020-2026 NiceBug Games All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Blueprint/UserWidget.h"
#include "CC_AttributeWidget.generated.h"


class UCC_AttributeSet;

UCLASS()
class GAS_COURSE_API UCC_AttributeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void OnAttributeChanged(const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair, const UCC_AttributeSet* AttributeSet, float OldValue);
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnAttributeChanged"))
	void BP_OnAttributeChanged(float OldValue, float NewValue, float NewMaxValue);
	
	bool MatchesAttributes(const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair) const;

	UPROPERTY(BlueprintReadOnly, Category = "Crash Attributes")
	TWeakObjectPtr<AActor> AvatarActor;
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Course | Attributes")
	FGameplayAttribute Attribute;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Course | Attributes")
	FGameplayAttribute MaxAttribute;
};
