#include "BLCharacterAnimInstance.h"
#include "BlacktopLegends/Character/BLCharacter.h"
#include "BlacktopLegends/Game/BLPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/Vector.h"

void UBLCharacterAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    // Get owning character
    OwnerCharacter = Cast<ABlCharacter>(TryGetPawnOwner());
    
    // Initialize parameters
    MovementSpeed = 0.0f;
    MovementState = EBLMovementState::Idle;
    DirectionInput = 0.0f;
    ForwardInput = 0.0f;
    bHasBall = false;
    bIsDribbling = false;
    bIsInAir = false;
    bIsAiming = false;
    PlayRate = 1.0f;
    StaminaPercent = 100.0f;
    bIsDefending = false;
    PreviousMovementSpeed = 0.0f;
}

void UBLCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    // Validate owner character
    if (!IsValid(OwnerCharacter))
    {
        OwnerCharacter = Cast<ABlCharacter>(TryGetPawnOwner());
        if (!OwnerCharacter) return;
    }

    // Update all animation parameters
    UpdateMovementParameters();
    UpdateBallParameters();
    UpdateStaminaParameters();
}

void UBLCharacterAnimInstance::NativePostEvaluateAnimation()
{
    Super::NativePostEvaluateAnimation();
    
    // Store current speed for next frame
    PreviousMovementSpeed = MovementSpeed;
}

void UBLCharacterAnimInstance::UpdateMovementParameters()
{
    if (!IsValid(OwnerCharacter)) return;

    // Get character's current velocity
    FVector Velocity = OwnerCharacter->GetCharacterMovement()->Velocity;
    MovementSpeed = Velocity.Length();

    // Determine movement state based on speed
    MovementState = GetMovementStateFromSpeed(MovementSpeed);

    // Get input direction from character
    float InputX = 0.0f;
    float InputY = 0.0f;

    // This assumes BLCharacter stores input values; adjust as needed
    // For now, we'll calculate from velocity direction
    FVector ForwardDir = OwnerCharacter->GetActorForwardVector();
    FVector RightDir = OwnerCharacter->GetActorRightVector();
    FVector NormalizedVelocity = Velocity.GetSafeNormal();

    // Calculate blend space inputs
    DirectionInput = FVector::DotProduct(NormalizedVelocity, RightDir);
    ForwardInput = FVector::DotProduct(NormalizedVelocity, ForwardDir);

    // Clamp to -1 to 1
    DirectionInput = FMath::Clamp(DirectionInput, -1.0f, 1.0f);
    ForwardInput = FMath::Clamp(ForwardInput, -1.0f, 1.0f);

    // Get character movement status
    bIsInAir = OwnerCharacter->GetCharacterMovement()->IsFalling();
}

void UBLCharacterAnimInstance::UpdateBallParameters()
{
    if (!IsValid(OwnerCharacter)) return;

    // Get ball possession from character
    bHasBall = OwnerCharacter->IsBallHeld();
    bIsDribbling = OwnerCharacter->IsDribbling();
    
    // Get aiming state (would be set from gameplay controller)
    // For now, just check if aiming animation should play
    bIsAiming = OwnerCharacter->IsAiming();
}

void UBLCharacterAnimInstance::UpdateStaminaParameters()
{
    if (!IsValid(OwnerCharacter)) return;

    // Get stamina from character
    float CurrentStamina = OwnerCharacter->GetStamina();
    float MaxStamina = OwnerCharacter->GetMaxStamina();
    
    StaminaPercent = (MaxStamina > 0.0f) ? (CurrentStamina / MaxStamina) * 100.0f : 100.0f;

    // Adjust animation playback speed based on fatigue
    // Lower stamina = slower animations (fatigue effect)
    float StaminaRatio = StaminaPercent / 100.0f;
    PlayRate = FMath::Lerp(0.8f, 1.2f, StaminaRatio);
}

EBLMovementState UBLCharacterAnimInstance::GetMovementStateFromSpeed(float Speed)
{
    // Thresholds (adjust in game settings if needed)
    const float WALK_THRESHOLD = 50.0f;    // 0.5 m/s
    const float RUN_THRESHOLD = 300.0f;    // 3.0 m/s
    const float SPRINT_THRESHOLD = 600.0f; // 6.0 m/s

    if (Speed < WALK_THRESHOLD)
    {
        return EBLMovementState::Idle;
    }
    else if (Speed < RUN_THRESHOLD)
    {
        return EBLMovementState::Walk;
    }
    else if (Speed < SPRINT_THRESHOLD)
    {
        return EBLMovementState::Run;
    }
    else
    {
        return EBLMovementState::Sprint;
    }
}
