// Copyright 2020-2026 NiceBug Games All Rights Reserved.


#include "Utils/CC_BlueprintFunctionLib.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/CC_AttributeSet.h"
#include "Characters/CC_BaseCharacter.h"
#include "Characters/CC_EnemyCharacter.h"
#include "Engine/OverlapResult.h"
#include "GameplayTags/CC_Tags.h"
#include "Kismet/GameplayStatics.h"

EHitDirection UCC_BlueprintFunctionLib::GetHitDirection(const FVector& TargetForward, const FVector& ToInstigator)
{
	const float DotProduct = FVector::DotProduct(TargetForward, ToInstigator);
	
	// Between 0.5 and 1.0 => Front
	if (DotProduct > 0.5f)
	{
		return EHitDirection::Front;
	}
	
	// Between -0.5 and -1.0 => Back
	if (DotProduct < -0.5f)
	{
		return EHitDirection::Back;
	}
	
	const FVector CrossProduct = FVector::CrossProduct(TargetForward, ToInstigator);
	
	// Between -0.5 and 0.5 => Lateral
	// < 0 => Down = Left
	if (CrossProduct.Z < 0.0f)
	{
		return EHitDirection::Left;
	}
	
	// > 0 => Up = Right
	return EHitDirection::Right;
}

FName UCC_BlueprintFunctionLib::GetHitDirectionName(const EHitDirection Direction)
{
	switch (Direction)
	{
		case EHitDirection::Front:
			return FName("Front");
		case EHitDirection::Back:
			return FName("Back");
		case EHitDirection::Left:
			return FName("Left");
		case EHitDirection::Right:
			return FName("Right");
		default:
			return FName("Unknown");
	}
}

FClosestActorWithTagResult UCC_BlueprintFunctionLib::FindClosestActorWithTag(const UObject* WorldContext,
	const FVector& Origin, const FName& Tag, float SearchRange)
{
	const float SearchRangeSqr = SearchRange * SearchRange;
	FClosestActorWithTagResult Result;
	
	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(WorldContext, Tag, ActorsWithTag);
	
	float ClosestDistance = TNumericLimits<float>::Max();
	ACC_BaseCharacter* ClosestActor = nullptr;
	
	for (AActor* PotentialTarget : ActorsWithTag)
	{
		if (!IsValid(PotentialTarget)) { continue; }

		ACC_BaseCharacter* TargetBaseCharacter = Cast<ACC_BaseCharacter>(PotentialTarget);
		if (!IsValid(TargetBaseCharacter) || !TargetBaseCharacter->GetIsAlive()) { continue; }

		const float DistanceSqr = FVector::DistSquared(Origin, TargetBaseCharacter->GetActorLocation());
		
		if (DistanceSqr > SearchRangeSqr) { continue; }
		
		if (DistanceSqr < ClosestDistance)
		{
			ClosestDistance = DistanceSqr;
			ClosestActor = TargetBaseCharacter;
		}
	}
	
	if (ClosestActor)
	{
		Result.Distance = FMath::Sqrt(ClosestDistance);
		Result.Target = ClosestActor;
	}	
	
	return Result;
}

void UCC_BlueprintFunctionLib::SendDamageEventToPlayer(AActor* Target, const TSubclassOf<UGameplayEffect>& DamageEffect,
	FGameplayEventData& Payload, const FGameplayTag& DataTag, float Damage, const FGameplayTag& EventTagOverride, UObject* OptionalParticleSystem)
{
	ACC_BaseCharacter* BaseCharacter = Cast<ACC_BaseCharacter>(Target);
	if (!IsValid(BaseCharacter)) { return; }
	if (!BaseCharacter->GetIsAlive()) { return; }
	
	FGameplayTag EventTag;
	
	if (!EventTagOverride.MatchesTagExact(CCTags::None))
	{
		EventTag = EventTagOverride;
	}
	else
	{
		UCC_AttributeSet* AttributeSet = Cast<UCC_AttributeSet>(BaseCharacter->GetAttributeSet());
		if (!IsValid(AttributeSet)){ return; }
		
		const bool bIsLethalDamage = (AttributeSet->GetHealth() - Damage) <= 0.0f;
		EventTag = bIsLethalDamage ? CCTags::CCEvents::Player::Death : CCTags::CCEvents::Player::HitReact;
	}
	
	Payload.OptionalObject = OptionalParticleSystem;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(BaseCharacter, EventTag, Payload);
	
	UAbilitySystemComponent* TargetASC = BaseCharacter->GetAbilitySystemComponent();
	if (!IsValid(TargetASC)) { return; }
	
	FGameplayEffectContextHandle ContextHandle = TargetASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(DamageEffect, 1.0f, ContextHandle);
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DataTag, -Damage);
	TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void UCC_BlueprintFunctionLib::SendDamageEventToPlayers(TArray<AActor*> Targets,
	const TSubclassOf<UGameplayEffect>& DamageEffect, FGameplayEventData& Payload, const FGameplayTag& DataTag,
	float Damage, const FGameplayTag& EventTagOverride, UObject* OptionalParticleSystem)
{
	for (AActor* Target : Targets)
	{
		SendDamageEventToPlayer(
			Target,
			DamageEffect, 
			Payload,
			DataTag,
			Damage,
			EventTagOverride,
			OptionalParticleSystem
			);
	}
}

