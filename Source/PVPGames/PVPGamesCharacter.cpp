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
        if (Attributes && Attributes->LevelStatsDataTable)
        {
            AbilitySystemComponent->InitStats(AttributeSetClass, Attributes->LevelStatsDataTable);
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

            // Initialize stats based on the starting level using the data table
            if (Attributes && Attributes->LevelStatsDataTable) {
                Attributes->InitializeStatsFromLevel();
            }
        }
    }
}

void APVPGamesCharacter::GiveDefaultAbilities()
{
    if (HasAuthority() && AbilitySystemComponent)
        for (TSubclassOf<UGameplayAbility>& StartUpAbility : DefaultAbilities)
            AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartUpAbility.GetDefaultObject(), 1, 0));
}

void APVPGamesCharacter::OnCharacterDeath()
{
    // Logic to handle character death in C++
    UE_LOG(LogTemplateCharacter, Log, TEXT("Character has died."));

    // Call the Blueprint version of this function
    OnCharacterDeath_BP();
}