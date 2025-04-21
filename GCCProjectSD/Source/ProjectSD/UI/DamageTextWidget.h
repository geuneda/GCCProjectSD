// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlock.h"
#include "DamageTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSD_API UDamageTextWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION()
	void SetDamageText(float Damage, bool Berserk = false);
	
	UPROPERTY(meta=(BindWidget))
	class URichTextBlock* DamageText;
};