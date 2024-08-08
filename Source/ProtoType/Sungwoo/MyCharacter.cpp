// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"


// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;

	FRotator CameraRotation = FRotator(-90.0f, 0.0f, 0.0f); // Pitch -90 degrees to look downwards
	Camera->SetRelativeRotation(CameraRotation);

	ensure(Camera);

	bIsLeftClicking = false;
	bIsRightClicking = false;


	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		MovementComponent->GravityScale = 0.0f; // �߷� ��Ȱ��ȭ
		MovementComponent->AirControl = 1.0f;  // ���߿��� �̵� ���� Ȱ��ȭ
		MovementComponent->JumpZVelocity = 0.0f; // ���� �ӵ� ��Ȱ��ȭ
	}
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//MyTCPModule.TCPCunnect();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// ��Ŭ�� �׼� ���ε�
		Input->BindAction(LeftClickAction, ETriggerEvent::Started, this, &AMyCharacter::OnLeftClick);
		Input->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &AMyCharacter::OnLeftClick);

		// ��Ŭ�� �׼� ���ε�
		Input->BindAction(RightClickAction, ETriggerEvent::Started, this, &AMyCharacter::OnRightClick);
		Input->BindAction(RightClickAction, ETriggerEvent::Completed, this, &AMyCharacter::OnRightClick);

		// �̵� �׼� ���ε�
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::QuaterMove);

		//�巡�� �׼�
		Input->BindAction(UpDownAction, ETriggerEvent::Triggered, this, &AMyCharacter::UpDown);

	}

}

void AMyCharacter::RayCast(const FVector& StartLocation, const FVector& EndLocation, int32 FloorInfo)
{
	UObject* WorldContextObject = GetWorld();
	if (!WorldContextObject) return;

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	bool bHit = WorldContextObject->GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);

	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 5.0f, 0, 1.0f);

	if (bHit)
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		AActor* HitActor = HitResult.GetActor();

		if (HitActor)
		{
			UStaticMeshComponent* HitStaticMesh = Cast<UStaticMeshComponent>(HitActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
			if (HitStaticMesh)
			{

				UMaterialInterface* Material = HitStaticMesh->GetMaterial(0);

				if (Material)
				{
					UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
					if (DynamicMaterialInstance)
					{
						// ��Ƽ���� �Ӽ� ���� (��: ����)
						FLinearColor NewColor = FLinearColor::Red;
						DynamicMaterialInstance->SetVectorParameterValue(FName("Color"), NewColor);

						// ����ƽ �޽� ������Ʈ�� ��Ƽ���� ����
						HitStaticMesh->SetMaterial(0, DynamicMaterialInstance);

						UE_LOG(LogTemp, Warning, TEXT("Material applied successfully."));
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Failed to create dynamic material instance."));
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No static mesh component found on the hit actor."));
			}
		}

		DrawDebugLine(GetWorld(), HitResult.Location + FVector(0, 0, 10000.0f), HitResult.Location, FColor::Green, false, 5.0f, 0, 1.0f);
	}
}


void AMyCharacter::QuaterMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (bIsLeftClicking)
	{
		ensure(Camera);
		// ��Ŭ�� ���¿����� �̵� ó��
		if (Camera)
		{

			float Height = FMath::Clamp(GetActorLocation().Z * 0.005, 0, 500);

			FVector CameraForward = Camera->GetForwardVector();
			FVector CameraRight = Camera->GetRightVector();

			FVector MoveDirection = CameraForward * MovementVector.Y + CameraRight * MovementVector.X;
			MoveDirection.Z = 0.0f;

			FVector CurrentLocation = GetActorLocation();
			FVector NewLocation = CurrentLocation + (MoveDirection * Height);

			SetActorLocation(NewLocation, true);

			//AddMovementInput(MoveDirection, MovementVector.Size());


			FString DebugMessage = FString::Printf(TEXT("Move with Left Clicks (%f:%f)"), MovementVector.X, MovementVector.Y);
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, DebugMessage);
			}
		}
	}

	if (bIsRightClicking)
	{
		// ��Ŭ�� ���¿����� ȭ�� ȸ�� ó��
		APlayerController* PlayerController = Cast<APlayerController>(Controller);
		if (PlayerController)
		{
			FRotator NewRotation = PlayerController->GetControlRotation();
			NewRotation.Yaw += MovementVector.X;
			NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + MovementVector.Y, -89.0f, 0.0f);
			PlayerController->SetControlRotation(NewRotation);

			FString DebugMessage = FString::Printf(TEXT("Look with Right Click (%f:%f)"), MovementVector.X, MovementVector.Y);
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, DebugMessage);
			}
		}
	}

}

