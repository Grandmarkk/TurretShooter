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


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Ability
	UPROPERTY(EditAnywhere)
	class TSubclassOf<class UGameplayAbility> Ability;

	// Collision box
	UPROPERTY(EditAnywhere, Category = "Components")
	class UBoxComponent* CollisionBox;

	// Mesh
	UPROPERTY(EditAnywhere, Category = "Components")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	USoundBase* PickupSound;

	UFUNCTION(BlueprintCallable)
	void OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void GrantAbility(class AMyCharacter* PlayerCharacter);
};
