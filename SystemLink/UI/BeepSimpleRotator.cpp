// Fill out your copyright notice in the Description page of Project Settings.


#include "BeepSimpleRotator.h"

void UBeepSimpleRotator::NativePreConstruct()
{
	Super::NativePreConstruct();

	// Hard-coded options
	TArray<FText> Labels;
	Labels.Add(FText::FromString(TEXT("Low")));
	Labels.Add(FText::FromString(TEXT("Medium")));
	Labels.Add(FText::FromString(TEXT("High")));

	PopulateTextLabels(Labels);
	SetSelectedItem(0); // start at first item
}

void UBeepSimpleRotator::NativeConstruct()
{
	Super::NativeConstruct();

	// Click → rotate right
	OnClicked().AddUObject(this, &UBeepSimpleRotator::HandleClicked);
}

void UBeepSimpleRotator::HandleClicked()
{
	ShiftTextRight();
}
