#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemInterface.h"
#include "CharacterAttributeSet.h"
#include "Logging/LogMacros.h"
#include "PVPGamesCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config = Game)
class APVPGamesCharacter : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    // Making AbilitySystemComponent public for Blueprint access
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
    UAbilitySystemComponent* AbilitySystemComponent;

    // Property to hold the class of the AttributeSet
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
    TSubclassOf<UCharacterAttributeSet> AttributeSetClass;

    // Property to hold the instance of the AttributeSet
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
    UCharacterAttributeSet* Attributes;

    // Other properties
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputAction* LookAction;

    APVPGamesCharacter();

    // Ability System Interface Override
    UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    // Game lifecycle events
    virtual void PossessedBy(AController* NewController) override;
    virtual void OnRep_PlayerState() override;
    virtual void InitializeAttributes();
    virtual void GiveDefaultAbilities();

    // Blueprint properties for abilities
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Abilities")
    TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Abilities")
    TArray<TSubclassOf<class UGameplayAbility>> DefaultAbilities;

    // Character death handling
    UFUNCTION(BlueprintCallable, Category = "Character")
    void OnCharacterDeath();

    UFUNCTION(BlueprintImplementableEvent, Category = "Character")
    void OnCharacterDeath_BP();

    // New functions for attribute change events
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attributes")
    void OnHealthChanged(const FGameplayAttributeData& OldHealth);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attributes")
    void OnManaChanged(const FGameplayAttributeData& OldMana);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attributes")
    void OnStaminaChanged(const FGameplayAttributeData& OldStamina);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attributes")
    void OnAttackDamageChanged(const FGameplayAttributeData& OldAttackDamage);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attributes")
    void OnAttackDamageMinChanged(const FGameplayAttributeData& OldAttackDamageMin);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attributes")
    void OnAttackDamageMaxChanged(const FGameplayAttributeData& OldAttackDamageMax);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attributes")
    void OnAttackSpeedChanged(const FGameplayAttributeData& OldAttackSpeed);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attributes")
    void OnDefenseChanged(const FGameplayAttributeData& OldDefense);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attributes")
    void OnMovementSpeedChanged(const FGameplayAttributeData& OldMovementSpeed);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attributes")
    void OnExperienceChanged(const FGameplayAttributeData& OldExperience);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attributes")
    void OnLevelChanged(const FGameplayAttributeData& OldLevel);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attributes")
    void OnKillCountChanged(const FGameplayAttributeData& OldKillCount);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attributes")
    void OnDeathCountChanged(const FGameplayAttributeData& OldDeathCount);

};
