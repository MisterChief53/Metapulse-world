#include "InventoryMenuComponent.h"
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <LyShine/Bus/UiButtonBus.h>
#include <AzCore/Console/ILogger.h>
#include <LyShine/Bus/UiCanvasManagerBus.h>
#include <LyShine/Bus/UiDraggableBus.h>
#include <LyShine/Bus/UiCanvasBus.h>
#include <LyShine/Bus/UiElementBus.h>

namespace metapulseWorld {
	void InventoryMenuComponent::Init()
	{
	}
	void InventoryMenuComponent::Activate()
	{
		UiButtonBus::Event(m_closeButtonEntityId, &UiButtonInterface::SetOnClickCallback,
			[]([[maybe_unused]] AZ::EntityId buttonEntityId, [[maybe_unused]] AZ::Vector2 position) {
				AZLOG_INFO("Close ui callback triggered");
				AZ::EntityId canvasEntityId;
				// It sounds like an awful idea to directly pass the canvasPath as a hardcoded string, however,
				// currently trying to pass an outer string variable to this lambda that contains the string that the member
				// of this class has crashes evertyhing. 
				AZStd::string canvasPath = "assets/ui/inventory_menu.uicanvas";
				UiCanvasManagerBus::BroadcastResult(canvasEntityId, &UiCanvasManagerBus::Events::FindLoadedCanvasByPathName, canvasPath, false);
				AZLOG_INFO("unloading canvas");
				UiCanvasManagerBus::Broadcast(&UiCanvasManagerBus::Events::UnloadCanvas, canvasEntityId);
			});

		UiDropTargetNotificationBus::Handler::BusConnect(m_equippedDropTargetEntityId);
	}
	void InventoryMenuComponent::Deactivate()
	{
	}
	void InventoryMenuComponent::Reflect(AZ::ReflectContext* context)
	{
		if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context)) {
			serializeContext->Class<InventoryMenuComponent, AZ::Component>()
				->Version(1)
				->Field("Close Button Entity", &InventoryMenuComponent::m_closeButtonEntityId)
				->Field("Unequipped List Entity", &InventoryMenuComponent::m_unequippedItemsListEntityId)
				->Field("Equipped List Entity", &InventoryMenuComponent::m_equippedItemsListEntityId)
				->Field("Equipped Drop Target", &InventoryMenuComponent::m_equippedDropTargetEntityId)
				;

			if (AZ::EditContext* editContext = serializeContext->GetEditContext()) {
				editContext->Class<InventoryMenuComponent>("InventoryMenuComponent", "Manages behavior of the inventory menu")
					->ClassElement(AZ::Edit::ClassElements::EditorData, "")
					->Attribute(AZ::Edit::Attributes::Category, "UI")
					->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
					->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("CanvasUI"))
					->DataElement(AZ::Edit::UIHandlers::Default, &InventoryMenuComponent::m_closeButtonEntityId, "Close Button Entity Id", "The id of the button used to close this UI")
					->DataElement(AZ::Edit::UIHandlers::Default, &InventoryMenuComponent::m_unequippedItemsListEntityId, "Unequipped List Entity", "The id of the list that contains unequipped items")
					->DataElement(AZ::Edit::UIHandlers::Default, &InventoryMenuComponent::m_equippedItemsListEntityId, "Equipped List Entity", "The id of the list that contains equipped items")
					->DataElement(AZ::Edit::UIHandlers::Default, &InventoryMenuComponent::m_equippedDropTargetEntityId, "Equipped Drop Target", "The id of the drop target for equipped items")
					;
			}
		}
	}
	void InventoryMenuComponent::OnDropHoverStart([[maybe_unused]] AZ::EntityId draggable)
	{
	}
	void InventoryMenuComponent::OnDropHoverEnd([[maybe_unused]] AZ::EntityId draggable)
	{
	}
	void InventoryMenuComponent::OnDrop(AZ::EntityId draggable)
	{
		AZ::EntityId draggableParent;
		UiElementBus::EventResult(draggableParent, draggable, &UiElementBus::Events::GetParentEntityId);

		if (draggableParent.IsValid() && draggableParent == m_unequippedItemsListEntityId) {
			// 0 to assign an invalid entityid
			UiElementBus::Event(draggable, &UiElementBus::Events::ReparentByEntityId, m_equippedItemsListEntityId, AZ::EntityId());
		}
		else if (draggableParent.IsValid() && draggableParent == m_equippedItemsListEntityId) {
			UiElementBus::Event(draggable, &UiElementBus::Events::ReparentByEntityId, m_unequippedItemsListEntityId, AZ::EntityId());
		}
	}
}
