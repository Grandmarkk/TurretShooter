// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityPickup.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter.h"

// Sets default values
AAbilityPickup::AAbilityPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(Mesh);

	// Bind function
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AAbilityPickup::OnPlayerOverlap);


}

// Called when the game starts or when spawned
void AAbilityPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAbilityPickup::GrantAbility(AMyCharacter* PlayerCharacter)
{
	PlayerCharacter->GetAbility(Ability);
}

void AAbilityPickup::OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the overlapping actor is a player character
	AMyCharacter* PlayerCharacter = Cast<AMyCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		// Grant the ability to the player character
		GrantAbility(PlayerCharacter);
		// Play Sound
		UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());

		// Optionally, destroy the pickup after granting the ability
		Destroy();
	}
	else
	{
		// Log or handle the case where the overlapping actor is not a player character
		UE_LOG(LogTemp, Warning, TEXT("Overlap with non-player actor: %s"), *OtherActor->GetName());
	}
}

// Called every frame
void AAbilityPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

