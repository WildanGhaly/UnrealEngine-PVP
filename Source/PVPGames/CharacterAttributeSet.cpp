#include "CharacterAttributeSet.h"
#include <Net/UnrealNetwork.h>
#include "PVPGamesCharacter.h"
#include "Math/UnrealMathUtility.h"

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

void UCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Health, OldHealth);

    if (Health.GetCurrentValue() <= 0)
    {
        HandleDeath();
    }
}

void UCharacterAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Mana, OldMana);
}

void UCharacterAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Stamina, OldStamina);
}

void UCharacterAttributeSet::OnRep_AttackDamage(const FGameplayAttributeData& OldAttackDamage)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, AttackDamage, OldAttackDamage);
}

void UCharacterAttributeSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, AttackSpeed, OldAttackSpeed);
}

void UCharacterAttributeSet::OnRep_Defense(const FGameplayAttributeData& OldDefense)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Defense, OldDefense);
}

void UCharacterAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, MovementSpeed, OldMovementSpeed);
}

void UCharacterAttributeSet::OnRep_Experience(const FGameplayAttributeData& OldExperience)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Experience, OldExperience);
}

void UCharacterAttributeSet::OnRep_Level(const FGameplayAttributeData& OldLevel)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Level, OldLevel);
    InitializeStatsFromLevel();
}

void UCharacterAttributeSet::OnRep_KillCount(const FGameplayAttributeData& OldKillCount)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, KillCount, OldKillCount);
}

void UCharacterAttributeSet::OnRep_DeathCount(const FGameplayAttributeData& OldDeathCount)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, DeathCount, OldDeathCount);
}

void UCharacterAttributeSet::HandleDeath()
{
    // Increment the DeathCount attribute
    DeathCount.SetCurrentValue(DeathCount.GetCurrentValue() + 1);

    // Call OnCharacterDeath on the owning character
    AActor* OwningActor = GetOwningActor();
    if (OwningActor)
    {
        APVPGamesCharacter* Character = Cast<APVPGamesCharacter>(OwningActor);
        if (Character)
        {
            Character->OnCharacterDeath();
        }
    }
}

void UCharacterAttributeSet::AddExperience(float ExperiencePoints)
{
    Experience.SetCurrentValue(Experience.GetCurrentValue() + ExperiencePoints);

    while (Level.GetCurrentValue() < MaxLevel && Experience.GetCurrentValue() >= ExperienceTable[(int32)Level.GetCurrentValue()])
    {
        LevelUp();
    }
}

void UCharacterAttributeSet::LevelUp()
{
    int32 CurrentLevel = (int32)Level.GetCurrentValue();
    if (CurrentLevel >= MaxLevel)
        return;

    const FLevelStats& Stats = LevelStatsTable[CurrentLevel];

    Health.SetBaseValue(Stats.Health);
    Health.SetCurrentValue(Stats.Health);

    Mana.SetBaseValue(Stats.Mana);
    Mana.SetCurrentValue(Stats.Mana);

    Stamina.SetBaseValue(Stats.Stamina);
    Stamina.SetCurrentValue(Stats.Stamina);

    AttackDamage.SetBaseValue((Stats.AttackDamage.X + Stats.AttackDamage.Y) / 2);
    AttackDamage.SetCurrentValue(GetRandomAttackDamage(Stats.AttackDamage));

    AttackSpeed.SetBaseValue(Stats.AttackSpeed);
    AttackSpeed.SetCurrentValue(Stats.AttackSpeed);

    Defense.SetBaseValue(Stats.Defense);
    Defense.SetCurrentValue(Stats.Defense);

    MovementSpeed.SetBaseValue(Stats.MovementSpeed);
    MovementSpeed.SetCurrentValue(Stats.MovementSpeed);

    Level.SetCurrentValue(CurrentLevel + 1);
    Experience.SetCurrentValue(Experience.GetCurrentValue() - ExperienceTable[CurrentLevel]);

    // Additional logic for level up can be added here
}

float UCharacterAttributeSet::GetRandomAttackDamage(FVector2D DamageRange)
{
    return FMath::RandRange(DamageRange.X, DamageRange.Y);
}

void UCharacterAttributeSet::InitializeStatsFromLevel()
{
    int32 CurrentLevel = (int32)Level.GetCurrentValue() - 1;
    if (CurrentLevel < 0 || CurrentLevel >= MaxLevel)
        return;

    const FLevelStats& Stats = LevelStatsTable[CurrentLevel];

    Health.SetBaseValue(Stats.Health);
    Health.SetCurrentValue(Stats.Health);

    Mana.SetBaseValue(Stats.Mana);
    Mana.SetCurrentValue(Stats.Mana);

    Stamina.SetBaseValue(Stats.Stamina);
    Stamina.SetCurrentValue(Stats.Stamina);

    AttackDamage.SetBaseValue((Stats.AttackDamage.X + Stats.AttackDamage.Y) / 2);
    AttackDamage.SetCurrentValue(GetRandomAttackDamage(Stats.AttackDamage));

    AttackSpeed.SetBaseValue(Stats.AttackSpeed);
    AttackSpeed.SetCurrentValue(Stats.AttackSpeed);

    Defense.SetBaseValue(Stats.Defense);
    Defense.SetCurrentValue(Stats.Defense);

    MovementSpeed.SetBaseValue(Stats.MovementSpeed);
    MovementSpeed.SetCurrentValue(Stats.MovementSpeed);
}