TArray<AActor*> UCC_BlueprintFunctionLib::HitBoxOverlapTest(AActor* AvatarActor, const float HitBoxForwardOffset,const float HitBoxUpOffset, const float HitBoxRadius, const bool bDrawDebug)
{
	if (!IsValid(AvatarActor)) { return {}; }
	
	const UWorld* World = AvatarActor->GetWorld();
		
	TArray<FOverlapResult> OverlapResults;
	
	const FVector Forward = AvatarActor->GetActorForwardVector() * HitBoxForwardOffset;
	const FVector Offset = Forward + FVector(0.0f, 0.0f, HitBoxUpOffset);
	const FVector HitBoxLocation = AvatarActor->GetActorLocation() + Offset;

	const FCollisionShape Sphere = FCollisionShape::MakeSphere(HitBoxRadius);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(AvatarActor);
	QueryParams.bTraceComplex = false;
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	
	World->OverlapMultiByObjectType(
		OverlapResults,
		HitBoxLocation,
		FQuat::Identity,
		ObjectQueryParams,
		Sphere,
		QueryParams
		);
	
	TArray<AActor*> HitActors;
	
	for (const FOverlapResult& OverlapResult : OverlapResults)
	{
		AActor* HitActor = OverlapResult.GetActor();
		
		if (IsValid(HitActor))
		{
			if (ACC_BaseCharacter* HitCharacter = Cast<ACC_BaseCharacter>(HitActor))
			{
				if (!HitCharacter->GetIsAlive()) 
				{
					continue; 
				}
			}
			
			HitActors.AddUnique(HitActor);
		}
	}
	
	if (bDrawDebug)
	{
		DrawHitBoxOverlapDebugs(AvatarActor, OverlapResults, HitBoxLocation, HitBoxRadius);
	}
	
	return HitActors;
}

void UCC_BlueprintFunctionLib::DrawHitBoxOverlapDebugs(const AActor* ContextActor, TArray<FOverlapResult>& OverlapResults, const FVector& HitBoxLocation, const float HitBoxRadius)
{
	const UWorld* World = ContextActor->GetWorld();
	
	if (!IsValid(World)) { return; }
	
	const FColor SphereColor = OverlapResults.IsEmpty() ? FColor::Green : FColor::Red;
		
	DrawDebugSphere(World, HitBoxLocation, HitBoxRadius, 16, SphereColor, false, 3.0f);
		
	for (const FOverlapResult& OverlapResult : OverlapResults)
	{
		if (const AActor* HitActor = OverlapResult.GetActor())
		{
			DrawDebugSphere(
				World,
				HitActor->GetActorLocation(),
				50.0f,
				8,
				FColor::Purple,
				false,
				3.0f);
		}
	}
}

TArray<AActor*> UCC_BlueprintFunctionLib::ApplyKnockback(AActor* AvatarActor, const TArray<AActor*>& HitActors,
	const float InnerRadius, const float OuterRadius, const float LaunchForceMagnitude, const float RotationAngle, const bool bDrawDebugs)
{
	if (!IsValid(AvatarActor)) return {};
	const float OuterRadiusSqr = OuterRadius * OuterRadius;
	const float InnerRadiusSqr = InnerRadius * InnerRadius;

	const UWorld* World = AvatarActor->GetWorld();
	
	TArray<AActor*> AffectedActors; 
	
	const FVector AvatarLocation = AvatarActor->GetActorLocation();
	
	for (AActor* HitActor : HitActors)
	{
		ACharacter* HitCharacter = Cast<ACharacter>(HitActor);
		if (!IsValid(HitCharacter)) { continue; }
		
		const FVector HitCharacterLocation = HitCharacter->GetActorLocation();
		const float DistanceSqr = FVector::DistSquared(AvatarLocation, HitCharacterLocation);
		
		if (DistanceSqr > OuterRadiusSqr) { continue; }
		
		float LaunchForce;
		if (DistanceSqr <= InnerRadiusSqr)
		{
			LaunchForce = LaunchForceMagnitude;
		}
		else
		{
			const float RealDistance = FMath::Sqrt(DistanceSqr);
			
			const FVector2D FalloffRange(InnerRadius, OuterRadius);
			const FVector2D LaunchForceRange(LaunchForceMagnitude, 0.f);
			LaunchForce = FMath::GetMappedRangeValueClamped(FalloffRange, LaunchForceRange, RealDistance);
		}
		
		FVector ToHitActor = HitCharacterLocation - AvatarLocation;
		ToHitActor.Z = 0.f; 
		FVector KnockbackDir = ToHitActor.GetSafeNormal();
		
		const FVector Right = KnockbackDir.RotateAngleAxis(90.f, FVector::UpVector);
		FVector FinalKnockbackForce = KnockbackDir.RotateAngleAxis(-RotationAngle, Right) * LaunchForce;
		
		if (bDrawDebugs && IsValid(World))
		{
			DrawDebugDirectionalArrow(World, HitCharacterLocation, HitCharacterLocation + FinalKnockbackForce, 100.f, FColor::Cyan, false, 3.0f);
		}
		
		if (ACC_EnemyCharacter* Enemy = Cast<ACC_EnemyCharacter>(HitActor))
		{
			Enemy->StopMovementUntilLanded();
		}
		
		HitCharacter->LaunchCharacter(FinalKnockbackForce, true, true);
		AffectedActors.Add(HitActor);
	}
	
	return AffectedActors;
}
