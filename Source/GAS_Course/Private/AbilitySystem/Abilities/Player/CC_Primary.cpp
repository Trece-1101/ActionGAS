// Copyright 2020-2026 NiceBug Games All Rights Reserved.


#include "AbilitySystem/Abilities/Player/CC_Primary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Engine/OverlapResult.h"
#include "GameplayTags/CC_Tags.h"


// TArray<AActor*> UCC_Primary::HitBoxOverlapTest()
// {
// 	const AActor* AvatarActor = GetAvatarActorFromActorInfo();
// 	const UWorld* World = GetWorld();
// 	
// 	if (!AvatarActor || !World) { return {}; }
// 	
// 	TArray<FOverlapResult> OverlapResults;
// 	
// 	const FVector Forward = AvatarActor->GetActorForwardVector() * HitBoxForwardOffset;
// 	const FVector Offset = Forward + FVector(0.0f, 0.0f, HitBoxUpOffset);
// 	const FVector HitBoxLocation = AvatarActor->GetActorLocation() + Offset;
//
// 	const FCollisionShape Sphere = FCollisionShape::MakeSphere(HitBoxRadius);
//
// 	FCollisionQueryParams QueryParams;
// 	QueryParams.AddIgnoredActor(AvatarActor);
// 	QueryParams.bTraceComplex = false;
// 	
// 	FCollisionObjectQueryParams ObjectQueryParams;
// 	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
// 	
// 	World->OverlapMultiByObjectType(
// 		OverlapResults,
// 		HitBoxLocation,
// 		FQuat::Identity,
// 		ObjectQueryParams,
// 		Sphere,
// 		QueryParams
// 		);
// 	
// 	TArray<AActor*> HitActors;
// 	
// 	for (const FOverlapResult& OverlapResult : OverlapResults)
// 	{
// 		if (AActor* HitActor = OverlapResult.GetActor())
// 		{
// 			HitActors.AddUnique(HitActor);
// 		}
// 	}
// 	
// 	if (bShowDebug)
// 	{
// 		DrawHitBoxOverlapDebugs(World, OverlapResults, HitBoxLocation);
// 	}
// 	
// 	return HitActors;
// }

void UCC_Primary::SendHitEventToActors(const TArray<AActor*>& ActorsHit)
{
	for (AActor* HitActor : ActorsHit)
	{
		FGameplayEventData Payload;
		Payload.Instigator = GetAvatarActorFromActorInfo();
		Payload.Target = HitActor;
		
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, CCTags::CCEvents::Enemy::HitReact, Payload);
	}
}

// void UCC_Primary::DrawHitBoxOverlapDebugs(const UWorld* World, TArray<FOverlapResult>& OverlapResults, const FVector& HitBoxLocation) const
// {
// 	const FColor SphereColor = OverlapResults.IsEmpty() ? FColor::Green : FColor::Red;
// 		
// 	DrawDebugSphere(World, HitBoxLocation, HitBoxRadius, 16, SphereColor, false, 3.0f);
// 		
// 	for (const FOverlapResult& OverlapResult : OverlapResults)
// 	{
// 		if (const AActor* HitActor = OverlapResult.GetActor())
// 		{
// 			DrawDebugSphere(
// 				World,
// 				HitActor->GetActorLocation(),
// 				HitBoxRadius / 2.0f,
// 				16,
// 				FColor::Purple,
// 				false,
// 				3.0f);
// 		}
// 	}
// }
