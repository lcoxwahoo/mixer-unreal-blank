// Fill out your copyright notice in the Description page of Project Settings.

#include "SMixerLoginSlateWidget.h"
#include "SlateOptMacros.h"
#include "SMixerLoginPane.h"
#include <MixerInteractivityModule.h>
#include <MixerInteractivityTypes.h>

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

// ++ This is needed in order to use the localization macro LOCTEXT
#define LOCTEXT_NAMESPACE "SMixerLoginSlateWidget"

void SMixerLoginSlateWidget::Construct(const FArguments& InArgs) 
{
	// ++ Asign the argument to our local variable
	// name will be _OwnerHUDArg instead of OwnerHUDArg, see comments about SLATE_ARGUMENT before
	OwnerHUD = InArgs._OwnerHUDArg;

	// ++ Add all this Slate code
	// If the code below doesn't look like C++ to you it's because it (sort-of) isn't,
	// Slate makes extensive use of the C++ Prerocessor(macros) and operator overloading,
	// Epic is trying to make our lives easier, look-up the macro/operator definitions to see why.
	TSharedPtr<const FUniqueNetId> NetId = nullptr; // Get net id for current user

	ChildSlot
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			// Inside lies a text block with these settings
			SNew(SMixerLoginPane)
			.AllowSilentLogin(true)
			.UserId(NetId)
	//	.OnUIFlowFinished(this, &SMixerLoginSlateWidget::OnMixerLoginCleared)
	//	.OnAuthCodeReady(this, &SMixerLoginSlateWidget::OnMixerOnAuthCodeReady)
		]
		];

	//ChildSlot
	//	.VAlign(VAlign_Fill)
	//	.HAlign(HAlign_Fill)
	//	[
	//		SNew(SOverlay)
	//		+ SOverlay::Slot()
	//	.VAlign(VAlign_Top)
	//	.HAlign(HAlign_Right)
	//	[
	//		// Inside lies a text block with these settings
	//		SNew(STextBlock)
	//		.ShadowColorAndOpacity(FLinearColor::Black)
	//	.ColorAndOpacity(FLinearColor::Red)
	//	.ShadowOffset(FIntPoint(-1, 1))
	//	.Font(FSlateFontInfo("Veranda", 16))
	//	// localized text to be translated with a generic name HelloSlateText
	//	.Text(LOCTEXT("HelloSlateText", "Look MENU UI! AND IT COMPILES IN VS!"))
	//	]
	//	];
}

// ++ We need to undefine this namespace after we finish creating the Slate widget
#undef LOCTEXT_NAMESPACE


END_SLATE_FUNCTION_BUILD_OPTIMIZATION
//
//void SMixerLoginSlateWidget::OnMixerLoginCleared(bool succeed)
//{
//	
//}
//FReply SMixerLoginSlateWidget::OnMixerOnAuthCodeReady(const FString&)
//{
//	return FReply::Unhandled();
//}
