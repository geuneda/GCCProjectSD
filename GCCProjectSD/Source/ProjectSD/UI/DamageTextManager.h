// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageTextManager.generated.h"

UCLASS()
class PROJECTSD_API ADamageTextManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageTextManager();

	UFUNCTION(BlueprintCallable)
	void SpawnDamageText(float Damage, FVector WorldLocation, bool Berserk = false);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = Damage)
	TSubclassOf<UUserWidget> DamageTextWidgetClass;

	UPROPERTY()
	APlayerController* CachedPlayerController;
};
