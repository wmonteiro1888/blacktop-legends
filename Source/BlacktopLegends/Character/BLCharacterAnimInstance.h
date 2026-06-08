#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "InputActionValue.h"
#include "BLCharacterAnimInstance.generated.h"

class ABlCharacter;

/**
 * Animation state enum for state machine
 */
UENUM(BlueprintType)
enum class EBLMovementState : uint8
{
    Idle = 0        UMETA(DisplayName = "Idle"),
    Walk = 1        UMETA(DisplayName = "Walk"),
    Run = 2         UMETA(DisplayName = "Run"),
    Sprint = 3      UMETA(DisplayName = "Sprint")
};

/**
 * UBLCharacterAnimInstance
 * 
 * Manages animation state, blend spaces, and transitions for the streetballer character.
 * Updates based on movement input, ball possession, and gameplay events.
 */
UCLASS()
class BLACKTOPLEGENDS_API UBLCharacterAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    /**
     * Initialize animation instance
     */
    virtual void NativeInitializeAnimation() override;

    /**
     * Called every frame to update animation parameters
     */
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    /**
     * Called when animation blueprint is played
     */
    virtual void NativePostEvaluateAnimation() override;

public:
    // ===== Animation Parameters =====

    /**
     * Character's current movement speed (magnitude of velocity)
     */
    UPROPERTY(BlueprintReadOnly, Category = "Animation|Movement")
    float MovementSpeed;

    /**
     * Current movement state (Idle, Walk, Run, Sprint)
     */
    UPROPERTY(BlueprintReadOnly, Category = "Animation|Movement")
    EBLMovementState MovementState;

    /**
     * Direction input for blend spaces (-1 left to 1 right)
     */
    UPROPERTY(BlueprintReadOnly, Category = "Animation|Movement")
    float DirectionInput;

    /**
     * Forward input for blend spaces (-1 back to 1 forward)
     */
    UPROPERTY(BlueprintReadOnly, Category = "Animation|Movement")
    float ForwardInput;

    /**
     * Does character currently have the ball?
     */
    UPROPERTY(BlueprintReadOnly, Category = "Animation|Ball")
    bool bHasBall;

    /**
     * Is character currently dribbling?
     */
    UPROPERTY(BlueprintReadOnly, Category = "Animation|Ball")
    bool bIsDribbling;

    /**
     * Is character in the air (jumped)?
     */
    UPROPERTY(BlueprintReadOnly, Category = "Animation|Movement")
    bool bIsInAir;

    /**
     * Is character aiming to shoot?
     */
    UPROPERTY(BlueprintReadOnly, Category = "Animation|Ball")
    bool bIsAiming;

    /**
     * Animation playback rate based on stamina (0.8 to 1.2)
     */
    UPROPERTY(BlueprintReadOnly, Category = "Animation|Movement")
    float PlayRate;

    /**
     * Character's stamina percent (0-100)
     */
    UPROPERTY(BlueprintReadOnly, Category = "Animation|Movement")
    float StaminaPercent;

    /**
     * Is character in defensive stance?
     */
    UPROPERTY(BlueprintReadOnly, Category = "Animation|Movement")
    bool bIsDefending;

private:
    /**
     * Reference to owning BLCharacter
     */
    ABlCharacter* OwnerCharacter;

    /**
     * Previous frame's movement speed for smooth transitions
     */
    float PreviousMovementSpeed;

    /**
     * Update movement parameters based on character velocity
     */
    void UpdateMovementParameters();

    /**
     * Update ball-related parameters
     */
    void UpdateBallParameters();

    /**
     * Update stamina and performance parameters
     */
    void UpdateStaminaParameters();

    /**
     * Determine movement state from current speed
     */
    EBLMovementState GetMovementStateFromSpeed(float Speed);
};
