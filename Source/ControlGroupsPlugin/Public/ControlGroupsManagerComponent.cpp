#include "ControlGroupsManagerComponent.h"

#include "SelectionComponent.h"
#include "SelectionManagerComponent.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/PlayerController.h"

UControlGroupsManagerComponent::UControlGroupsManagerComponent()
	: m_NumberKeys{
		EKeys::Zero,
		EKeys::One,
		EKeys::Two,
		EKeys::Three,
		EKeys::Four,
		EKeys::Five,
		EKeys::Six,
		EKeys::Seven,
		EKeys::Eight,
		EKeys::Nine
	}
{
	PrimaryComponentTick.bCanEverTick = false;
	for (int i = 0; i < m_NumberKeys.Num(); ++i)
		m_ControlGroups.Add(TArray<USelectionComponent*>());
}

int UControlGroupsManagerComponent::TotalControlGroupsNum() const
{
	return m_NumberKeys.Num();
}

TArray<USelectionComponent*> UControlGroupsManagerComponent::GetGroup(int GroupId) const
{
	if (GroupId < 0 || GroupId >= m_ControlGroups.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("GroupId is not valid %i"), GroupId);
		return TArray<USelectionComponent*>();
	}

	return m_ControlGroups[GroupId];
}

void UControlGroupsManagerComponent::SetGroup(int GroupId, TArray<USelectionComponent*> SelectionComponents)
{
	if (GroupId < 0 || GroupId >= m_ControlGroups.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("GroupId is not valid %i"), GroupId);
		return;
	}

	m_ControlGroups[GroupId] = SelectionComponents;
}

void UControlGroupsManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	auto PlayerController = GetPlayerController();
	if (!IsValid(PlayerController))
		return;

	auto InputComponent = PlayerController->InputComponent;
	if (!IsValid(InputComponent))
		return;

	for (const auto Key : m_NumberKeys)
		InputComponent->BindKey(Key, IE_Pressed, this, &UControlGroupsManagerComponent::OnNumberKeyPressed);
}

APlayerController* UControlGroupsManagerComponent::GetPlayerController() const
{
	auto World = GetWorld();
	if (!IsValid(World))
		return nullptr;

	return World->GetFirstPlayerController();
}

void UControlGroupsManagerComponent::SetSelectionAsGroup(const int GroupIndex)
{
	auto SelectionManager = GetSelectionManager();
	if (!IsValid(SelectionManager))
	{
		UE_LOG(LogTemp, Error, TEXT("SelectionManager is not valid"));
		return;
	}

	SetGroup(GroupIndex, SelectionManager->GetSelectedSelectionComponents());
}

void UControlGroupsManagerComponent::OnNumberKeyPressed()
{

	const auto GroupIndex = GetPressedNumberKeyIndex();
	if (GroupIndex == InvalidPressedNumberKeyIndex())
		return;

	if (IsKeyDown(m_ModifierKey))
		SetSelectionAsGroup(GroupIndex);
	else
		SelectGroup(GroupIndex);
}

void UControlGroupsManagerComponent::SelectGroup(const int GroupIndex)
{
	auto SelectionManager = GetSelectionManager();
	if (!IsValid(SelectionManager))
	{
		UE_LOG(LogTemp, Error, TEXT("SelectionManager is not valid"));
		return;
	}

	SelectionManager->UnselectAll();

	for (auto SelectionComponent : GetGroup(GroupIndex))
	{
		if (!IsValid(SelectionComponent))
			continue;

		SelectionComponent->SetSelected(true);
	}
}

int UControlGroupsManagerComponent::GetPressedNumberKeyIndex() const
{
	for (int i=0;i<m_NumberKeys.Num();++i)
	{
		const auto Key = m_NumberKeys[i];
		if (IsKeyDown(Key))
			return i;
	}

	return InvalidPressedNumberKeyIndex();
}

int UControlGroupsManagerComponent::InvalidPressedNumberKeyIndex()
{
	return -1;
}

USelectionManagerComponent* UControlGroupsManagerComponent::GetSelectionManager() const
{
	auto GameMode = UGameplayStatics::GetGameMode(GetWorld());
	if (!IsValid(GameMode))
		return nullptr;

	return Cast<USelectionManagerComponent>(GameMode->GetComponentByClass(USelectionManagerComponent::StaticClass()));
}

bool UControlGroupsManagerComponent::IsKeyDown(const FKey& Key) const
{
	auto PlayerController = GetPlayerController();
	if (!IsValid(PlayerController))
		return false;

	return PlayerController->IsInputKeyDown(Key);
}