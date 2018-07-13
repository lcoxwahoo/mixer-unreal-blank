// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "MixerLoginHUD.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class MIXBLANK_API SMixerLoginSlateWidget : public SCompoundWidget
{

public:
	SLATE_BEGIN_ARGS(SMixerLoginSlateWidget)
	{}

	// ++ We add a new argument called OwnerHUDArg
	SLATE_ARGUMENT(TWeakObjectPtr<class AMixerLoginHUD>, OwnerHUDArg);

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

//	void OnMixerLoginCleared(bool succeed);
//	FReply OnMixerOnAuthCodeReady(const FString&);

private:

	// ++ Pointer to our parent HUD. To make sure HUD's lifetime is controlled elsewhere, use "weak" ptr.
	// ++ HUD has a "strong" pointer to Widget, circular ownership would prevent/break self-destruction of hud/widget (cause a memory leak).
	TWeakObjectPtr<class AMixerLoginHUD> OwnerHUD;
};
