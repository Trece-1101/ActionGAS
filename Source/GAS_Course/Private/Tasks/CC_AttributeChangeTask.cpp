// Copyright 2020-2026 NiceBug Games All Rights Reserved.


#include "Tasks/CC_AttributeChangeTask.h"

#include "AbilitySystemComponent.h"

void UCC_AttributeChangeTask::Activate()
{
	Super::Activate();
	
	if (!ASC.IsValid())
	{
		EndTask();
		return;
	}
    
	ASC->GetGameplayAttributeValueChangeDelegate(AttributeListenFor).AddUObject(this, &UCC_AttributeChangeTask::AttributeChanged);
}

UCC_AttributeChangeTask* UCC_AttributeChangeTask::ListenForAttributeChange(
	UAbilitySystemComponent* AbilitySystemComponent, const FGameplayAttribute Attribute)
{
	UCC_AttributeChangeTask* Task = NewObject<UCC_AttributeChangeTask>();
	Task->ASC = AbilitySystemComponent;
	Task->AttributeListenFor = Attribute;
	
	return Task;
}

void UCC_AttributeChangeTask::AttributeChanged(const FOnAttributeChangeData& Data)
{
	OnAttributeChanged.Broadcast(Data.Attribute, Data.NewValue, Data.OldValue);
}

void UCC_AttributeChangeTask::EndTask()
{
	if (ASC.IsValid())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(AttributeListenFor).RemoveAll(this);
	}
	
	SetReadyToDestroy();
}
