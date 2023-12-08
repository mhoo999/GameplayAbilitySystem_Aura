// Copyright Hoon


#include "..\..\..\Public\UI\WidgetController\AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributesSet = WCParams.AttributeSet;
}

void UAuraWidgetController::BroadcastInintialValues()
{
	
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
}
