#include "PVPGamesCharacter.h"
#include "Engine/LocalPlayer.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributeSet.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// APVPGamesCharacter

APVPGamesCharacter::APVPGamesCharacter()
{
    AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComp");
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

    // Initialize attributes in the constructor
    Attributes = nullptr;

    // Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
    // are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

UAbilitySystemComponent* APVPGamesCharacter::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

void APVPGamesCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    if (AbilitySystemComponent) {
        AbilitySystemComponent->InitAbilityActorInfo(this, this);
    }

    // Initialize attributes based on the selected class
    if (AttributeSetClass)
    {
        Attributes = NewObject<UCharacterAttributeSet>(this, AttributeSetClass);
        if (Attributes)
        {
            AbilitySystemComponent->AddAttributeSetSubobject(Attributes);
            Attributes->InitializeStatsFromLevel();
        }
    }

    InitializeAttributes();
    GiveDefaultAbilities();
}

void APVPGamesCharacter::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();

    if (AbilitySystemComponent) {
        AbilitySystemComponent->InitAbilityActorInfo(this, this);
    }

    InitializeAttributes();
}

void APVPGamesCharacter::InitializeAttributes()
{
    if (AbilitySystemComponent && DefaultAttributeEffect) {
        FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
        EffectContext.AddSourceObject(this);
        FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);

        if (SpecHandle.IsValid()) {
            FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        }
    }
}

void APVPGamesCharacter::GiveDefaultAbilities()
{
    if (HasAuthority() && AbilitySystemComponent)
    {
        for (TSubclassOf<UGameplayAbility>& StartUpAbility : DefaultAbilities)
        {
            AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartUpAbility.GetDefaultObject(), 1, 0));
        }
    }
}

void APVPGamesCharacter::OnCharacterDeath()
{
    // Logic to handle character death in C++
    UE_LOG(LogTemplateCharacter, Log, TEXT("Character has died."));

    // Call the Blueprint version of this function
    OnCharacterDeath_BP();
}

// Implementations for the new attribute change functions
void APVPGamesCharacter::OnHealthChanged_Implementation(const FGameplayAttributeData& OldHealth) {
    UE_LOG(LogTemplateCharacter, Log, TEXT("Health changed."));
}

void APVPGamesCharacter::OnManaChanged_Implementation(const FGameplayAttributeData& OldMana) {
    UE_LOG(LogTemplateCharacter, Log, TEXT("Mana changed."));
}

void APVPGamesCharacter::OnStaminaChanged_Implementation(const FGameplayAttributeData& OldStamina) {
    UE_LOG(LogTemplateCharacter, Log, TEXT("Stamina changed."));
}

void APVPGamesCharacter::OnAttackDamageChanged_Implementation(const FGameplayAttributeData& OldAttackDamage) {
    UE_LOG(LogTemplateCharacter, Log, TEXT("Attack Damage changed."));
}

void APVPGamesCharacter::OnAttackDamageMinChanged_Implementation(const FGameplayAttributeData& OldAttackDamageMin) {
    UE_LOG(LogTemplateCharacter, Log, TEXT("Attack Damage Min changed."));
}

void APVPGamesCharacter::OnAttackDamageMaxChanged_Implementation(const FGameplayAttributeData& OldAttackDamageMax) {
    UE_LOG(LogTemplateCharacter, Log, TEXT("Attack Damage Max changed."));
}

void APVPGamesCharacter::OnAttackSpeedChanged_Implementation(const FGameplayAttributeData& OldAttackSpeed) {
    UE_LOG(LogTemplateCharacter, Log, TEXT("Attack Speed changed."));
}

void APVPGamesCharacter::OnDefenseChanged_Implementation(const FGameplayAttributeData& OldDefense) {
    UE_LOG(LogTemplateCharacter, Log, TEXT("Defense changed."));
}

void APVPGamesCharacter::OnMovementSpeedChanged_Implementation(const FGameplayAttributeData& OldMovementSpeed) {
    UE_LOG(LogTemplateCharacter, Log, TEXT("Movement Speed changed."));
}

void APVPGamesCharacter::OnExperienceChanged_Implementation(const FGameplayAttributeData& OldExperience) {
    UE_LOG(LogTemplateCharacter, Log, TEXT("Experience changed."));
}

void APVPGamesCharacter::OnLevelChanged_Implementation(const FGameplayAttributeData& OldLevel) {
    UE_LOG(LogTemplateCharacter, Log, TEXT("Level changed."));
}

void APVPGamesCharacter::OnKillCountChanged_Implementation(const FGameplayAttributeData& OldKillCount) {
    UE_LOG(LogTemplateCharacter, Log, TEXT("Kill Count changed."));
}

void APVPGamesCharacter::OnDeathCountChanged_Implementation(const FGameplayAttributeData& OldDeathCount) {
    UE_LOG(LogTemplateCharacter, Log, TEXT("Death Count changed."));
}