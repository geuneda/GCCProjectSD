// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageTextWidget.h"

void UDamageTextWidget::SetDamageText(float Damage, bool Berserk)
{
	if (!Berserk)
	{
		DamageText->SetText(FText::AsNumber(Damage));
	}
	else
	{
		DamageText->SetDefaultColorAndOpacity(FLinearColor::Yellow);
		DamageText->SetText(FText::FromString(TEXT("면역")));
	}
}