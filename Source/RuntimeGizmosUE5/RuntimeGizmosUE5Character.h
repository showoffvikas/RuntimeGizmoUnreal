// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Gizmo.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "CollisionQueryParams.h"
#include "GameFramework/Character.h"
#include "RuntimeGizmosUE5Character.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class ARuntimeGizmosUE5Character : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* SelectMultiAction;

	
public:
	ARuntimeGizmosUE5Character();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool rightclicked = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	   bool leftclicked;
	FVector CurrentHitLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	   bool HoldingAxis;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	    float axisholdingdiff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	   AActor* SelectedGizmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ShiftClicked;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CtrlClicked;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool RecentlyLeftClicked;
	AGizmo* PreviousSelectedGizmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	   TArray<AGizmo*>SelectedGizmoArr;
	int SelectedAxis;

	AActor* PreviosHittedActor;
	AActor* CurrentHittingActor;
	UStaticMeshComponent* PreviousHittedComponent;
	TMap<AActor*, int>SelectedActors;
	TArray<AActor* >SelectedActorArr;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void AxisSelected(int currentaxis,AGizmo* Gizmo);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void RecentlySelected();


	
protected:
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	void CheckObject();

	void OnLeftClick();

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
	void OnMultiSelectClick();
	void OnMultiSelectRelease();
	
	

	

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }


};

