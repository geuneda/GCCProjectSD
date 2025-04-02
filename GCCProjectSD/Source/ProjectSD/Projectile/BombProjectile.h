#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "BombProjectile.generated.h"


UCLASS()
class PROJECTSD_API ABombProjectile : public AProjectileBase
{
	GENERATED_BODY()

public:
	ABombProjectile();

private:

protected:
	/** 총알이 충돌을 감지할 경우 호출 */
	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
				   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
};
