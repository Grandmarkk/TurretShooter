// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Destructible.h"
#include "Turret.generated.h"


class UCapsuleComponent;

UCLASS()
class TURRETSHOOTER_API ATurret : public AActor, public IDestructible
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TurretBase;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TurretTop;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* CapsulePlayerCheck;

	UPROPERTY(EditAnywhere)
	USoundBase* DestroySound;

	UPROPERTY(EditAnywhere)
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere)
	USoundBase* TargetedSound;

	UPROPERTY(EditAnywhere)
	class UArrowComponent* Arrow;

	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* ExplosionParticle;

	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* FireParticle;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> FireActor;

	UPROPERTY(EditAnywhere)
	float ScanSpeed;

	UPROPERTY(EditAnywhere)
	float ScanAngle;

	UPROPERTY(EditDefaultsOnly)
	float FireRate;

	// Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;


	// Implement Destructible interface
	virtual void TakeDamage_Implementation(float amount, FHitResult Hit) override;

protected:
	virtual bool CheckIsDead_Implementation() override;

	virtual void SelfDestruct_Implementation(FHitResult Hit) override;

private:
	class ACharacter* PlayerCharacter;

	float ScanRotation;

	UFUNCTION()
	void OnCheckBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCheckEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void Fire();

	void CheckPlayerInView();

	bool bIsDestroyed;

	bool bIsPlayerOverlapped;

	bool bIsPlayerTracked;

	FTimerHandle TimerHandle;
};
