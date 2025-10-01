// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Actor.h"
#include "ProjectileActor.generated.h"

UCLASS()
class THREEFORSIMPLE_API AProjectileActor : public AActor, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileActor();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void ShootProjectile(
		float InSpeed,
		float InMaxDistance,
		const AActor* InTarget,
		FGenericTeamId InTeamId,
		FGameplayEffectSpecHandle InHitEffectHandle,
		ETeamAttitude::Type InTargetTeamAttitude = ETeamAttitude::Hostile
		);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID);
	
	virtual FGenericTeamId GetGenericTeamId() const;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Cue")
	FGameplayTag HitGameplayCueTag;
	//Replicate everything that the client needs to know/ Don't replicate something if its not necessary
	//If a replicated variable changes on the server the client's version of that variable will also get updated with the new value set by the server
	UPROPERTY(Replicated)
	FGenericTeamId TeamId;

	UPROPERTY(Replicated)
	FVector MoveDir;

	UPROPERTY(Replicated)
	float ProjectileSpeed;

	UPROPERTY(Replicated)
	int TargetTeamAttitude;

	UPROPERTY()
	const AActor* Target;
	
	FGameplayEffectSpecHandle HitEffectSpecHandle;
	
	FTimerHandle ShootTimerHandle;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void TravelMaxDistanceReached();
	void SendLocalGameplayCue(AActor* CueTargetActor, const FHitResult& HitResult);

};
