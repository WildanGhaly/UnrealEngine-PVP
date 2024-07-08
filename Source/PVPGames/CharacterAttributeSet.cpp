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
    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, AttackDamageMin, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, AttackDamageMax, COND_None, REPNOTIFY_Always);
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
    // Handle logic before an attribute changes
    Super::PreAttributeChange(Attribute, NewValue);

    if (Attribute == GetLevelAttribute())
    {
        // Logic to handle before the Level attribute changes
        // For example, you might want to modify the new value based on some conditions
    }
}

void UCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    AActor* Owner = GetOwningActor();
    APVPGamesCharacter* Character = Cast<APVPGamesCharacter>(Owner);

    if (Character)
    {
        if (Data.EvaluatedData.Attribute == GetHealthAttribute())
        {
            OnHealthChanged(Health.GetCurrentValue());
            Character->OnHealthChanged(Health);
        }
        else if (Data.EvaluatedData.Attribute == GetManaAttribute())
        {
            OnManaChanged(Mana.GetCurrentValue());
            Character->OnManaChanged(Mana);
        }
        else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
        {
            OnStaminaChanged(Stamina.GetCurrentValue());
            Character->OnStaminaChanged(Stamina);
        }
        else if (Data.EvaluatedData.Attribute == GetAttackDamageAttribute())
        {
            OnAttackDamageChanged(AttackDamage.GetCurrentValue());
            Character->OnAttackDamageChanged(AttackDamage);
        }
        else if (Data.EvaluatedData.Attribute == GetAttackDamageMinAttribute())
        {
            OnAttackDamageMinChanged(AttackDamageMin.GetCurrentValue());
            Character->OnAttackDamageMinChanged(AttackDamageMin);
        }
        else if (Data.EvaluatedData.Attribute == GetAttackDamageMaxAttribute())
        {
            OnAttackDamageMaxChanged(AttackDamageMax.GetCurrentValue());
            Character->OnAttackDamageMaxChanged(AttackDamageMax);
        }
        else if (Data.EvaluatedData.Attribute == GetAttackSpeedAttribute())
        {
            OnAttackSpeedChanged(AttackSpeed.GetCurrentValue());
            Character->OnAttackSpeedChanged(AttackSpeed);
        }
        else if (Data.EvaluatedData.Attribute == GetDefenseAttribute())
        {
            OnDefenseChanged(Defense.GetCurrentValue());
            Character->OnDefenseChanged(Defense);
        }
        else if (Data.EvaluatedData.Attribute == GetMovementSpeedAttribute())
        {
            OnMovementSpeedChanged(MovementSpeed.GetCurrentValue());
            Character->OnMovementSpeedChanged(MovementSpeed);
        }
        else if (Data.EvaluatedData.Attribute == GetExperienceAttribute())
        {
            OnExperienceChanged(Experience.GetCurrentValue());

            // Check for level up
            int32 CurrentLevel = static_cast<int32>(Level.GetCurrentValue());
            while (CurrentLevel < MaxLevel && Experience.GetCurrentValue() >= ExperienceTable[CurrentLevel - 1])
            {
                float ExcessExperience = Experience.GetCurrentValue() - ExperienceTable[CurrentLevel - 1];
                LevelUp();
                Experience.SetBaseValue(ExcessExperience);
                Experience.SetCurrentValue(ExcessExperience);
                CurrentLevel = static_cast<int32>(Level.GetCurrentValue());
            }

            // Call the OnExperienceChanged function on the owning character
            Character->OnExperienceChanged(Experience);
        }
        else if (Data.EvaluatedData.Attribute == GetLevelAttribute())
        {
            OnLevelChanged(Level.GetCurrentValue());
            Character->OnLevelChanged(Level);
            InitializeStatsFromLevel();
        }
        else if (Data.EvaluatedData.Attribute == GetKillCountAttribute())
        {
            OnKillCountChanged(KillCount.GetCurrentValue());
            Character->OnKillCountChanged(KillCount);
        }
        else if (Data.EvaluatedData.Attribute == GetDeathCountAttribute())
        {
            OnDeathCountChanged(DeathCount.GetCurrentValue());
            Character->OnDeathCountChanged(DeathCount);
        }
    }
}

void UCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Health, OldHealth);
    OnHealthChanged(OldHealth);

    AActor* Owner = GetOwningActor();
    APVPGamesCharacter* Character = Cast<APVPGamesCharacter>(Owner);
    if (Character)
    {
        Character->OnHealthChanged(OldHealth);
    }
}

void UCharacterAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Mana, OldMana);
    OnManaChanged(OldMana);

    AActor* Owner = GetOwningActor();
    APVPGamesCharacter* Character = Cast<APVPGamesCharacter>(Owner);
    if (Character)
    {
        Character->OnManaChanged(OldMana);
    }
}

void UCharacterAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Stamina, OldStamina);
    OnStaminaChanged(OldStamina);

    AActor* Owner = GetOwningActor();
    APVPGamesCharacter* Character = Cast<APVPGamesCharacter>(Owner);
    if (Character)
    {
        Character->OnStaminaChanged(OldStamina);
    }
}

void UCharacterAttributeSet::OnRep_AttackDamage(const FGameplayAttributeData& OldAttackDamage)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, AttackDamage, OldAttackDamage);
    OnAttackDamageChanged(OldAttackDamage);

    AActor* Owner = GetOwningActor();
    APVPGamesCharacter* Character = Cast<APVPGamesCharacter>(Owner);
    if (Character)
    {
        Character->OnAttackDamageChanged(OldAttackDamage);
    }
}

void UCharacterAttributeSet::OnRep_AttackDamageMin(const FGameplayAttributeData& OldAttackDamageMin)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, AttackDamageMin, OldAttackDamageMin);
    OnAttackDamageMinChanged(OldAttackDamageMin);

    AActor* Owner = GetOwningActor();
    APVPGamesCharacter* Character = Cast<APVPGamesCharacter>(Owner);
    if (Character)
    {
        Character->OnAttackDamageMinChanged(OldAttackDamageMin);
    }
}

void UCharacterAttributeSet::OnRep_AttackDamageMax(const FGameplayAttributeData& OldAttackDamageMax)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, AttackDamageMax, OldAttackDamageMax);
    OnAttackDamageMaxChanged(OldAttackDamageMax);

    AActor* Owner = GetOwningActor();
    APVPGamesCharacter* Character = Cast<APVPGamesCharacter>(Owner);
    if (Character)
    {
        Character->OnAttackDamageMaxChanged(OldAttackDamageMax);
    }
}

void UCharacterAttributeSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, AttackSpeed, OldAttackSpeed);
    OnAttackSpeedChanged(OldAttackSpeed);

    AActor* Owner = GetOwningActor();
    APVPGamesCharacter* Character = Cast<APVPGamesCharacter>(Owner);
    if (Character)
    {
        Character->OnAttackSpeedChanged(OldAttackSpeed);
    }
}

void UCharacterAttributeSet::OnRep_Defense(const FGameplayAttributeData& OldDefense)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Defense, OldDefense);
    OnDefenseChanged(OldDefense);

    AActor* Owner = GetOwningActor();
    APVPGamesCharacter* Character = Cast<APVPGamesCharacter>(Owner);
    if (Character)
    {
        Character->OnDefenseChanged(OldDefense);
    }
}

void UCharacterAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, MovementSpeed, OldMovementSpeed);
    OnMovementSpeedChanged(OldMovementSpeed);

    AActor* Owner = GetOwningActor();
    APVPGamesCharacter* Character = Cast<APVPGamesCharacter>(Owner);
    if (Character)
    {
        Character->OnMovementSpeedChanged(OldMovementSpeed);
    }
}

void UCharacterAttributeSet::OnRep_Experience(const FGameplayAttributeData& OldExperience)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Experience, OldExperience);
    OnExperienceChanged(OldExperience);

    AActor* Owner = GetOwningActor();
    APVPGamesCharacter* Character = Cast<APVPGamesCharacter>(Owner);
    if (Character)
    {
        Character->OnExperienceChanged(OldExperience);
    }
}

