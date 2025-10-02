// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnablePlayAndDelete, bool, EnableButton);


class UMVVM_Slot;
/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	void InitialLoadSlots();

	UPROPERTY(BlueprintAssignable)
	FEnablePlayAndDelete EnablePlayAndDelete;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_Slot> LoadSlotClass;

	UFUNCTION(BlueprintPure)
	UMVVM_Slot* GetLoadSlotViewModelByIndex(int32 Index) { return LoadSlots.FindChecked(Index); }

	UFUNCTION(BlueprintCallable)
	void NewSlotButtonClicked(int32 Slot, const FString& EnterName);

	UFUNCTION(BlueprintCallable)
	void NewGameButtonClicked(int32 Slot);	

	UFUNCTION(BlueprintCallable)
	void SelectButtonClicked(int32 Slot);

	UFUNCTION(BlueprintCallable)
	void DeleteSlot();

	UFUNCTION(BlueprintCallable)
	void PlayButtonClick();
	
	void SetLoadSlotName(const FString& SlotName);
	FString GetLoadSlotName() const {return LoadSlotName;}

	void LoadData();
private:
	
	UPROPERTY()
	TMap<int32, UMVVM_Slot*> LoadSlots;

	UPROPERTY()
	TObjectPtr<UMVVM_Slot> LoadSlot_0;

	UPROPERTY()
	TObjectPtr<UMVVM_Slot> LoadSlot_1;

	UPROPERTY()
	TObjectPtr<UMVVM_Slot> LoadSlot_2;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true"))
	FString LoadSlotName;

	UPROPERTY()
	TObjectPtr<UMVVM_Slot> SelectedSlot;
};
