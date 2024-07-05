#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class PVPGAMES_API UCharacterAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    UCharacterAttributeSet();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // Health
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Attributes")
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Health);

    UFUNCTION()
    virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

    // Mana
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Attributes")
    FGameplayAttributeData Mana;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Mana);

    UFUNCTION()
    virtual void OnRep_Mana(const FGameplayAttributeData& OldMana);

    // Stamina
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category = "Attributes")
    FGameplayAttributeData Stamina;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Stamina);

    UFUNCTION()
    virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

    // Attack Damage
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackDamage, Category = "Attributes")
    FGameplayAttributeData AttackDamage;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, AttackDamage);

    UFUNCTION()
    virtual void OnRep_AttackDamage(const FGameplayAttributeData& OldAttackDamage);

    // Attack Speed
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackSpeed, Category = "Attributes")
    FGameplayAttributeData AttackSpeed;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, AttackSpeed);

    UFUNCTION()
    virtual void OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed);

    // Defense
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Defense, Category = "Attributes")
    FGameplayAttributeData Defense;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Defense);

    UFUNCTION()
    virtual void OnRep_Defense(const FGameplayAttributeData& OldDefense);

    // Movement Speed
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MovementSpeed, Category = "Attributes")
    FGameplayAttributeData MovementSpeed;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MovementSpeed);

    UFUNCTION()
    virtual void OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed);

    // Experience
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Experience, Category = "Attributes")
    FGameplayAttributeData Experience;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Experience);

    UFUNCTION()
    virtual void OnRep_Experience(const FGameplayAttributeData& OldExperience);

    // Level
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Level, Category = "Attributes")
    FGameplayAttributeData Level;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Level);

    UFUNCTION()
    virtual void OnRep_Level(const FGameplayAttributeData& OldLevel);

    // Kill Count
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_KillCount, Category = "Attributes")
    FGameplayAttributeData KillCount;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, KillCount);

    UFUNCTION()
    virtual void OnRep_KillCount(const FGameplayAttributeData& OldKillCount);

    // Death Count
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_DeathCount, Category = "Attributes")
    FGameplayAttributeData DeathCount;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, DeathCount);

    UFUNCTION()
    virtual void OnRep_DeathCount(const FGameplayAttributeData& OldDeathCount);

    // Handle character's death
    UFUNCTION(BlueprintCallable, Category = "Attributes")
    void HandleDeath();

    // Add experience and handle level up
    UFUNCTION(BlueprintCallable, Category = "Attributes")
    void AddExperience(float ExperiencePoints);

    // Initialize stats based on the current level
    UFUNCTION(BlueprintCallable, Category = "Attributes")
    void InitializeStatsFromLevel();

private:
    static const int32 MaxLevel = 10;
    int32 ExperienceTable[MaxLevel] = { 500, 1000, 1600, 2300, 3100, 4000, 5200, 6500, 8000 };

    struct FLevelStats
    {
        float Health;
        FVector2D AttackDamage; // X: Min, Y: Max
        float AttackSpeed;
        float Defense;
        float MovementSpeed;
        float Mana;
        float Stamina;
    };

    FLevelStats LevelStatsTable[MaxLevel] = {
        { 2700, FVector2D(135, 170), 90, 105, 70, 100, 100 },
        { 2900, FVector2D(138, 175), 90, 108, 70, 110, 110 },
        { 3200, FVector2D(140, 180), 92, 110, 70, 120, 120 },
        { 3400, FVector2D(143, 185), 92, 112, 70, 130, 130 },
        { 3600, FVector2D(148, 190), 92, 114, 72, 140, 140 },
        { 3700, FVector2D(153, 192), 93, 116, 72, 150, 150 },
        { 4000, FVector2D(157, 197), 94, 119, 72, 160, 160 },
        { 4200, FVector2D(164, 200), 95, 122, 72, 170, 170 },
        { 4500, FVector2D(170, 210), 95, 125, 72, 200, 200 }
    };

    void LevelUp();
    float GetRandomAttackDamage(FVector2D DamageRange);
};
