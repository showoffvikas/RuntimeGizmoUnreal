// Copyright Epic Games, Inc. All Rights Reserved.

#include "RuntimeGizmosUE5Character.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"



//////////////////////////////////////////////////////////////////////////
// ARuntimeGizmosUE5Character

ARuntimeGizmosUE5Character::ARuntimeGizmosUE5Character()
{
	
	
	
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	

}

void ARuntimeGizmosUE5Character::BeginPlay()
{ 
	Super::BeginPlay();

	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
}

void ARuntimeGizmosUE5Character::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);
	CheckObject();
	if (leftclicked) {
		OnLeftClick();
	}
	for (int i = 0; i < SelectedGizmoArr.Num(); i++) {
		if (SelectedGizmoArr[i]) {
			FVector GizmoLocation = SelectedGizmoArr[i]->GetActorLocation();
			FVector Dist1 = GetActorLocation() - GizmoLocation;
			float res = Dist1.Length();
			res /= 10000;
			SelectedGizmoArr[i]->StaticMeshX->SetWorldScale3D(FVector(res, res, res));
			SelectedGizmoArr[i]->StaticMeshY->SetWorldScale3D(FVector(res, res, res));
			SelectedGizmoArr[i]->StaticMeshZ->SetWorldScale3D(FVector(res, res, res));
		}
		
	}
}

