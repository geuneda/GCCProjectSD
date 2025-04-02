// Fill out your copyright notice in the Description page of Project Settings.


#include "Ground.h"

// Sets default values
AGround::AGround()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(FName("Ground"));
}

// Called when the game starts or when spawned
void AGround::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGround::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

