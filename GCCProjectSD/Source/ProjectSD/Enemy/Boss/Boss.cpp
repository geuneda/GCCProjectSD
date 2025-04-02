// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectSD/Enemy/Boss/Boss.h"
#include "GameFramework/Character.h"
#include "ProjectSD/Projectile/BombProjectile.h"

// Sets default values
ABoss::ABoss()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();

	CastPlayer();

	LSlugShot();
}

// Called every frame
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABoss::CastPlayer()
{
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if (playerController != nullptr)
	{
		Player = Cast<ACharacter>(playerController->GetCharacter());
	}

	Mesh = GetMesh();
}


#pragma region Normal Attack Pattern
void ABoss::LSlugShot()
{
	if (Player == nullptr) return;

	// 플레이어 위치 반환
	FVector PlayerLocation = Player->GetActorLocation();
	FVector LeftMuzzleLocation = Mesh->GetSocketLocation(FName("L_MuzzleSocket"));
	FVector LTargetDirection = (PlayerLocation - LeftMuzzleLocation).GetSafeNormal();
	ABombProjectile* Projectile = GetWorld()->SpawnActor<ABombProjectile>(BombClass, LeftMuzzleLocation, FRotator::ZeroRotator);

	if (Projectile != nullptr)
	{
		Projectile->Fire(LTargetDirection);
	}
}
#pragma endregion