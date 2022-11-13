// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSBlackHole.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;
	
	InnerSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InnerSphereComponent"));
	InnerSphereComponent->SetSphereRadius(100);
	InnerSphereComponent->SetupAttachment(MeshComp);

	InnerSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::OnOverlapInnerSphere);

	OuterSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("OuterSphereComponent"));
	OuterSphereComponent->SetSphereRadius(3000);
	OuterSphereComponent->SetupAttachment(MeshComp);
}

void AFPSBlackHole::OnOverlapInnerSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}

// Called when the game starts or when spawned
void AFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();

	InnerSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::OnOverlapInnerSphere);
	
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> OverlappedComponents;
	OuterSphereComponent->GetOverlappingComponents(OverlappedComponents);

	for (int32 i = 0; i < OverlappedComponents.Num(); i++)
	{
		UPrimitiveComponent* PrimComp = OverlappedComponents[i];
		if (PrimComp && PrimComp->IsSimulatingPhysics())
		{
			const float SphereRadius = OuterSphereComponent->GetScaledSphereRadius();
			const float ForceStrength = -2000;

			PrimComp->AddRadialForce(GetActorLocation(), SphereRadius, ForceStrength, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}

}

