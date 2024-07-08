#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "CharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT(BlueprintType)
struct FLevelStats : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Mana;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Stamina;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D AttackDamage; // X: Min, Y: Max

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Defense;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MovementSpeed;
};

UCLASS()
class PVPGAMES_API UCharacterAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    UCharacterAttributeSet();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes")
    UDataTable* LevelStatsDataTable;

    UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_Health, Category = "Attributes")
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Health);

    UFUNCTION()
    virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

    UFUNCTION(BlueprintImplementableEvent, Category = "Attributes")
    void OnHealthChanged(const FGameplayAttributeData& OldHealth);

    UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_Mana, Category = "Attributes")
    FGameplayAttributeData Mana;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Mana);

    UFUNCTION()
    virtual void OnRep_Mana(const FGameplayAttributeData& OldMana);

    UFUNCTION(BlueprintImplementableEvent, Category = "Attributes")
    void OnManaChanged(const FGameplayAttributeData& OldMana);

    UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_Stamina, Category = "Attributes")
    FGameplayAttributeData Stamina;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Stamina);

    UFUNCTION()
    virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

    UFUNCTION(BlueprintImplementableEvent, Category = "Attributes")
    void OnStaminaChanged(const FGameplayAttributeData& OldStamina);

    UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_AttackDamage, Category = "Attributes")
    FGameplayAttributeData AttackDamage;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, AttackDamage);

    UFUNCTION()
    virtual void OnRep_AttackDamage(const FGameplayAttributeData& OldAttackDamage);

    UFUNCTION(BlueprintImplementableEvent, Category = "Attributes")
    void OnAttackDamageChanged(const FGameplayAttributeData& OldAttackDamage);

    UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_AttackSpeed, Category = "Attributes")
    FGameplayAttributeData AttackSpeed;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, AttackSpeed);

    UFUNCTION()
    virtual void OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed);

    UFUNCTION(BlueprintImplementableEvent, Category = "Attributes")
    void OnAttackSpeedChanged(const FGameplayAttributeData& OldAttackSpeed);

    UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_Defense, Category = "Attributes")
    FGameplayAttributeData Defense;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Defense);

    UFUNCTION()
    virtual void OnRep_Defense(const FGameplayAttributeData& OldDefense);

    UFUNCTION(BlueprintImplementableEvent, Category = "Attributes")
    void OnDefenseChanged(const FGameplayAttributeData& OldDefense);

    UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_MovementSpeed, Category = "Attributes")
    FGameplayAttributeData MovementSpeed;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MovementSpeed);

    UFUNCTION()
    virtual void OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed);

    UFUNCTION(BlueprintImplementableEvent, Category = "Attributes")
    void OnMovementSpeedChanged(const FGameplayAttributeData& OldMovementSpeed);

    UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_Experience, Category = "Attributes")
    FGameplayAttributeData Experience;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Experience);

    UFUNCTION()
    virtual void OnRep_Experience(const FGameplayAttributeData& OldExperience);

    UFUNCTION(BlueprintImplementableEvent, Category = "Attributes")
    void OnExperienceChanged(const FGameplayAttributeData& OldExperience);

    UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_Level, Category = "Attributes")
    FGameplayAttributeData Level;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Level);

    UFUNCTION()
    virtual void OnRep_Level(const FGameplayAttributeData& OldLevel);

    UFUNCTION(BlueprintImplementableEvent, Category = "Attributes")
    void OnLevelChanged(const FGameplayAttributeData& OldLevel);

    UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_KillCount, Category = "Attributes")
    FGameplayAttributeData KillCount;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, KillCount);

    UFUNCTION()
    virtual void OnRep_KillCount(const FGameplayAttributeData& OldKillCount);

    UFUNCTION(BlueprintImplementableEvent, Category = "Attributes")
    void OnKillCountChanged(const FGameplayAttributeData& OldKillCount);

    UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_DeathCount, Category = "Attributes")
    FGameplayAttributeData DeathCount;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, DeathCount);

    UFUNCTION()
    virtual void OnRep_DeathCount(const FGameplayAttributeData& OldDeathCount);

    UFUNCTION(BlueprintImplementableEvent, Category = "Attributes")
    void OnDeathCountChanged(const FGameplayAttributeData& OldDeathCount);

    UFUNCTION(BlueprintCallable, Category = "Attributes")
    void HandleDeath();

    UFUNCTION(BlueprintCallable, Category = "Attributes")
    void AddExperience(float ExperiencePoints);

    UFUNCTION(BlueprintCallable, Category = "Attributes")
    void InitializeStatsFromLevel();

protected:
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

private:
    static const int32 MaxLevel = 10;
    int32 ExperienceTable[MaxLevel] = { 500, 1000, 1600, 2300, 3100, 4000, 5200, 6500, 8000 };

    void LevelUp();
    float GetRandomAttackDamage(FVector2D DamageRange);
};
