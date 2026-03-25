#include "Misc/AutomationTest.h"
#include "Utils/CC_BlueprintFunctionLib.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHitDirectionTest, "NiceBug.Combat.HitDirection", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FHitDirectionTest::RunTest(const FString& Parameters)
{
	FVector TargetForward = FVector::ForwardVector;
	
	// Caso Front
	EHitDirection ResultFront = UCC_BlueprintFunctionLib::GetHitDirection(TargetForward, FVector::ForwardVector);
	TestEqual(TEXT("Front Hit"), UCC_BlueprintFunctionLib::GetHitDirectionName(ResultFront).ToString(), FString("Front"));

	// Caso Back
	EHitDirection ResultBack = UCC_BlueprintFunctionLib::GetHitDirection(TargetForward, FVector::BackwardVector);
	TestEqual(TEXT("Back Hit"), UCC_BlueprintFunctionLib::GetHitDirectionName(ResultBack).ToString(), FString("Back"));

	// Caso Right
	EHitDirection ResultRight = UCC_BlueprintFunctionLib::GetHitDirection(TargetForward, FVector::RightVector);
	TestEqual(TEXT("Right Hit"), UCC_BlueprintFunctionLib::GetHitDirectionName(ResultRight).ToString(), FString("Right"));

	// Caso Left
	EHitDirection ResultLeft = UCC_BlueprintFunctionLib::GetHitDirection(TargetForward, FVector::LeftVector);
	TestEqual(TEXT("Left Hit"), UCC_BlueprintFunctionLib::GetHitDirectionName(ResultLeft).ToString(), FString("Left"));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHitDirectionNameTest, "NiceBug.Combat.HitDirectionName", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FHitDirectionNameTest::RunTest(const FString& Parameters)
{
	FName NameFront = UCC_BlueprintFunctionLib::GetHitDirectionName(EHitDirection::Front);
	TestEqual(TEXT("Front Name"), NameFront.ToString(), FString("Front"));
	
	FName NameBack = UCC_BlueprintFunctionLib::GetHitDirectionName(EHitDirection::Back);
	TestEqual(TEXT("Back Name"), NameBack.ToString(), FString("Back"));

	FName NameLeft = UCC_BlueprintFunctionLib::GetHitDirectionName(EHitDirection::Left);
	TestEqual(TEXT("Left Name"), NameLeft.ToString(), FString("Left"));

	FName NameRight = UCC_BlueprintFunctionLib::GetHitDirectionName(EHitDirection::Right);
	TestEqual(TEXT("Right Name"), NameRight.ToString(), FString("Right"));
	
	return true;
}