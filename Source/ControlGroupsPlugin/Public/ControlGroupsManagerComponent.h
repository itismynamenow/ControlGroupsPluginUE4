#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ControlGroupsManagerComponent.generated.h"

class USelectionComponent;
class USelectionManagerComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CONTROLGROUPSPLUGIN_API UControlGroupsManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UControlGroupsManagerComponent();

	UFUNCTION(BlueprintCallable)
	int TotalControlGroupsNum() const;
	UFUNCTION(BlueprintCallable)
	TArray<USelectionComponent*> GetGroup(int GroupId) const;
	UFUNCTION(BlueprintCallable)
	void SetGroup(int GroupId, TArray<USelectionComponent*> SelectionComponents);
	UFUNCTION(BlueprintCallable)
	void SelectGroup(const  int GroupIndex);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FKey m_ModifierKey = EKeys::LeftControl;

protected:
	virtual void BeginPlay() override;

private:
	APlayerController* GetPlayerController() const;
	USelectionManagerComponent* GetSelectionManager() const;
	void OnNumberKeyPressed();
	void SetSelectionAsGroup(const  int GroupIndex);
	bool IsKeyDown(const FKey& Key) const;
	int GetPressedNumberKeyIndex() const;
	static int InvalidPressedNumberKeyIndex();

private:
	const TArray<FKey> m_NumberKeys;
	TArray<TArray<USelectionComponent*>> m_ControlGroups;
};
