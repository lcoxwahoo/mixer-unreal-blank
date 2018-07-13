// Fill out your copyright notice in the Description page of Project Settings.

#include "MixerLoginHUD.h"
#include "SMixerLoginSlateWidget.h"
#include "Engine.h"
#include "Widgets/SWeakWidget.h"

void AMixerLoginHUD::BeginPlay()
{
	Super::BeginPlay();
	// So far only TSharedPtr<SMyUIWidget> has been created, now create the actual object.
	// Create a SMyUIWidget on heap, our MyUIWidget shared pointer provides handle to object
	// Widget will not self-destruct unless the HUD's SharedPtr (and all other SharedPtrs) are destroyed first.
	
	MyUIWidget = SNew(SMixerLoginSlateWidget).OwnerHUDArg(this);

	// Pass our viewport a weak ptr to our widget
	// Viewport's weak ptr will not give Viewport ownership of Widget
	GEngine->GameViewport->AddViewportWidgetContent(
		SNew(SWeakWidget)
		.PossiblyNullContent(MyUIWidget.ToSharedRef())
	);

	// Set widget's properties as visible (sets child widget's properties recursively)
	MyUIWidget->SetVisibility(EVisibility::Visible);

}