void ARuntimeGizmosUE5Character::CheckObject()
{
	

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
		return;

	
	FVector2D MousePosition;
	if (!PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y))
		return;

	
	
	FVector WorldLocation, WorldDirection;
	PlayerController->DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldLocation, WorldDirection);
	AActor* HitActor = NULL;
	FHitResult HitResult;

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnPhysicalMaterial = false;
	QueryParams.AddIgnoredActor(this);

	TArray<FHitResult> OutHits;


	bool bTraceSuccess = GetWorld()->LineTraceMultiByObjectType(
		OutHits,
		WorldLocation + WorldDirection * 50,
		WorldLocation + WorldDirection * 1000000,
		FCollisionObjectQueryParams::AllObjects,
		QueryParams
	);

	DrawDebugLine(
		GetWorld(),
		WorldLocation + WorldDirection * 50,
		WorldLocation + WorldDirection * 1000000,
		FColor::Red,
		false,
		-1,
		0,
		1.0f
	);
	CurrentHitLocation = WorldDirection;
	
	if (bTraceSuccess)
	{
		
		int lastindx = OutHits.Num() - 1;
		OutHits.Sort([](const FHitResult& A, const FHitResult& B) {
			return A.Distance < B.Distance;
			});
		int32 OutHitsNum = OutHits.Num();
		FString OutHitsLengthString = FString::Printf(TEXT("OutHits Array Length: %d"), OutHitsNum);

	
		for (int32 HitIndex = lastindx; HitIndex > 0; HitIndex--)
		{

			FHitResult& HitResultloc = OutHits[HitIndex];
			if (HitResultloc.GetActor()->ActorHasTag("gizmo")) {
				if (AGizmo* Gizmo = Cast<AGizmo>(HitResultloc.GetActor())) {
					HitActor = Gizmo;
					HitResult = HitResultloc;
				
					break;
				}
			}
		}
		if (!HitActor) {
			HitActor = OutHits[0].GetActor();
		}
		if (!HitResult.bBlockingHit) {
			HitResult = OutHits[0];
		}
	  
		if (HitActor)
		{   
			
			CurrentHittingActor = HitActor;
			if (SelectedActors.Find(CurrentHittingActor ) || HoldingAxis) {
				return;
			}
			if (HitActor->ActorHasTag("floor")) {
				if (PreviosHittedActor == CurrentHittingActor) {
					return;
				}
				
				if (PreviosHittedActor) {
					UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(PreviosHittedActor->GetRootComponent());
					if (PrimitiveComponent)
					{
						
						PrimitiveComponent->SetRenderCustomDepth(false);
						PrimitiveComponent->SetCustomDepthStencilValue(-1);
						
					}
					if (PreviosHittedActor->ActorHasTag("gizmo")) {
						if (AGizmo* HitGizmo = Cast<AGizmo>(PreviosHittedActor)) 
						{
						
							    HitGizmo->StaticMesh->SetRenderCustomDepth(true);
								HitGizmo->StaticMesh->SetCustomDepthStencilValue(0);
								HitGizmo->StaticMeshX->SetRenderCustomDepth(true);
								HitGizmo->StaticMeshX->SetCustomDepthStencilValue(1);
								HitGizmo->StaticMeshY->SetRenderCustomDepth(true);
								HitGizmo->StaticMeshY->SetCustomDepthStencilValue(2);
								HitGizmo->StaticMeshZ->SetRenderCustomDepth(true);
								HitGizmo->StaticMeshZ->SetCustomDepthStencilValue(3);

							

						}
						
						
					}
					PreviosHittedActor = CurrentHittingActor;
					return;
				}
				
				PreviosHittedActor = HitActor;
				return;
			}
			else if (HitActor->ActorHasTag("gizmo")) {
				  
					if (AGizmo* HitGizmo = Cast<AGizmo>(HitActor)) {

						if (HitResult.Component == HitGizmo->StaticMeshX) {
							HitGizmo->StaticMeshX->SetRenderCustomDepth(true);
							HitGizmo->StaticMeshX->SetCustomDepthStencilValue(5);
							HitGizmo->StaticMeshY->SetRenderCustomDepth(true);
							HitGizmo->StaticMeshY->SetCustomDepthStencilValue(2);
							HitGizmo->StaticMeshZ->SetRenderCustomDepth(true);
							HitGizmo->StaticMeshZ->SetCustomDepthStencilValue(3);
							HitGizmo->StaticMesh->SetRenderCustomDepth(true);
							HitGizmo->StaticMesh->SetCustomDepthStencilValue(0);
							PreviousHittedComponent = HitGizmo->StaticMeshX;

						}
						else if (HitResult.Component == HitGizmo->StaticMeshY) {
							HitGizmo->StaticMeshX->SetRenderCustomDepth(true);
							HitGizmo->StaticMeshX->SetCustomDepthStencilValue(1);
							HitGizmo->StaticMeshY->SetRenderCustomDepth(true);
							HitGizmo->StaticMeshY->SetCustomDepthStencilValue(5);
							HitGizmo->StaticMeshZ->SetRenderCustomDepth(true);
							HitGizmo->StaticMeshZ->SetCustomDepthStencilValue(3);
							HitGizmo->StaticMesh->SetRenderCustomDepth(true);
							HitGizmo->StaticMesh->SetCustomDepthStencilValue(0);
							PreviousHittedComponent = HitGizmo->StaticMeshY;

						}
						else if (HitResult.Component == HitGizmo->StaticMeshZ) {
							HitGizmo->StaticMeshX->SetRenderCustomDepth(true);
							HitGizmo->StaticMeshX->SetCustomDepthStencilValue(1);
							HitGizmo->StaticMeshY->SetRenderCustomDepth(true);
							HitGizmo->StaticMeshY->SetCustomDepthStencilValue(2);
							HitGizmo->StaticMeshZ->SetRenderCustomDepth(true);
							HitGizmo->StaticMeshZ->SetCustomDepthStencilValue(5);
							HitGizmo->StaticMesh->SetRenderCustomDepth(true);
							HitGizmo->StaticMesh->SetCustomDepthStencilValue(0);
							PreviousHittedComponent = HitGizmo->StaticMeshZ;
						}
						PreviosHittedActor = CurrentHittingActor;
						return;
						
					}
					PreviosHittedActor = CurrentHittingActor;
					return;
				
			}
			else if (PreviosHittedActor) {
				if (CurrentHittingActor == PreviosHittedActor) {
					return;
			    }
			   
					UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(PreviosHittedActor->GetRootComponent());
					if (PrimitiveComponent)
					{
					
						PrimitiveComponent->SetRenderCustomDepth(false);
						PrimitiveComponent->SetCustomDepthStencilValue(-1);
					}
					if (CurrentHittingActor) {
						UPrimitiveComponent* PrimitiveComponent1 = Cast<UPrimitiveComponent>(CurrentHittingActor->GetRootComponent());
						if (PrimitiveComponent1)
						{
						
							PrimitiveComponent1->SetRenderCustomDepth(true);
							PrimitiveComponent1->SetCustomDepthStencilValue(5);
						}
					}
					if (PreviosHittedActor->ActorHasTag("gizmo")) {
						if (AGizmo* HitGizmo = Cast<AGizmo>(PreviosHittedActor)) {

							
								HitGizmo->StaticMeshX->SetRenderCustomDepth(true);
								HitGizmo->StaticMeshX->SetCustomDepthStencilValue(1);
								HitGizmo->StaticMeshY->SetRenderCustomDepth(true);
								HitGizmo->StaticMeshY->SetCustomDepthStencilValue(2);
								HitGizmo->StaticMeshZ->SetRenderCustomDepth(true);
								HitGizmo->StaticMeshZ->SetCustomDepthStencilValue(3);
								HitGizmo->StaticMesh->SetRenderCustomDepth(true);
								HitGizmo->StaticMesh->SetCustomDepthStencilValue(0);

							
							PreviosHittedActor = CurrentHittingActor;
							return;
						}
						PreviosHittedActor = CurrentHittingActor;
						return;
					}

					PreviosHittedActor = CurrentHittingActor;
					return;
				
				
			}
			
			
			UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(HitActor->GetRootComponent());
			if (PrimitiveComponent)
			{
				
				PrimitiveComponent->SetRenderCustomDepth(true);
				PrimitiveComponent->SetCustomDepthStencilValue(5);
			}
			PreviosHittedActor = CurrentHittingActor;
			return;
		   
		
		}
		else
		{
			
			CurrentHittingActor = nullptr;
		}
	}

	
}
 
