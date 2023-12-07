// Copyright Hoon


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	// 커서 히트를 담는 변수 선언
	FHitResult CursorHit;
	// 열거형 ECC_Visibility를 Hit, 복작한 Trace 사용 false, 적중 결과는 커서 적중
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	// Hit되는 Actor를 반환<IEnemyInterface 타입이 아니면 NULL을 반환>
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/**
	 * Line trace cursor. There are several scenarios:
	 *	A. LastActor is null && ThisActor is null
	 *		- Do nothing.
	 *	B. LastActor is null && ThisActor is valid
	 *		- Highlight ThisActor
	 *	C. LastActor is Valid && ThisActor is null
	 *		- UnHighlight LastActor
	 *	D. Both actors art valid, but LastActor != ThisActor
	 *		- UnHighlight LastActor, and Highlight ThisActor
	 *	E. Both actors are valid, and are the same actor
	 *		- Do nothing
	 */

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			// Case B
			ThisActor->HighlightActor();
		}
		else
		{
			// Case A - both are null, di nothing
		}
	}
	else // LastActor is valid
	{
		if (ThisActor == nullptr)
		{
			// Case C
			LastActor->UnHighlightActor();
		}
		else // both actors are valid
		{
			if (LastActor != ThisActor)
			{
				// Case D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else
			{
				// Case E - Do nothing
			}
		}
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	// 기본 마우스 커서를 표시
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	// 게임과 UI 입력모드 구조체 선언
	FInputModeGameAndUI InputModeData;
	// 커서를 뷰포트에 고정하지 않음
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	// 캡처하는 동안 커서를 숨기지 않음
	InputModeData.SetHideCursorDuringCapture(false);
	// 위의 입력 모드 데이터를 사용하기 위해 플레이어 컨트롤러 구성
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	// .Get 필요한 형태로 값을 얻을 수 있게 해주는 함수
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	// 컨트롤러의 회전값을 얻고, 해당 컨트롤의 Pitch와 Roll 값을 0으로 초기화 
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	// 각 축 기준 전방 벡터를 구함
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// 제어되는 Pawn이 NULL일 경우 아무것도 하지 않기 위해 if문 사용
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}