// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Boss.generated.h"

class AHomingProjectile;
class ABombProjectile;
class AProjectileBase;
class ASpawnVolume;
class AMineItem;
class AMissileProjectile;
class ABossController;

UCLASS()
class PROJECTSD_API ABoss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABoss();
	
	// 플레이어
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Target")
	ACharacter* Player;

	// SkeletalMesh
	USkeletalMeshComponent* Mesh;

	// 발사할 폭탄 클래스 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	TSubclassOf<ABombProjectile> BombClass;
	
	/** 발사할 유도 미사일 클래스 정보 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	TSubclassOf<AHomingProjectile> HomingClass;

	// 자벨린 샷 타이머 (L)
	FTimerHandle LJavelinRepeatTimer;
	// 자벨린 샷 타이머 (R)
	FTimerHandle RJavelinRepeatTimer;
	// 타이머 관리 카운트
	int LJavelinRepeatCount = 0;
	// 타이머 관리 카운트
	int RJavelinRepeatCount = 0;
	
	// Flame 타이머
	FTimerHandle FlameRepeatTimer;

	// 타이머 관리 카운트
	int FlameRepeatCount = 0;
	
	// Flame 미사일 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	TSubclassOf<AMissileProjectile> MissileClass;
	
	// 로켓 소켓 리스트 위치정보
	TArray<FTransform> RocketSocketTransforms;

	// 미사일 발사 후 폭파할 위치 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	TArray<AActor*> ExplosionLocations;
	
	// Mine 위치 인덱스 정보
	uint32 MineLocationIndex = 0;

	// Mine 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	TSubclassOf<AMineItem> MineClass;

	// 왼쪽 SlugShot 패턴 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Locomotion")
	bool bIsLSlugShot;
	// 오른쪽 SlugShot 패턴 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Locomotion")
	bool bIsRSlugShot;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Locomotion")
	bool bIsSpawned;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CastPlayer();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// LSlugShot (투사체, 폭발)
	UFUNCTION(BlueprintCallable)
	void LSlugShot();
	
	// RSlugShot (투사체, 폭발)
	UFUNCTION(BlueprintCallable)
	void RSlugShot();

	// JabelinShotStartTimer ()
	UFUNCTION(BlueprintCallable)
	void LJavelinShotStart();
	void LJavelinShot();

	// JabelinShotStartTimer ()
	UFUNCTION(BlueprintCallable)
	void RJavelinShotStart();
	void RJavelinShot();

	// Flame 로직 시작 타이머 함수
	UFUNCTION(BlueprintCallable)
	void SetFlameExplosionTimer();
	// Flame 로직 내부 함수
	void FlameExplosion();
	// 이동
	UFUNCTION(BlueprintCallable)
	void MoveToTarget();

	UFUNCTION(BlueprintCallable)
	void StopMovement();
};

