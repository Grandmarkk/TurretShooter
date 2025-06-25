// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Destructible.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->SetNotifyRigidBodyCollision(true);


	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(CollisionComponent);

	// Set default values
	DamageAmount = 10.0f;



	// Bind functions
	//CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	OnActorHit.AddDynamic(this, &AProjectile::OnHit);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AProjectile::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	// Spawn partivle system at hit location
	FRotator SpawnRotation = FRotationMatrix::MakeFromX(Hit.ImpactNormal).Rotator();
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitParticle, Hit.ImpactPoint, SpawnRotation);


	// Apply damage to the hit actor
	IDestructible* DestructibleActor = Cast<IDestructible>(OtherActor);
	if (DestructibleActor) 
	{
		if (!DestructibleActor->Execute_CheckIsDead(OtherActor))
		{
			DestructibleActor->Execute_TakeDamage(OtherActor, DamageAmount, Hit);
		}
		
	}

	// Destroy the projectile
	Destroy();
	
}


