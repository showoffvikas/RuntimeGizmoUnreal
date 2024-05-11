// Fill out your copyright notice in the Description page of Project Settings.


#include "Gizmo.h"

// Sets default values
AGizmo::AGizmo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMesh;
	StaticMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	StaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	StaticMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

	CapsuleComponentX = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponentX->SetupAttachment(RootComponent);
	CapsuleComponentY = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent1"));
	CapsuleComponentY->SetupAttachment(RootComponent);
	CapsuleComponentZ = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent2"));
	CapsuleComponentZ->SetupAttachment(RootComponent);

	StaticMeshX = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponentX"));
	StaticMeshX->SetupAttachment(RootComponent);
	StaticMeshX->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	StaticMeshX->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	StaticMeshX->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

	StaticMeshY = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponentY"));
	StaticMeshY->SetupAttachment(RootComponent);
	StaticMeshY->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	StaticMeshY->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	StaticMeshY->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

	StaticMeshZ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponentZ"));
	StaticMeshZ->SetupAttachment(RootComponent);
	StaticMeshZ->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	StaticMeshZ->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	StaticMeshZ->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

}

// Called when the game starts or when spawned
void AGizmo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGizmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

