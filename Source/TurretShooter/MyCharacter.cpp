// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "AbilitySystemComponent.h"
#include "BasicAttributeSet.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create ability system
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UBasicAttributeSet>(TEXT("AttributeSet"));
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Initialize ability system component
	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		AbilitySystemComponent->InitStats(UBasicAttributeSet::StaticClass(), DefaultStartingData);

		// Grant default abilities to the character
		for (auto Ability : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(Ability);
		}

		ActiveAbility = nullptr;
	}

	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyCharacter::GetAbility(TSubclassOf<UGameplayAbility> AbilityClass)
{
	if (IsValid(AbilitySystemComponent) && IsValid(AbilityClass))
	{
		AbilitySystemComponent->GiveAbility(AbilityClass);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Ability %s granted!"), *AbilityClass->GetName()));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Ability System Component or Ability Class"));
	}
}

bool AMyCharacter::CheckIsDead_Implementation()
{
	if (!AttributeSet)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AttributeSet Invalid"));
		return true;
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Player health: %f"), AttributeSet->Health.GetCurrentValue()));
	return AttributeSet->Health.GetCurrentValue() <= 0.0f;
}

void AMyCharacter::SelfDestruct_Implementation(FHitResult Hit)
{
	OnPlayerDeath.Broadcast();
}

void AMyCharacter::TakeDamage_Implementation(float amount, FHitResult Hit)
{
	// Maually apply damage to the character's health attribute
	AbilitySystemComponent->ApplyModToAttribute(
		UBasicAttributeSet::GetHealthAttribute(),
		EGameplayModOp::Additive,
		-amount
	);
	
	if (Execute_CheckIsDead(this))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Player is dead"));
		Execute_SelfDestruct(this, Hit);
	}
}
