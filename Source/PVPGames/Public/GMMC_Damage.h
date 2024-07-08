// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "GMMC_Damage.generated.h"

/**
 * 
 */
UCLASS()
class PVPGAMES_API UGMMC_Damage : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
	UGMMC_Damage();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& spec) const override;

	FGameplayEffectAttributeCaptureDefinition DefenseDef;
	FGameplayEffectAttributeCaptureDefinition AttackDamageDef;
	
};
