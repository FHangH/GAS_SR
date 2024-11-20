#include "GAS_SR/Public/Utils/FuncLib_Utils.h"
#include "Data/SR_Types.h"

void UFuncLib_Utils::SetPlayerControllerInputMode(ESR_InputMode InputMode, APlayerController* PlayerController, const bool IsShowMouse)
{
	switch (InputMode)
	{
	case ESR_InputMode::OnlyUI:
		{
			FInputModeUIOnly InputModeUIOnly;
			InputModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetShowMouseCursor(IsShowMouse);
			PlayerController->SetInputMode(InputModeUIOnly);
			break;
		}
	case ESR_InputMode::OnlyGame:
		{
			FInputModeGameOnly InputModeGameOnly;
			InputModeGameOnly.SetConsumeCaptureMouseDown(false);
			PlayerController->SetShowMouseCursor(IsShowMouse);
			PlayerController->SetInputMode(InputModeGameOnly);
			break;
		}
	case ESR_InputMode::Both:
		{
			FInputModeGameAndUI InputModeGameAndUI;
			InputModeGameAndUI.SetHideCursorDuringCapture(!IsShowMouse);
			InputModeGameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
			PlayerController->SetShowMouseCursor(IsShowMouse);
			PlayerController->SetInputMode(InputModeGameAndUI);
			break;
		}
	}
}
