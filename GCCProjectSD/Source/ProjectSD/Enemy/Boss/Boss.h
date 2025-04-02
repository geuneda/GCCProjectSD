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
};

