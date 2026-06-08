#include "BlacktopLegends/Character/BLCharacterController.h"
#include "BlacktopLegends/Character/BLCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "GameFramework/CharacterMovementComponent.h"

ABLCharacterController::ABLCharacterController()
    : Super()
{
    // Default values
}

void ABLCharacterController::BeginPlay()
{
    Super::BeginPlay();

    // Add Input Mapping Context
    if (UEnhancedInputSubsystems* InputSubsystem = 
        GetLocalPlayer()->GetSubsystem<UEnhancedInputSubsystems>())
    {
        InputSubsystem->AddMappingContext(InputMappingContext, 0);
    }
}

void ABLCharacterController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInputComponent = 
        CastChecked<UEnhancedInputComponent>(InputComponent))
    {
        // Movement
        EnhancedInputComponent->BindAction(
            MoveAction,
            ETriggerEvent::Triggered,
            this,
            &ABLCharacterController::Move
        );

        // Looking
        EnhancedInputComponent->BindAction(
            LookAction,
            ETriggerEvent::Triggered,
            this,
            &ABLCharacterController::Look
        );

        // Jumping
        EnhancedInputComponent->BindAction(
            JumpAction,
            ETriggerEvent::Started,
            this,
            &ABLCharacterController::OnJump
        );
        EnhancedInputComponent->BindAction(
            JumpAction,
            ETriggerEvent::Completed,
            this,
            &ABLCharacterController::OnStopJump
        );

        // Shooting
        EnhancedInputComponent->BindAction(
            ShootAction,
            ETriggerEvent::Started,
            this,
            &ABLCharacterController::OnShoot
        );
        EnhancedInputComponent->BindAction(
            ShootAction,
            ETriggerEvent::Completed,
            this,
            &ABLCharacterController::OnStopShoot
        );

        // Passing
        EnhancedInputComponent->BindAction(
            PassAction,
            ETriggerEvent::Started,
            this,
            &ABLCharacterController::OnPass
        );

        // Dribble Toggle
        EnhancedInputComponent->BindAction(
            DribbleToggleAction,
            ETriggerEvent::Started,
            this,
            &ABLCharacterController::OnToggleDribble
        );

        // Sprinting
        EnhancedInputComponent->BindAction(
            SprintAction,
            ETriggerEvent::Started,
            this,
            &ABLCharacterController::OnStartSprint
        );
        EnhancedInputComponent->BindAction(
            SprintAction,
            ETriggerEvent::Completed,
            this,
            &ABLCharacterController::OnStopSprint
        );
    }
}

void ABLCharacterController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // Cast to BLCharacter
    ABlCharacter* ControlledCharacter = Cast<ABlCharacter>(InPawn);
    if (ControlledCharacter)
    {
        // Controller now possesses the BLCharacter
        // Any initialization needed here
    }
}

void ABLCharacterController::Move(const FInputActionValue& Value)
{
    ABlCharacter* ControlledCharacter = Cast<ABlCharacter>(GetPawn());
    if (!ControlledCharacter) return;

    const FVector2D MovementVector = Value.Get<FVector2D>();

    // Move forward/backward
    if (MovementVector.Y != 0.0f)
    {
        ControlledCharacter->AddMovementInput(
            ControlledCharacter->GetActorForwardVector(),
            MovementVector.Y
        );
    }

    // Move left/right
    if (MovementVector.X != 0.0f)
    {
        ControlledCharacter->AddMovementInput(
            ControlledCharacter->GetActorRightVector(),
            MovementVector.X
        );
    }
}

void ABLCharacterController::Look(const FInputActionValue& Value)
{
    const FVector2D LookAxisVector = Value.Get<FVector2D>();

    // Add yaw and pitch input to camera
    if (LookAxisVector.X != 0.0f)
    {
        AddYawInput(LookAxisVector.X);
    }

    if (LookAxisVector.Y != 0.0f)
    {
        AddPitchInput(LookAxisVector.Y);
    }
}

void ABLCharacterController::OnJump()
{
    ABlCharacter* ControlledCharacter = Cast<ABlCharacter>(GetPawn());
    if (ControlledCharacter)
    {
        ControlledCharacter->Jump();
    }
}

void ABLCharacterController::OnStopJump()
{
    ABlCharacter* ControlledCharacter = Cast<ABlCharacter>(GetPawn());
    if (ControlledCharacter)
    {
        ControlledCharacter->StopJumping();
    }
}

void ABLCharacterController::OnShoot()
{
    ABlCharacter* ControlledCharacter = Cast<ABlCharacter>(GetPawn());
    if (ControlledCharacter)
    {
        ControlledCharacter->RequestShoot();
    }
}

void ABLCharacterController::OnStopShoot()
{
    ABlCharacter* ControlledCharacter = Cast<ABlCharacter>(GetPawn());
    if (ControlledCharacter)
    {
        ControlledCharacter->CancelShoot();
    }
}

void ABLCharacterController::OnPass()
{
    ABlCharacter* ControlledCharacter = Cast<ABlCharacter>(GetPawn());
    if (ControlledCharacter)
    {
        ControlledCharacter->RequestPass();
    }
}

void ABLCharacterController::OnToggleDribble()
{
    ABlCharacter* ControlledCharacter = Cast<ABlCharacter>(GetPawn());
    if (ControlledCharacter)
    {
        ControlledCharacter->ToggleDribble();
    }
}

void ABLCharacterController::OnStartSprint()
{
    ABlCharacter* ControlledCharacter = Cast<ABlCharacter>(GetPawn());
    if (ControlledCharacter)
    {
        ControlledCharacter->StartSprinting();
    }
}

void ABLCharacterController::OnStopSprint()
{
    ABlCharacter* ControlledCharacter = Cast<ABlCharacter>(GetPawn());
    if (ControlledCharacter)
    {
        ControlledCharacter->StopSprinting();
    }
}