void AMyCharacter::OnLeftClick(const FInputActionValue& Value)
{
	bIsLeftClicking = Value.Get<bool>();
	FString DebugMessage = FString::Printf(TEXT("Left Click %s"), bIsLeftClicking ? TEXT("Pressed") : TEXT("Released"));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, DebugMessage);
	}
}

void AMyCharacter::OnRightClick(const FInputActionValue& Value)
{
	bIsRightClicking = Value.Get<bool>();
	FString DebugMessage = FString::Printf(TEXT("Right Click %s"), bIsRightClicking ? TEXT("Pressed") : TEXT("Released"));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, DebugMessage);
	}
}

void AMyCharacter::UpDown(const FInputActionValue& Value)
{
	float Height =FMath::Clamp(GetActorLocation().Z * 0.1,0,1000);
	float Power = Value.Get<float>();
	FVector CurrentLocation = GetActorLocation();
	FVector NewLocation = CurrentLocation + FVector(0.0f, 0.0f, Power * Height); // �̵� �ӵ��� �����մϴ�.

	SetActorLocation(NewLocation, true); // true�� �浹�� ����Ͽ� ��ġ�� �����մϴ�.

	FString DebugMessage = FString::Printf(TEXT("[Drag Move]Current Height (%f)"), float(GetActorLocation().Z));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, DebugMessage);
	}


}

FViewLocation AMyCharacter::GetCornerPoints()
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController)
	{
		return FViewLocation();

	}
	FViewLocation ViewLocation;

	// ȭ�� �𼭸� ��ǥ
	const FVector2D ScreenCorners[4] = {
		FVector2D(0, 0), // Left-up
		FVector2D(0, GEngine->GameViewport->Viewport->GetSizeXY().Y), // Left-down
		FVector2D(GEngine->GameViewport->Viewport->GetSizeXY().X, 0), // Right-up
		FVector2D(GEngine->GameViewport->Viewport->GetSizeXY().X, GEngine->GameViewport->Viewport->GetSizeXY().Y) // Right-down
	};

	FHitResult HitResult;
	FVector WorldLocation;
	FVector WorldDirection;

	// �� ȭ�� �𼭸��� ���� ó��
	for (int32 i = 0; i < 4; i++)
	{
		if (PlayerController->DeprojectScreenPositionToWorld(ScreenCorners[i].X, ScreenCorners[i].Y, WorldLocation, WorldDirection))
		{
			// �ٴڰ��� �������� ã�� ���� ���� Ʈ���̽� ����
			FVector Start = WorldLocation;
			FVector End = WorldLocation + (WorldDirection * 1000000.0f); // ����� �� �Ÿ�

			// ���� Ʈ���̽�, �ٴ� ������Ʈ�� �������
			if (GetWorld()->LineTraceSingleByObjectType(
				HitResult,
				Start,
				End,
				FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic)
			))
			{
				// ��Ʈ ����� ����
				switch (i)
				{
				case 0 :
					ViewLocation.LeftUp = FVector2D(HitResult.Location);
					break;
				case 1:
					ViewLocation.LeftDown = FVector2D(HitResult.Location);
					break;
				case 2:
					ViewLocation.RightUp = FVector2D(HitResult.Location);
					break;
				case 3:
					ViewLocation.RightDown = FVector2D(HitResult.Location);
					break;
				default:
					break;
				}

				// ����� ���� �׸���
				DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red, false, 5.0f, 0, 1.0f);
				DrawDebugPoint(GetWorld(), HitResult.Location, 10.0f, FColor::Green, false, 5.0f);
			}
		}
	}
	GetPoint(ViewLocation.LeftUp, ViewLocation.LeftDown, ViewLocation.RightUp, ViewLocation.RightDown);
	return ViewLocation;
}

void AMyCharacter::TempRayCast()
{
	FVector StartLocation = GetActorLocation();
	FVector Direction = GetActorForwardVector();
	FVector EndLocation = StartLocation + (Direction * 1000.0f);
	RayCast(StartLocation, EndLocation);
}

void AMyCharacter::GetPoint(FVector2D LU, FVector2D LD, FVector2D RU, FVector2D RD)
{

}



