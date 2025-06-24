// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Destructible.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDestructible : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TURRETSHOOTER_API IDestructible
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TakeDamage(float amount, FHitResult Hit);
	virtual void TakeDamage_Implementation(float amount, FHitResult Hit) = 0;

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CheckIsDead();
	virtual bool CheckIsDead_Implementation() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SelfDestruct(FHitResult Hit);
	virtual void SelfDestruct_Implementation(FHitResult Hit) = 0;
};
