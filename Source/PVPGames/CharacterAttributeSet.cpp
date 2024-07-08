#include "CharacterAttributeSet.h"
#include <Net/UnrealNetwork.h>
#include "PVPGamesCharacter.h"
#include "Math/UnrealMathUtility.h"
#include "GameplayEffectExtension.h"

UCharacterAttributeSet::UCharacterAttributeSet()
{
}

void UCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Mana, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, AttackDamage, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, AttackSpeed, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Defense, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Experience, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Level, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, KillCount, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, DeathCount, COND_None, REPNOTIFY_Always);
}

void UCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    Super::PreAttributeChange(Attribute, NewValue);

    if (Attribute == GetLevelAttribute())
    {
        // Logic to handle before the Level attribute changes
    }
}

void UCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        OnHealthChanged(Health.GetCurrentValue());
    }
    else if (Data.EvaluatedData.Attribute == GetManaAttribute())
    {
        OnManaChanged(Mana.GetCurrentValue());
    }
    else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
    {
        OnStaminaChanged(Stamina.GetCurrentValue());
    }
    else if (Data.EvaluatedData.Attribute == GetAttackDamageAttribute())
    {
        OnAttackDamageChanged(AttackDamage.GetCurrentValue());
    }
    else if (Data.EvaluatedData.Attribute == GetAttackSpeedAttribute())
    {
        OnAttackSpeedChanged(AttackSpeed.GetCurrentValue());
    }
    else if (Data.EvaluatedData.Attribute == GetDefenseAttribute())
    {
        OnDefenseChanged(Defense.GetCurrentValue());
    }
    else if (Data.EvaluatedData.Attribute == GetMovementSpeedAttribute())
    {
        OnMovementSpeedChanged(MovementSpeed.GetCurrentValue());
    }
    else if (Data.EvaluatedData.Attribute == GetExperienceAttribute())
    {
        OnExperienceChanged(Experience.GetCurrentValue());
        // Check for level up
        int32 CurrentLevel = static_cast<int32>(Level.GetCurrentValue());
        if (Experience.GetCurrentValue() >= ExperienceTable[CurrentLevel])
        {
            LevelUp();
        }
    }
    else if (Data.EvaluatedData.Attribute == GetLevelAttribute())
    {
        OnLevelChanged(Level.GetCurrentValue());
        InitializeStatsFromLevel();
    }
    else if (Data.EvaluatedData.Attribute == GetKillCountAttribute())
    {
        OnKillCountChanged(KillCount.GetCurrentValue());
    }
    else if (Data.EvaluatedData.Attribute == GetDeathCountAttribute())
    {
        OnDeathCountChanged(DeathCount.GetCurrentValue());
    }
}

void UCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Health, OldHealth);
    OnHealthChanged(OldHealth);
}

void UCharacterAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Mana, OldMana);
    OnManaChanged(OldMana);
}

void UCharacterAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Stamina, OldStamina);
    OnStaminaChanged(OldStamina);
}

void UCharacterAttributeSet::OnRep_AttackDamage(const FGameplayAttributeData& OldAttackDamage)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, AttackDamage, OldAttackDamage);
    OnAttackDamageChanged(OldAttackDamage);
}

void UCharacterAttributeSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, AttackSpeed, OldAttackSpeed);
    OnAttackSpeedChanged(OldAttackSpeed);
}

void UCharacterAttributeSet::OnRep_Defense(const FGameplayAttributeData& OldDefense)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Defense, OldDefense);
    OnDefenseChanged(OldDefense);
}

void UCharacterAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, MovementSpeed, OldMovementSpeed);
    OnMovementSpeedChanged(OldMovementSpeed);
}

void UCharacterAttributeSet::OnRep_Experience(const FGameplayAttributeData& OldExperience)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Experience, OldExperience);
    OnExperienceChanged(OldExperience);
}

void UCharacterAttributeSet::OnRep_Level(const FGameplayAttributeData& OldLevel)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Level, OldLevel);
    OnLevelChanged(OldLevel);
    InitializeStatsFromLevel();
}

void UCharacterAttributeSet::OnRep_KillCount(const FGameplayAttributeData& OldKillCount)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, KillCount, OldKillCount);
    OnKillCountChanged(OldKillCount);
}

void UCharacterAttributeSet::OnRep_DeathCount(const FGameplayAttributeData& OldDeathCount)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, DeathCount, OldDeathCount);
    OnDeathCountChanged(OldDeathCount);
}

void UCharacterAttributeSet::HandleDeath()
{
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwningActor());
    if (OwnerCharacter)
    {
        APVPGamesCharacter* Character = Cast<APVPGamesCharacter>(OwnerCharacter);
        if (Character)
        {
            Character->OnCharacterDeath_BP();
        }
    }
}

void UCharacterAttributeSet::AddExperience(float ExperiencePoints)
{
    float OldExperience = Experience.GetCurrentValue();
    float NewExperience = OldExperience + ExperiencePoints;
    Experience.SetBaseValue(NewExperience);
    Experience.SetCurrentValue(NewExperience);

    OnExperienceChanged(OldExperience);

    int32 CurrentLevel = static_cast<int32>(Level.GetCurrentValue());
    if (CurrentLevel < MaxLevel && NewExperience >= ExperienceTable[CurrentLevel])
    {
        LevelUp();
    }
}

void UCharacterAttributeSet::LevelUp()
{
    int32 CurrentLevel = static_cast<int32>(Level.GetCurrentValue());
    if (CurrentLevel < MaxLevel)
    {
        Level.SetBaseValue(CurrentLevel + 1);
        Level.SetCurrentValue(CurrentLevel + 1);
        InitializeStatsFromLevel();
    }
}

void UCharacterAttributeSet::InitializeStatsFromLevel()
{
    int32 CurrentLevel = static_cast<int32>(Level.GetCurrentValue());
    if (LevelStatsDataTable)
    {
        FString ContextString;
        FName RowName = FName(*FString::FromInt(CurrentLevel));
        FLevelStats* LevelStats = LevelStatsDataTable->FindRow<FLevelStats>(RowName, ContextString);
        if (LevelStats)
        {
            Health.SetBaseValue(LevelStats->Health);
            Mana.SetBaseValue(LevelStats->Mana);
            Stamina.SetBaseValue(LevelStats->Stamina);
            AttackDamage.SetBaseValue(GetRandomAttackDamage(LevelStats->AttackDamage));
            AttackSpeed.SetBaseValue(LevelStats->AttackSpeed);
            Defense.SetBaseValue(LevelStats->Defense);
            MovementSpeed.SetBaseValue(LevelStats->MovementSpeed);

            Health.SetCurrentValue(LevelStats->Health);
            Mana.SetCurrentValue(LevelStats->Mana);
            Stamina.SetCurrentValue(LevelStats->Stamina);
            AttackDamage.SetCurrentValue(GetRandomAttackDamage(LevelStats->AttackDamage));
            AttackSpeed.SetCurrentValue(LevelStats->AttackSpeed);
            Defense.SetCurrentValue(LevelStats->Defense);
            MovementSpeed.SetCurrentValue(LevelStats->MovementSpeed);
        }
    }
}

float UCharacterAttributeSet::GetRandomAttackDamage(FVector2D DamageRange)
{
    return FMath::RandRange(DamageRange.X, DamageRange.Y);
}
