// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
		MovementComponent->GravityScale = 0.0f; // 중력 비활성화
		MovementComponent->AirControl = 1.0f;  // 공중에서 이동 조절 활성화
		MovementComponent->JumpZVelocity = 0.0f; // 점프 속도 비활성화
	}
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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
		// 좌클릭 액션 바인딩
		Input->BindAction(LeftClickAction, ETriggerEvent::Started, this, &AMyCharacter::OnLeftClick);
		Input->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &AMyCharacter::OnLeftClick);

		// 우클릭 액션 바인딩
		Input->BindAction(RightClickAction, ETriggerEvent::Started, this, &AMyCharacter::OnRightClick);
		Input->BindAction(RightClickAction, ETriggerEvent::Completed, this, &AMyCharacter::OnRightClick);

		// 이동 액션 바인딩
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::QuaterMove);

		//드래그 액션
		Input->BindAction(UpDownAction, ETriggerEvent::Triggered, this, &AMyCharacter::UpDown);

	}

}

void AMyCharacter::QuaterMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (bIsLeftClicking)
	{
		ensure(Camera);
		// 좌클릭 상태에서의 이동 처리
		if (Camera)
		{
			FVector CameraForward = Camera->GetForwardVector();
			FVector CameraRight = Camera->GetRightVector();

			FVector MoveDirection = CameraForward * MovementVector.Y + CameraRight * MovementVector.X;
			MoveDirection.Z = 0.0f;

			FVector CurrentLocation = GetActorLocation();
			AddMovementInput(MoveDirection, MovementVector.Size());


			FString DebugMessage = FString::Printf(TEXT("Move with Left Clicks (%f:%f)"), MovementVector.X, MovementVector.Y);
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, DebugMessage);
			}
		}
	}

	if (bIsRightClicking)
	{
		// 우클릭 상태에서의 화면 회전 처리
		APlayerController* PlayerController = Cast<APlayerController>(Controller);
		if (PlayerController)
		{
			FRotator NewRotation = PlayerController->GetControlRotation();
			NewRotation.Yaw += MovementVector.X;
			NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + MovementVector.Y, -89.0f, 60.0f);
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
	float Height =FMath::Clamp(GetActorLocation().Z * 0.2,0,500);
	float Power = Value.Get<float>();
	FVector CurrentLocation = GetActorLocation();
	FVector NewLocation = CurrentLocation + FVector(0.0f, 0.0f, Power * Height); // 이동 속도를 조정합니다.

	SetActorLocation(NewLocation, true); // true는 충돌을 고려하여 위치를 설정합니다.

	FString DebugMessage = FString::Printf(TEXT("[Drag Move]Current Height (%f)"), float(GetActorLocation().Z));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, DebugMessage);
	}


}

