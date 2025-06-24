// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Projectile.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"


// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	TurretBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBase"));
	RootComponent = TurretBase;
	
	TurretTop = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretTop"));
	TurretTop->SetupAttachment(RootComponent);

	CapsulePlayerCheck = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	CapsulePlayerCheck->SetupAttachment(TurretTop);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(TurretTop);


	// Bind function
	CapsulePlayerCheck->OnComponentBeginOverlap.AddDynamic(this, &ATurret::OnCheckBeginOverlap);

	CapsulePlayerCheck->OnComponentEndOverlap.AddDynamic(this, &ATurret::OnCheckEndOverlap);

	// Initialize vars
	ScanRotation = 0;
	ScanSpeed = 10.0f;
	ScanAngle = 50.0f;
	bIsPlayerOverlapped = false;
	bIsDestroyed = false;
	bIsPlayerTracked = false;
	FireRate = 0.5f;
	PlayerCharacter = nullptr;
	Health = 100.0f;

}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ScanRotation > ScanAngle || ScanRotation < 0)
	{
		ScanSpeed *= -1;
	}
	ScanRotation += ScanSpeed * DeltaTime;

	CheckPlayerInView();

	if (PlayerCharacter && (bIsPlayerTracked || bIsPlayerOverlapped))
	{
		// Turret lock
		FRotator LockRotation = UKismetMathLibrary::FindLookAtRotation(PlayerCharacter->GetActorLocation(), TurretTop->GetComponentLocation());
		LockRotation = UKismetMathLibrary::InverseTransformRotation(GetActorTransform(), LockRotation);
		TurretTop->SetRelativeRotation(UKismetMathLibrary::RInterpTo(TurretTop->GetRelativeRotation(), LockRotation, DeltaTime, 5.0f));

		// Fire
		if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATurret::Fire, FireRate, true);
		}
	}
	else
	{
		// Turret scan rotation
		TurretTop->SetRelativeRotation(UKismetMathLibrary::RInterpTo(TurretTop->GetRelativeRotation(), FRotator(0.0f, ScanRotation, 0.0f), DeltaTime, 1.0f));
	}
}

bool ATurret::CheckIsDead_Implementation()
{
	return Health <= 0;
}

void ATurret::SelfDestruct_Implementation(FHitResult Hit)
{
	DestroyTurret(Hit);
}

void ATurret::TakeDamage_Implementation(float amount, FHitResult Hit)
{
	Health -= amount;
	if (CheckIsDead_Implementation())
	{
		SelfDestruct_Implementation(Hit);
	}
}



void ATurret::DestroyTurret(FHitResult Hit)
{
	if (!bIsDestroyed)
	{
		bIsDestroyed = true;
		TurretTop->SetSimulatePhysics(true);
		PrimaryActorTick.SetTickFunctionEnable(false);
		TurretTop->AddImpulseAtLocation(Hit.ImpactNormal * 3, Hit.Location);
		UGameplayStatics::PlaySoundAtLocation(this, DestroySound, GetActorLocation(), 0.5f);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionParticle, GetActorLocation() + FVector(0.0f, 0.0f, 10.0f), GetActorRotation());
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
	
}

void ATurret::OnCheckBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (Character)
	{
		PlayerCharacter = Character;
		bIsPlayerOverlapped = true;
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Player in range")));
	}
}

void ATurret::OnCheckEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (Character)
	{
		bIsPlayerOverlapped = false;
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Player out of range")));
	}
}

void ATurret::Fire()
{
	if (bIsPlayerTracked)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Fire")));
		if (FireActor)
		{
			GetWorld()->SpawnActor<AActor>(FireActor, Arrow->GetComponentTransform());
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FireParticle, Arrow->GetComponentLocation(), Arrow->GetComponentRotation());
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation(), 0.5f);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
	
}

void ATurret::CheckPlayerInView()
{
	if (!PlayerCharacter)
	{
		bIsPlayerTracked = false;
		return;
	}
	FHitResult HitResult;
	FVector Start = Arrow->GetComponentLocation();
	FVector End = PlayerCharacter->GetActorLocation();
	FCollisionObjectQueryParams ObjectTypeParams;
	ObjectTypeParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	ObjectTypeParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	ObjectTypeParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	ObjectTypeParams.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, ObjectTypeParams, CollisionParams);

	if (bHit)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Hit actor: %s"), *HitResult.GetActor()->GetName()));
		if (HitResult.GetActor() == PlayerCharacter)
		{
			if (!bIsPlayerTracked)
			{
				UGameplayStatics::PlaySoundAtLocation(this, TargetedSound, GetActorLocation(), 1.0f);
			}
			bIsPlayerTracked = true;
		}
		else
		{
			bIsPlayerTracked = false;
			PlayerCharacter = nullptr;
		}
	}
}



