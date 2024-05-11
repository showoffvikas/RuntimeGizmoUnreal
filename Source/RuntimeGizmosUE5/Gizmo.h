// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Gizmo.generated.h"

UCLASS()
class RUNTIMEGIZMOSUE5_API AGizmo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGizmo();
	AActor* AttachedActor = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gizmo")
		UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
		UCapsuleComponent* CapsuleComponentX;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
		UCapsuleComponent* CapsuleComponentY;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
		UCapsuleComponent* CapsuleComponentZ;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GizmoX")
		UStaticMeshComponent* StaticMeshX;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GizmoY")
		UStaticMeshComponent* StaticMeshY;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GizmoZ")
		UStaticMeshComponent* StaticMeshZ;


};
