// Copyright 2020-2026 NiceBug Games All Rights Reserved.


#include "UI/CC_AttributeWidget.h"

#include "AbilitySystem/CC_AttributeSet.h"

bool UCC_AttributeWidget::MatchesAttributes(const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair) const
{
	return Attribute == Pair.Key && MaxAttribute == Pair.Value;
}

void UCC_AttributeWidget::OnAttributeChanged(const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair,
	const UCC_AttributeSet* AttributeSet, float OldValue)
{
	const float AttributeValue = Pair.Key.GetNumericValue(AttributeSet);
	const float MaxAttributeValue = Pair.Value.GetNumericValue(AttributeSet);
	
	BP_OnAttributeChanged(OldValue, AttributeValue, MaxAttributeValue);
	
}
