// Copyright Hoon


#include "..\..\..\Public\UI\WidgetController\AuraWidgetController.h"

void UAuraWidgetObject::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributesSet = WCParams.AttributeSet;
}
