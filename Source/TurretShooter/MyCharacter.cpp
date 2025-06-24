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

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Initialize ability system component
	if (IsValid(AbilitySystemComponent))
	{
		AttributeSet = AbilitySystemComponent->GetSet<UBasicAttributeSet>();

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


