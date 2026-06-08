#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "BLCharacterController.generated.h"

class ABlCharacter;
class UInputComponent;
class UInputMappingContext;
class UInputAction;
class UEnhancedInputComponent;
class UEnhancedInputSubsystems;

/**
 * UBLCharacterController
 * 
 * Handles player input for streetballer character.
 * Maps input to movement, ball handling, shooting, and special abilities.
 */
UCLASS()
class BLACKTOPLEGENDS_API ABLCharacterController : public APlayerController
{
    GENERATED_BODY()

public:
    ABLCharacterController();

    /**
     * Setup input for this controller
     */
    virtual void BeginPlay() override;

    /**
     * Setup input component and bind actions
     */
    virtual void SetupInputComponent() override;

private:
    // ===== Input System =====

    /**
     * Input mapping context for character controls
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputMappingContext* InputMappingContext;

    /**
     * Movement input action
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* MoveAction;

    /**
     * Look input action
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* LookAction;

    /**
     * Jump input action
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* JumpAction;

    /**
     * Shoot input action
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* ShootAction;

    /**
     * Pass input action
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* PassAction;

    /**
     * Dribble toggle input action
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* DribbleToggleAction;

    /**
     * Sprint input action
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* SprintAction;

    // ===== Callback Functions =====

    /**
     * Move callback
     */
    void Move(const FInputActionValue& Value);

    /**
     * Look callback
     */
    void Look(const FInputActionValue& Value);

    /**
     * Jump callback
     */
    void OnJump();
    void OnStopJump();

    /**
     * Shoot callback
     */
    void OnShoot();
    void OnStopShoot();

    /**
     * Pass callback
     */
    void OnPass();

    /**
     * Dribble toggle callback
     */
    void OnToggleDribble();

    /**
     * Sprint callbacks
     */
    void OnStartSprint();
    void OnStopSprint();

    /**
     * Possesses character
     */
    virtual void OnPossess(APawn* InPawn) override;
};