void ARuntimeGizmosUE5Character::OnLeftClick()
{
	
	if (HoldingAxis) {
		if (SelectedGizmo) {
			FVector Dist = GetActorLocation() - SelectedGizmo->GetActorLocation();
			CurrentHitLocation = GetActorLocation() + (CurrentHitLocation * Dist.Length());
			if (SelectedAxis == 1) {
				FVector CurrentLocation1 = SelectedGizmo->GetActorLocation();
				FVector NewLocation1 = FVector(CurrentHitLocation.X - axisholdingdiff, CurrentLocation1.Y, CurrentLocation1.Z);
				SelectedGizmo->SetActorLocation(NewLocation1);
				
			}
			else if (SelectedAxis == 2) {
				FVector CurrentLocation1 = SelectedGizmo->GetActorLocation();
				FVector NewLocation1 = FVector(CurrentLocation1.X , CurrentHitLocation.Y - axisholdingdiff, CurrentLocation1.Z); 
				SelectedGizmo->SetActorLocation(NewLocation1);
				
			}
			else if (SelectedAxis == 3) {
				FVector CurrentLocation1 = SelectedGizmo->GetActorLocation();
				FVector NewLocation1 = FVector(CurrentLocation1.X , CurrentLocation1.Y, CurrentHitLocation.Z - axisholdingdiff); 
				SelectedGizmo->SetActorLocation(NewLocation1);
				
			}
		}
		return;
	}
	else if (CurrentHittingActor ) {
		if (CurrentHittingActor->ActorHasTag("gizmo")) {
			
			if (AGizmo* HitGizmo = Cast<AGizmo>(CurrentHittingActor) ) {
				if (SelectedGizmo) {
					FVector Dist = GetActorLocation() - SelectedGizmo->GetActorLocation();
					CurrentHitLocation = GetActorLocation() + (CurrentHitLocation * Dist.Length());
				}
				
				if (PreviousHittedComponent == HitGizmo->StaticMeshX && HitGizmo->AttachedActor )
				{
					
					FVector CurrentLocation = HitGizmo->GetActorLocation();
					if (!HoldingAxis) {
						SelectedGizmo = HitGizmo;
						HoldingAxis = true;
						AxisSelected(1, HitGizmo);
						SelectedAxis = 1;
						axisholdingdiff = CurrentHitLocation.X - CurrentLocation.X;
						axisholdingdiff;
						
					}
					
				
					
				}
				else if (PreviousHittedComponent == HitGizmo->StaticMeshY && HitGizmo->AttachedActor )
				{
					
					FVector CurrentLocation = HitGizmo->GetActorLocation();
					if (!HoldingAxis) {
						SelectedGizmo = HitGizmo;
						HoldingAxis = true;
						AxisSelected(2, HitGizmo);
						SelectedAxis = 2;
						axisholdingdiff = CurrentHitLocation.Y - CurrentLocation.Y;
						HitGizmo->SetActorLocation(CurrentLocation);
					}
					FVector NewLocation = FVector(CurrentLocation.X , CurrentLocation.Y-axisholdingdiff, CurrentLocation.Z); // Adjust the translation amount as needed
					HitGizmo->SetActorLocation(NewLocation);
					
				}
				else if (PreviousHittedComponent == HitGizmo->StaticMeshZ && HitGizmo->AttachedActor)
				{
					
					FVector CurrentLocation = HitGizmo->GetActorLocation();
					if (!HoldingAxis) {
						SelectedGizmo = HitGizmo;
						HoldingAxis = true;
						AxisSelected(3, HitGizmo);
						SelectedAxis = 3;
						axisholdingdiff = CurrentHitLocation.Z - CurrentLocation.Z;
						HitGizmo->SetActorLocation(CurrentLocation);
					}
					FVector NewLocation = FVector(CurrentLocation.X , CurrentLocation.Y, CurrentLocation.Z - axisholdingdiff); // Adjust the translation amount as needed
					HitGizmo->SetActorLocation(NewLocation);
					
				}
				return;
				
			}
			return;
		}
		
		
	}
	if (SelectedActors.Find(CurrentHittingActor) && !RecentlyLeftClicked && !HoldingAxis) {
		if (!CtrlClicked && !ShiftClicked) {
			TArray<AActor*> AttachedActors;
			for (int i = 0; i < SelectedGizmoArr.Num(); i++) {
				SelectedGizmoArr[i]->GetAttachedActors(AttachedActors);
				for (int j = 0; j < AttachedActors.Num(); j++) {
					if (AttachedActors[j]) {
						AttachedActors[j]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
					}
				}
				SelectedGizmoArr[i]->Destroy();
				SelectedGizmoArr[i] = nullptr;
			}
			PreviousSelectedGizmo = nullptr;
			SelectedActors.Empty();
			SelectedGizmoArr.Empty();
			SelectedActorArr.Empty();
		}
		if ((CtrlClicked && !ShiftClicked) || (!CtrlClicked && ShiftClicked)) {
			if (SelectedGizmoArr.Num() > 1) {
				for (int i = 0; i < SelectedGizmoArr.Num(); i++) {
					if (CurrentHittingActor == SelectedGizmoArr[i]->AttachedActor) {
						SelectedGizmoArr[i]->AttachedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
						SelectedActorArr.Remove(SelectedGizmoArr[i]->AttachedActor);
						SelectedActors.Remove(SelectedGizmoArr[i]->AttachedActor);
						SelectedGizmoArr.RemoveAt(i);
						SelectedGizmoArr[i]->Destroy();
						break;
						
					}
				}
				PreviousSelectedGizmo = SelectedGizmoArr[SelectedGizmoArr.Num() - 1];
				return;
			}
			else {
				
				for (int i = 0; i < SelectedActorArr.Num() ; i++) {
					SelectedActorArr[i]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
					if (SelectedActorArr[i] == CurrentHittingActor) {
						SelectedActorArr.RemoveAt(i);
						SelectedActors.Remove(CurrentHittingActor);
						break;
					}
				}
				PreviousSelectedGizmo->Destroy();
				PreviousSelectedGizmo = nullptr;
				SelectedGizmoArr.Empty();

				float x = 0.f;
				float y = 0.f;
				float z = 0.f;

				float avg1 = 0.f;
				float avg2 = 0.f;
				float avg3 = 0.f;
				for (int i = 0; i < SelectedActorArr.Num(); i++) {
					x += SelectedActorArr[i]->GetActorLocation().X;
					y += SelectedActorArr[i]->GetActorLocation().Y;
					z += SelectedActorArr[i]->GetActorLocation().Z;
				}
				avg1 = x / SelectedActorArr.Num();
				avg2 = y / SelectedActorArr.Num();
				avg3 = z / SelectedActorArr.Num();
				
			

				FVector ResultVector = FVector(avg1, avg2, avg3);
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = GetInstigator();
				TSubclassOf<AGizmo> BlueprintClass = LoadClass<AGizmo>(nullptr, TEXT("/Game/BP_Gizmo.BP_Gizmo_C"));
				FVector SpawnLocation = ResultVector;
				FRotator SpawnRotation = FRotator::ZeroRotator;
				UWorld* World = GetWorld();
				AGizmo* SpawnedActor = World->SpawnActor<AGizmo>(BlueprintClass, SpawnLocation, SpawnRotation, SpawnParams);

				if (SpawnedActor)
				{
					FString ActorName = SpawnedActor->GetName();
					FString SpawnLocationString = SpawnLocation.ToString();
					for (int i = 0; i < SelectedActorArr.Num(); i++) {
						SelectedActorArr[i]->AttachToActor(SpawnedActor, FAttachmentTransformRules::KeepWorldTransform);
					}
					PreviousSelectedGizmo = SpawnedActor;
					SelectedGizmoArr.Add(SpawnedActor);
					SpawnedActor->AttachedActor = CurrentHittingActor;
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("down to up"));
					RecentlySelected();
					
				}

				
				return;
			}
		}
	
	

	}
	if (CurrentHittingActor && !SelectedActors.Find(CurrentHittingActor) && !HoldingAxis && !RecentlyLeftClicked) {
		if (!CtrlClicked && !ShiftClicked) {
			
				TArray<AActor*> AttachedActors;
				for (int i = 0; i < SelectedGizmoArr.Num(); i++) {
					SelectedGizmoArr[i]->GetAttachedActors(AttachedActors);
					for (int j = 0; j < AttachedActors.Num(); j++) {
						AttachedActors[j]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
					}
					SelectedGizmoArr[i]->Destroy();
					SelectedGizmoArr[i] = nullptr;
				}
				PreviousSelectedGizmo = nullptr;
				SelectedActors.Empty();
				SelectedGizmoArr.Empty();
				SelectedActorArr.Empty();
	
		}
		if (CtrlClicked && !ShiftClicked) {
			if (PreviousSelectedGizmo) {
				
				for (UActorComponent* Component : CurrentHittingActor->GetComponents())
				{
					USceneComponent* SceneComponent = Cast<USceneComponent>(Component);
					if (SceneComponent)
					{
						
						SceneComponent->SetMobility(EComponentMobility::Movable);
					}
				}
				UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(CurrentHittingActor->GetRootComponent());
				if (PrimitiveComponent)
				{
					
					PrimitiveComponent->SetRenderCustomDepth(false);
					PrimitiveComponent->SetCustomDepthStencilValue(-1);
				}
				
				SelectedActorArr.Add(CurrentHittingActor);
				SelectedActors.Add(CurrentHittingActor);

				float x = 0.f;
				float y = 0.f;
				float z = 0.f;

				float avg1 = 0.f;
				float avg2 = 0.f;
				float avg3 = 0.f;
				for (int i = 0; i < SelectedActorArr.Num(); i++) {
					x += SelectedActorArr[i]->GetActorLocation().X;
					y += SelectedActorArr[i]->GetActorLocation().Y;
					z += SelectedActorArr[i]->GetActorLocation().Z;
				}
				avg1 = x / SelectedActorArr.Num();
				avg2 = y / SelectedActorArr.Num();
				avg3 = z / SelectedActorArr.Num();
				for (int i = 0; i < SelectedActorArr.Num() - 1; i++) {
					SelectedActorArr[i]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				}
				PreviousSelectedGizmo->Destroy();
				
				FVector ResultVector = FVector(avg1, avg2, avg3);
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = GetInstigator();
				TSubclassOf<AGizmo> BlueprintClass = LoadClass<AGizmo>(nullptr, TEXT("/Game/BP_Gizmo.BP_Gizmo_C"));
				FVector SpawnLocation = ResultVector;
				FRotator SpawnRotation = FRotator::ZeroRotator;
				UWorld* World = GetWorld();
				AGizmo* SpawnedActor = World->SpawnActor<AGizmo>(BlueprintClass, SpawnLocation, SpawnRotation, SpawnParams);

				if (SpawnedActor)
				{
					FString ActorName = SpawnedActor->GetName();
					FString SpawnLocationString = SpawnLocation.ToString();
					for (int i = 0; i < SelectedActorArr.Num() ; i++) {
						SelectedActorArr[i]->AttachToActor(SpawnedActor, FAttachmentTransformRules::KeepWorldTransform);
					}
					PreviousSelectedGizmo = SpawnedActor;
					RecentlySelected();
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("down to mid"));
				}
			
				SpawnedActor->AttachedActor = CurrentHittingActor;
				return;

			}
		}
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		for (UActorComponent* Component : CurrentHittingActor->GetComponents())
		{
			USceneComponent* SceneComponent = Cast<USceneComponent>(Component);
			if (SceneComponent)
			{
				
				SceneComponent->SetMobility(EComponentMobility::Movable);
			}
		}
		UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(CurrentHittingActor->GetRootComponent());
		TSubclassOf<AGizmo> BlueprintClass = LoadClass<AGizmo>(nullptr, TEXT("/Game/BP_Gizmo.BP_Gizmo_C"));
		if (PrimitiveComponent)
		{
			
			PrimitiveComponent->SetRenderCustomDepth(false);
			PrimitiveComponent->SetCustomDepthStencilValue(-1);
		}

		FVector SpawnLocation = CurrentHittingActor->GetActorLocation();
		FRotator SpawnRotation = FRotator::ZeroRotator;
		UWorld* World = GetWorld();

		AGizmo* SpawnedActor = World->SpawnActor<AGizmo>(BlueprintClass, SpawnLocation, SpawnRotation, SpawnParams);

		if (SpawnedActor)
		{
			FString ActorName = SpawnedActor->GetName();
			FString SpawnLocationString = SpawnLocation.ToString();



			
			
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, false);
			CurrentHittingActor->AttachToActor(SpawnedActor, AttachmentRules);

		
			
			



			PreviousSelectedGizmo = SpawnedActor;
			SelectedGizmoArr.Add(SpawnedActor);
			SelectedActors.Add(CurrentHittingActor);
			SelectedActorArr.Add(CurrentHittingActor);
			RecentlySelected();
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("down to earth"));
			SpawnedActor->AttachedActor = CurrentHittingActor;

			
		}
		
		
	}
	
			
		
	

}
void ARuntimeGizmosUE5Character::OnMultiSelectClick()
{
	//leftclicking = true;
	//GEngine->AddOnScreenDebugMessage(-1, 0.1, FColor::Red, TEXT("true"));
}
void ARuntimeGizmosUE5Character::OnMultiSelectRelease()
{
	//leftclicking = false;
	GEngine->AddOnScreenDebugMessage(-1, 0.1, FColor::Red, TEXT("false"));
}
//////////////////////////////////////////////////////////////////////////// Input

void ARuntimeGizmosUE5Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARuntimeGizmosUE5Character::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARuntimeGizmosUE5Character::Look);

		//EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ARuntimeGizmosUE5Character::OnLeftClick);

		//EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ARuntimeGizmosUE5Character::OnMultiSelectClick);

		//EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &ARuntimeGizmosUE5Character::OnMultiSelectRelease);
	}
}


void ARuntimeGizmosUE5Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		FRotator ControlRotation = GetControlRotation();
		FRotator CameraRotation = FirstPersonCameraComponent->GetComponentRotation();

		// Calculate the movement direction based on the camera's orientation
		FVector MovementDirection = FRotationMatrix(CameraRotation).GetScaledAxis(EAxis::X) * MovementVector.Y;
		MovementDirection += FRotationMatrix(CameraRotation).GetScaledAxis(EAxis::Y) * MovementVector.X;
		MovementDirection += FRotationMatrix(CameraRotation).GetScaledAxis(EAxis::Z) * MovementVector.Y;

		
		AddMovementInput(MovementDirection);
	}
}

void ARuntimeGizmosUE5Character::Look(const FInputActionValue& Value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("No Hit Result"));
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	

	if (Controller != nullptr && rightclicked)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

