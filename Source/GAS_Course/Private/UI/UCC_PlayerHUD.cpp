// Copyright 2020-2026 NiceBug Games All Rights Reserved.


#include "UI/UCC_PlayerHUD.h"

#include "AbilitySystem/CC_AbilitySystemComponent.h"
#include "Blueprint/WidgetTree.h"
#include "UI/CC_AttributeWidget.h"
#include "AbilitySystem/CC_AttributeSet.h"

void UUCC_PlayerHUD::InitializeHUD(UCC_AbilitySystemComponent* ASC, UCC_AttributeSet* AS)
{
	if (!ASC || !AS) return;

	AbilitySystemComponent = ASC;
	AttributeSet = AS;

	BindToAttributeChanges();
}

void UUCC_PlayerHUD::BindToAttributeChanges()
{
	for (const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair : AttributeMap)
	{
		BindWidgetToAttributeChanges(this, Pair);
        
		if (WidgetTree)
		{
			WidgetTree->ForEachWidget(
				[this, &Pair](UWidget* ChildWidget)
				{
					BindWidgetToAttributeChanges(ChildWidget, Pair);
				});
		}
	}
}

void UUCC_PlayerHUD::BindWidgetToAttributeChanges(UWidget* WidgetObject,
	const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair) const
{
	UCC_AttributeWidget* AttributeWidget = Cast<UCC_AttributeWidget>(WidgetObject);
	if (!AttributeWidget) { return; }
	if (!AttributeWidget->MatchesAttributes(Pair)) { return; }
        
	AttributeWidget->OnAttributeChanged(Pair, AttributeSet.Get(), 0.0f);
    
	TWeakObjectPtr<UCC_AttributeWidget> WeakWidget(AttributeWidget);
	TWeakObjectPtr<UCC_AttributeSet> WeakSet(AttributeSet.Get());
        
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Key).AddLambda(
		[WeakWidget, WeakSet, Pair](const FOnAttributeChangeData& Data)
		{
			if (WeakWidget.IsValid() && WeakSet.IsValid())
			{
			   WeakWidget->OnAttributeChanged(Pair, WeakSet.Get(), Data.OldValue);
			}
		});
    
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value).AddLambda(
	   [WeakWidget, WeakSet, Pair](const FOnAttributeChangeData& Data)
	   {
		  if (WeakWidget.IsValid() && WeakSet.IsValid())
		  {
			 WeakWidget->OnAttributeChanged(Pair, WeakSet.Get(), Data.OldValue);
		  }
	   });
}

void UUCC_PlayerHUD::OnAbilitySystemComponentInitialized(UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	AbilitySystemComponent = Cast<UCC_AbilitySystemComponent>(ASC);
	AttributeSet = Cast<UCC_AttributeSet>(AS);
	
	if (!IsAbilitySystemComponentInitialized()) { return; }
	
	InitializeAttributeDelegate();
}

bool UUCC_PlayerHUD::IsAbilitySystemComponentInitialized() const
{
	return AbilitySystemComponent.IsValid() && AttributeSet.IsValid();
}

void UUCC_PlayerHUD::InitializeAttributeDelegate()
{
	if (!AttributeSet->bAttributesInitialized)
	{
		AttributeSet->OnAttributeInitialized.AddDynamic(this, &ThisClass::BindToAttributeChanges);
	}
	else
	{
		BindToAttributeChanges();
	}
}
