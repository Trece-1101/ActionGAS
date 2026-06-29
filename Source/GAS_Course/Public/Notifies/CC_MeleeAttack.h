// Copyright 2020-2026 NiceBug Games All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CC_MeleeAttack.generated.h"

UCLASS()
class GAS_COURSE_API UCC_MeleeAttack : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	
private:
	TArray<FHitResult> PerformSphereTrace(USkeletalMeshComponent* MeshComp) const;
	void SendEventsToActors(const USkeletalMeshComponent* MeshComp, const TArray<FHitResult>& Hits) const;
	
	UPROPERTY(EditAnywhere, Category = "Course|Debugs")
	bool bDrawDebugs = true;
	
	UPROPERTY(EditAnywhere, Category = "Course|Socket")
	FName SocketName = "FX_Trail_01_R";
	
	UPROPERTY(EditAnywhere, Category = "Course|Socket")
	float SocketExtensionOffset = 40.0f;
	
	UPROPERTY(EditAnywhere, Category = "Course|Socket")
	float SphereTraceRadius = 60.f;
	
};
