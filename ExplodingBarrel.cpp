// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplodingBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
AExplodingBarrel::AExplodingBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
	RootComponent = BarrelMesh;

	FireparticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireParticleSystemComponent"));
	FireparticleSystemComponent->SetupAttachment(RootComponent);
	FireparticleSystemComponent->bAutoActivate = false;

	RadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));

	RadialForce->Radius = ExplosionRadius;
	RadialForce->bImpulseVelChange = true;
	RadialForce->bAutoActivate = false;
	RadialForce->bIgnoreOwningActor = true;

	OnTakeAnyDamage.AddDynamic(this, &AExplodingBarrel::OnDamageTaken);

	HitPoints = 10.0f;
	ExplosionDamage = 30.0f;
	ExplosionRadius = 200.0f;
	
}

// Called when the game starts or when spawned
void AExplodingBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

void AExplodingBarrel::OnDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if(Damage > 0.f && HitPoints > 0.f)
	{
		HitPoints -= Damage;
		UE_LOG(LogTemp, Warning, TEXT("Damage Taken, HitPoints: %f"), HitPoints);

		if (!FireparticleSystemComponent->IsActive())
		{
			FireparticleSystemComponent->Activate();
		}

		if (!GetWorld()->GetTimerManager().IsTimerActive(CountdownTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &AExplodingBarrel::Countdown, 1.0f, true);
		}
	}

	if (HitPoints <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Barrel Exploded"));
		if (FireparticleSystemComponent != nullptr)
		{
			FireparticleSystemComponent->Deactivate();
		}
	}

	FVector BarrelLocation = GetActorLocation();

	Destroy();

	UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, BarrelLocation, ExplosionRadius, nullptr, TArray<AActor*>(), this);

	if(RadialForce != nullptr)
	{
		RadialForce->FireImpulse();
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, BarrelLocation);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, BarrelLocation);
}

void AExplodingBarrel::Countdown()
{
	HitPoints--;

	if (HitPoints <= 0.f)
	{
		OnDamageTaken(this, 0.f, nullptr, nullptr, nullptr);
	}
}

// Called every frame
void AExplodingBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