void UCharacterAttributeSet::OnRep_Level(const FGameplayAttributeData& OldLevel)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Level, OldLevel);
    OnLevelChanged(OldLevel);
    InitializeStatsFromLevel();

    AActor* Owner = GetOwningActor();
    APVPGamesCharacter* Character = Cast<APVPGamesCharacter>(Owner);
    if (Character)
    {
        Character->OnLevelChanged(OldLevel);
    }
}

void UCharacterAttributeSet::OnRep_KillCount(const FGameplayAttributeData& OldKillCount)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, KillCount, OldKillCount);
    OnKillCountChanged(OldKillCount);

    AActor* Owner = GetOwningActor();
    APVPGamesCharacter* Character = Cast<APVPGamesCharacter>(Owner);
    if (Character)
    {
        Character->OnKillCountChanged(OldKillCount);
    }
}

void UCharacterAttributeSet::OnRep_DeathCount(const FGameplayAttributeData& OldDeathCount)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, DeathCount, OldDeathCount);
    OnDeathCountChanged(OldDeathCount);

    AActor* Owner = GetOwningActor();
    APVPGamesCharacter* Character = Cast<APVPGamesCharacter>(Owner);
    if (Character)
    {
        Character->OnDeathCountChanged(OldDeathCount);
    }
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

    int32 CurrentLevel = Level.GetCurrentValue();
    while (CurrentLevel < MaxLevel && NewExperience >= ExperienceTable[CurrentLevel - 1])
    {
        NewExperience -= ExperienceTable[CurrentLevel - 1];
        LevelUp();
        CurrentLevel = Level.GetCurrentValue();
    }

    // Update the experience to reflect the remaining experience after level-ups
    Experience.SetBaseValue(NewExperience);
    Experience.SetCurrentValue(NewExperience);

    // Call the OnExperienceChanged function on the owning character
    AActor* Owner = GetOwningActor();
    APVPGamesCharacter* Character = Cast<APVPGamesCharacter>(Owner);
    if (Character)
    {
        Character->OnExperienceChanged(Experience);
    }
}


void UCharacterAttributeSet::LevelUp()
{
    int32 CurrentLevel = Level.GetCurrentValue();
    if (CurrentLevel < MaxLevel)
    {
        Level.SetBaseValue(CurrentLevel + 1);
        Level.SetCurrentValue(CurrentLevel + 1);
        InitializeStatsFromLevel();

        // Call the OnLevelChanged function on the owning character
        AActor* Owner = GetOwningActor();
        APVPGamesCharacter* Character = Cast<APVPGamesCharacter>(Owner);
        if (Character)
        {
            Character->OnLevelChanged(Level);
        }
    }
}

void UCharacterAttributeSet::InitializeStatsFromLevel()
{
    int32 CurrentLevel = Level.GetCurrentValue();
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
            AttackDamageMin.SetBaseValue(LevelStats->AttackDamageMin);
            AttackDamageMax.SetBaseValue(LevelStats->AttackDamageMax);
            AttackSpeed.SetBaseValue(LevelStats->AttackSpeed);
            Defense.SetBaseValue(LevelStats->Defense);
            MovementSpeed.SetBaseValue(LevelStats->MovementSpeed);

            Health.SetCurrentValue(LevelStats->Health);
            Mana.SetCurrentValue(LevelStats->Mana);
            Stamina.SetCurrentValue(LevelStats->Stamina);
            AttackDamage.SetCurrentValue(GetRandomAttackDamage(LevelStats->AttackDamage));
            AttackDamageMin.SetCurrentValue(LevelStats->AttackDamageMin);
            AttackDamageMax.SetCurrentValue(LevelStats->AttackDamageMax);
            AttackSpeed.SetCurrentValue(LevelStats->AttackSpeed);
            Defense.SetCurrentValue(LevelStats->Defense);
            MovementSpeed.SetCurrentValue(LevelStats->MovementSpeed);

            // Update ExperienceTable to include ExperienceRequired from the LevelStats
            ExperienceTable[CurrentLevel - 1] = LevelStats->ExperienceRequired;
        }
    }
}


float UCharacterAttributeSet::GetRandomAttackDamage(FVector2D DamageRange)
{
    return FMath::RandRange(DamageRange.X, DamageRange.Y);
}
