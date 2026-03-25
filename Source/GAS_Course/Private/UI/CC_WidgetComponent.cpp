// Copyright 2020-2026 NiceBug Games All Rights Reserved.


#include "UI/CC_WidgetComponent.h"

#include "AbilitySystem/CC_AbilitySystemComponent.h"
#include "AbilitySystem/CC_AttributeSet.h"
#include "Blueprint/WidgetTree.h"
#include "Characters/CC_BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UI/CC_AttributeWidget.h"

UCC_WidgetComponent::UCC_WidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UCC_WidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	InitAbilitySystemData();
	
	if (!BaseCharacter.IsValid()) { return; }
	
	if (!IsAbilitySystemComponentInitialized())
	{
		BaseCharacter->OnAbilitySystemComponentInitialized.AddDynamic(this, &ThisClass::OnAbilitySystemComponentInitialized);
		return;
	}
	
	InitializeAttributeDelegate();	
}

void UCC_WidgetComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!GetWorld() || !GetWorld()->IsGameWorld()) { return; }
	
	if (GetNetMode() == NM_DedicatedServer)
	{
		SetComponentTickEnabled(false);
		return; 
	}
	
	const APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	
	if (CameraManager != nullptr)
	{
		FRotator CameraRotation = CameraManager->GetCameraRotation();
		CameraRotation.Pitch = 0.f;
		CameraRotation.Roll = 0.f;
		CameraRotation.Yaw += 180.f;
        
		SetWorldRotation(CameraRotation);
	}
}

bool UCC_WidgetComponent::IsAbilitySystemComponentInitialized() const
{
	return AbilitySystemComponent.IsValid() && AttributeSet.IsValid();
}

void UCC_WidgetComponent::InitializeAttributeDelegate()
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

void UCC_WidgetComponent::InitAbilitySystemData()
{
	BaseCharacter = Cast<ACC_BaseCharacter>(GetOwner());
	
	if (!BaseCharacter.IsValid())
	{
		return;
	}
	
	AttributeSet = Cast<UCC_AttributeSet>(BaseCharacter->GetAttributeSet());
	AbilitySystemComponent = Cast<UCC_AbilitySystemComponent>(BaseCharacter->GetAbilitySystemComponent());
}

void UCC_WidgetComponent::OnAbilitySystemComponentInitialized(UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	AbilitySystemComponent = Cast<UCC_AbilitySystemComponent>(ASC);
	AttributeSet = Cast<UCC_AttributeSet>(AS);
	
	if (!IsAbilitySystemComponentInitialized()) { return; }
	
	InitializeAttributeDelegate();
}

void UCC_WidgetComponent::BindToAttributeChanges()
{
	for (const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair : AttributeMap)
	{
		BindWidgetToAttributeChanges(GetUserWidgetObject(), Pair);
		GetUserWidgetObject()->WidgetTree->ForEachWidget(
			[this, &Pair]
			(UWidget* ChildWidget)
			{
				BindWidgetToAttributeChanges(ChildWidget, Pair);
			});
	}
}

void UCC_WidgetComponent::BindWidgetToAttributeChanges(UWidget* WidgetObject,
	const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair) const
{
	UCC_AttributeWidget* AttributeWidget = Cast<UCC_AttributeWidget>(WidgetObject);
	if (!AttributeWidget) { return; }
	if (!AttributeWidget->MatchesAttributes(Pair)) { return; }
		
	AttributeWidget->OnAttributeChanged(Pair, AttributeSet.Get(), 0.0f);
	AttributeWidget->AvatarActor = BaseCharacter;
	
	TWeakObjectPtr<UCC_AttributeWidget> WeakWidget(AttributeWidget);
	TWeakObjectPtr<UCC_AttributeSet> WeakSet(AttributeSet);
		
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Key).AddLambda(
		[WeakWidget, WeakSet, Pair]
		(const FOnAttributeChangeData& Data)
		{
			if (WeakWidget.IsValid() && WeakSet.IsValid())
			{
			   WeakWidget->OnAttributeChanged(Pair, WeakSet.Get(), Data.OldValue);
			}
		});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value).AddLambda(
	   [WeakWidget, WeakSet, Pair]
	   (const FOnAttributeChangeData& Data)
	   {
		  if (WeakWidget.IsValid() && WeakSet.IsValid())
		  {
			 WeakWidget->OnAttributeChanged(Pair, WeakSet.Get(), Data.OldValue);
		  }
	   });
}
