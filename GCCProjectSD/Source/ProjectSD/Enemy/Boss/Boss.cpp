// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectSD/Enemy/Boss/Boss.h"
#include "GameFramework/Character.h"
#include "ProjectSD/Projectile/BombProjectile.h"
#include "ProjectSD/Projectile/HomingProjectile.h"

// Sets default values
ABoss::ABoss(): Player(nullptr), Mesh(nullptr), LJavelinRepeatCount(0)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();

	CastPlayer();
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

	FVector PlayerLocation = Player->GetActorLocation();
	FVector LeftMuzzleLocation = Mesh->GetSocketLocation(FName("L_MuzzleSocket"));
	FVector LTargetDirection = (PlayerLocation - LeftMuzzleLocation).GetSafeNormal();
	ABombProjectile* Projectile = GetWorld()->SpawnActor<ABombProjectile>(BombClass, LeftMuzzleLocation, FRotator::ZeroRotator);

	if (Projectile != nullptr)
	{
		Projectile->Fire(LTargetDirection);
	}
}

void ABoss::RSlugShot()
{
	if (Player == nullptr) return;

	FVector PlayerLocation = Player->GetActorLocation();
	FVector RightMuzzleLocation = Mesh->GetSocketLocation(FName("R_MuzzleSocket"));
	FVector RTargetDirection = (PlayerLocation - RightMuzzleLocation).GetSafeNormal();
	ABombProjectile* Projectile = GetWorld()->SpawnActor<ABombProjectile>(BombClass, RightMuzzleLocation, FRotator::ZeroRotator);
	
	if (Projectile != nullptr)
	{
		Projectile->Fire(RTargetDirection);
	}
}

void ABoss::LJavelinShotStart()
{
	GetWorldTimerManager().SetTimer(
		LJavelinRepeatTimer,
		this,
		&ABoss::LJavelinShot,
		0.4f,
		true);
}


void ABoss::LJavelinShot()
{
	// 왼쪽 어깨 미사일 Socket 반환
	FString SocketName = FString::Printf(TEXT("MissileSocket_L%d"), LJavelinRepeatCount+1);
	FTransform SocketTransform = Mesh->GetSocketTransform(FName(*SocketName), ERelativeTransformSpace::RTS_World);

	// Homing Projectile 생성
	AHomingProjectile* HomingProjectile = GetWorld()->SpawnActor<AHomingProjectile>(HomingClass,SocketTransform.GetLocation(),FRotator::ZeroRotator);

	// 방향벡터 설정
	FVector LaunchDirection = FVector(0, -1, 1); 

	// 발사
	HomingProjectile->Fire(LaunchDirection);
	
	// 발사한 탄환 수 1개 증가
	++LJavelinRepeatCount;

	// 모두 발사한 경우 변수 및 타이머 초기화
	if (LJavelinRepeatCount >=  5)
	{
		LJavelinRepeatCount = 0;
		GetWorldTimerManager().ClearTimer(LJavelinRepeatTimer);
	}
	
}

void ABoss::RJavelinShotStart()
{
	GetWorldTimerManager().SetTimer(
		RJavelinRepeatTimer,
		this,
		&ABoss::RJavelinShot,
		0.4f,
		true);
}


void ABoss::RJavelinShot()
{
	// 왼쪽 어깨 미사일 Socket 반환
	FString SocketName = FString::Printf(TEXT("MissileSocket_R%d"), RJavelinRepeatCount+1);
	FTransform SocketTransform = Mesh->GetSocketTransform(FName(*SocketName), ERelativeTransformSpace::RTS_World);

	// Homing Projectile 생성
	AHomingProjectile* HomingProjectile = GetWorld()->SpawnActor<AHomingProjectile>(HomingClass,SocketTransform.GetLocation(),FRotator::ZeroRotator);

	// 방향벡터 설정
	FVector LaunchDirection = FVector(0, -1, 1); 

	// 발사
	HomingProjectile->Fire(LaunchDirection);
	
	// 발사한 탄환 수 1개 증가
	++RJavelinRepeatCount;

	// 모두 발사한 경우 변수 및 타이머 초기화
	if (RJavelinRepeatCount >=  5)
	{
		RJavelinRepeatCount = 0;
		GetWorldTimerManager().ClearTimer(RJavelinRepeatTimer);
	}
	
}
#pragma endregion
