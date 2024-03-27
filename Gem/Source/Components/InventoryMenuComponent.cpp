#include "InventoryMenuComponent.h"
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <LyShine/Bus/UiButtonBus.h>
#include <AzCore/Console/ILogger.h>
#include <LyShine/Bus/UiCanvasManagerBus.h>

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
				;

			if (AZ::EditContext* editContext = serializeContext->GetEditContext()) {
				editContext->Class<InventoryMenuComponent>("InventoryMenuComponent", "Manages behavior of the inventory menu")
					->ClassElement(AZ::Edit::ClassElements::EditorData, "")
					->Attribute(AZ::Edit::Attributes::Category, "UI")
					->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
					->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("CanvasUI"))
					->DataElement(AZ::Edit::UIHandlers::Default, &InventoryMenuComponent::m_closeButtonEntityId, "Close Button Entity Id", "The id of the button used to close this UI")
					;
			}
		}
	}
}
