// Copyright 2020-2026 NiceBug Games All Rights Reserved.


#include "Notifies/CC_MeleeAttack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "KismetTraceUtils.h"
#include "Characters/CC_BaseCharacter.h"
#include "GameplayTags/CC_Tags.h"
#include "Kismet/KismetMathLibrary.h"

void UCC_MeleeAttack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
                                 const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	if (!IsValid(MeshComp)) return;
	if (!IsValid(MeshComp->GetOwner())) return;

	const TArray<FHitResult> Hits = PerformSphereTrace(MeshComp);
	SendEventsToActors(MeshComp, Hits);
}

TArray<FHitResult> UCC_MeleeAttack::PerformSphereTrace(USkeletalMeshComponent* MeshComp) const
{
	TArray<FHitResult> OutHits;
	
	const FTransform SocketTransform = MeshComp->GetSocketTransform(SocketName);
	const FVector Start = SocketTransform.GetLocation();
	//const FVector ExtendedSocketDirection = UKismetMathLibrary::GetForwardVector(SocketTransform.GetRotation().Rotator()) * SocketExtensionOffset;
	const FVector ExtendedSocketDirection = SocketTransform.GetRotation().GetForwardVector() * SocketExtensionOffset;
	const FVector End = Start - ExtendedSocketDirection;
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(MeshComp->GetOwner());
	
	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block);
	
	//UWorld* World = GEngine->GetWorldFromContextObject(MeshComp, EGetWorldErrorMode::LogAndReturnNull);
	UWorld* World = MeshComp->GetWorld();
	if (!IsValid(World)) { return OutHits; }
	
	bool const bHit = World->SweepMultiByChannel(
		OutHits,
		Start,
		End,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(SphereTraceRadius),
		Params,
		ResponseParams
		);
	
	if (bDrawDebugs)
	{
		DrawDebugSphereTraceMulti(
			World,
			Start,
			End,
			SphereTraceRadius,
			EDrawDebugTrace::ForDuration,
			bHit,
			OutHits,
			FColor::Red,
			FColor::Green,
			5.f);
	}
	
	return OutHits;
}

void UCC_MeleeAttack::SendEventsToActors(const USkeletalMeshComponent* MeshComp, const TArray<FHitResult>& Hits) const
{
	for (const FHitResult HitResult : Hits)
	{
		ACC_BaseCharacter* HitCharacter = Cast<ACC_BaseCharacter>(HitResult.GetActor());
		
		if (!IsValid(HitCharacter)) { continue; }
		if (!HitCharacter->GetIsAlive()) { continue; }

		const UAbilitySystemComponent* ASC = HitCharacter->GetAbilitySystemComponent();
		if (!IsValid(ASC)) { continue; }
		
		FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
		ContextHandle.AddHitResult(HitResult);
		
		FGameplayEventData Payload;
		Payload.Target = HitCharacter;
		Payload.ContextHandle = ContextHandle;
		Payload.Instigator = MeshComp->GetOwner();
		
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			MeshComp->GetOwner(),
			CCTags::CCEvents::Enemy::MeleeTraceHit,
			Payload
			);
	}
}
