// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class TURRETSHOOTER_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Ability system component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	class UAbilitySystemComponent* AbilitySystemComponent;

	// Attribute set for this character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	const class UBasicAttributeSet* AttributeSet;

	// Default abilities
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> DefaultAbilities;

	// Active ability
	UPROPERTY(BlueprintReadWrite, Category = "Abilities")
	class UGameplayAbility* ActiveAbility;

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void GetAbility(class UGameplayAbility* AbilityClass);
};
