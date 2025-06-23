// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 100.0f;
	Health = MaxHealth;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	
	
}

void UHealthComponent::ApplyDamage(float DamageAmount, FHitResult Hit)
{
	Health -= DamageAmount;

	if (Health <= 0.0f)
	{
		Health = 0.0f;
		OnHealthExhausted.Broadcast(Hit);
	}
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Health: %f"), Health));
}

void UHealthComponent::AddHealth(float HealAmount)
{
	Health += HealAmount;

	if (Health > MaxHealth)
	{
		Health = MaxHealth;
	}
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Health: %f"), Health));
}



