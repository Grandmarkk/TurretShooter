// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilityPickup.generated.h"

UCLASS()
class TURRETSHOOTER_API AAbilityPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbilityPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GrantAbility();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Ability
	UPROPERTY(EditAnywhere)
	class UGameplayAbility* Ability;

	// Collision box


	// Mesh

};
