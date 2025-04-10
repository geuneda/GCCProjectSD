// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectSD/Enemy/Boss/Boss.h"

#include "DSP/Delay.h"
#include "GameFramework/Character.h"
#include "ProjectSD/Item/MineItem/MineItem.h"
#include "ProjectSD/Projectile/BombProjectile.h"
#include "ProjectSD/Projectile/HomingProjectile.h"
#include "ProjectSD/Projectile/MissileProjectile.h"

// Sets default values
ABoss::ABoss(): Player(nullptr), Mesh(nullptr)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ExplosionLocations.Empty();
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

#pragma region special Attack Patten
void ABoss::SetFlameExplosionTimer()
{
	GetWorldTimerManager().SetTimer(
	FlameRepeatTimer,
	this,
	&ABoss::FlameExplosion,
	0.075f,
	true);
}

void ABoss::FlameExplosion()
{
	// 소켓 이름 리스트 생성
	for (int i = 1; i <= 6; i++)
	{
		FString SocketName = FString::Printf(TEXT("RocketSocket%d"), i);
		FTransform SocketTransform = Mesh->GetSocketTransform(FName(*SocketName), ERelativeTransformSpace::RTS_World);

		// 배열에 추가
		RocketSocketTransforms.Add(SocketTransform);
	}

	// 미사일을 발사할 소켓의 위치를 랜덤으로 할당
	int32 RandSocketIndex = FMath::RandRange(0,5);
	FVector SpawnLocation = RocketSocketTransforms[RandSocketIndex].GetLocation();
	FRotator SpawnRotation = RocketSocketTransforms[RandSocketIndex].Rotator();

	// 발사할 미사일 캐스팅
	AMissileProjectile* SpawnedRocket = GetWorld()->SpawnActor<AMissileProjectile>(MissileClass, SpawnLocation, SpawnRotation);

	// 랜덤 각도 할당
	float RandSocketRoll = FMath::RandRange(-0.2f, 0.2f);
	float RandSocketPitch = FMath::RandRange(-0.2f, 0.2f);

	// 미사일 발사
	if (SpawnedRocket)
	{
		FVector LaunchDirection = FVector(RandSocketRoll, RandSocketPitch, 1); 
		SpawnedRocket->FireMissileIntoTheSky(LaunchDirection);
	}

	// 스폰할 지뢰 좌표 할당
	FVector MineSpawnLocation = ExplosionLocations[MineLocationIndex]->GetActorLocation();

	GetWorld()->SpawnActor<AMineItem>(MineClass, MineSpawnLocation, FRotator::ZeroRotator);
	

	++FlameRepeatCount;
	++MineLocationIndex;

	if (FlameRepeatCount >= 16)
	{
		GetWorldTimerManager().ClearTimer(FlameRepeatTimer);
		FlameRepeatCount = 0;
	}

	if (MineLocationIndex == ExplosionLocations.Num())
	{
		MineLocationIndex = 0;
	}
}

void ABoss::MoveToTarget()
{
	FVector BossLocation = GetActorLocation();
	FVector PlayerLocation = Player->GetActorLocation();
	FVector Direction = (PlayerLocation - BossLocation).GetSafeNormal();

	AddMovementInput(Direction, 1.0f);
}

void ABoss::StopMovement()
{
	AddMovementInput(FVector::ZeroVector, 1.0f);
}


#pragma endregion

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
		bIsLSlugShot = true;
		Projectile->Fire(LTargetDirection);
		GetWorldTimerManager().SetTimer(
			LSlugShotTimer,
			this,
			&ABoss::LSlugShotTimerFunction,
			0.5f,
			false);
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
		bIsRSlugShot = true;
		Projectile->Fire(RTargetDirection);
		GetWorldTimerManager().SetTimer(
			RSlugShotTimer,
			this,
			&ABoss::RSlugShotTimerFunction,
			0.5f,
			false);
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

