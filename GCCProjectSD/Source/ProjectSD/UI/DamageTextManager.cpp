// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageTextManager.h"
#include "DamageTextWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADamageTextManager::ADamageTextManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ADamageTextManager::BeginPlay()
{
	Super::BeginPlay();

	CachedPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

void ADamageTextManager::SpawnDamageText(float Damage, FVector WorldLocation, bool Berserk)
{
	if (!DamageTextWidgetClass || !CachedPlayerController) return;

	FVector2D ScreenPosition;
	bool bSuccess = UGameplayStatics::ProjectWorldToScreen(CachedPlayerController, WorldLocation, ScreenPosition);

	if (!bSuccess) return;

	UDamageTextWidget* DamageWidget = Cast<UDamageTextWidget>(CreateWidget(GetWorld(), DamageTextWidgetClass));
	if (!DamageWidget) return;

	DamageWidget->AddToViewport();
	DamageWidget->SetDamageText(Damage, Berserk);

	DamageWidget->SetPositionInViewport(ScreenPosition, true);
}
