// Fill out your copyright notice in the Description page of Project Settings.


#include "GMMC_Damage.h"
#include "PVPGames/CharacterAttributeSet.h"

UGMMC_Damage::UGMMC_Damage() 
{
	DefenseDef.AttributeToCapture = UCharacterAttributeSet::GetDefenseAttribute();
	DefenseDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	DefenseDef.bSnapshot = false;

	AttackDamageDef.AttributeToCapture = UCharacterAttributeSet::GetAttackDamageAttribute();
	AttackDamageDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	AttackDamageDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(DefenseDef);
	RelevantAttributesToCapture.Add(AttackDamageDef);
}

float UGMMC_Damage::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& spec) const
{
	const FGameplayTagContainer* SourceTags = spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Defense = 0.0f;
	GetCapturedAttributeMagnitude(DefenseDef, spec, EvaluationParameters, Defense);

	float AttackDamage = 0.0f;
	GetCapturedAttributeMagnitude(AttackDamageDef, spec, EvaluationParameters, AttackDamage);

	float damage = (AttackDamage / (AttackDamage + Defense)) * AttackDamage;
	UE_LOG(LogTemp, Log, TEXT("Attack Damage: %f"), AttackDamage);
	UE_LOG(LogTemp, Log, TEXT("Defense: %f"), Defense);
	UE_LOG(LogTemp, Log, TEXT("Calculated Damage: %f"), damage);

	return damage;
}
