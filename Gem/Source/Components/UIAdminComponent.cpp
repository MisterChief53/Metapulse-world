#include "UIAdminComponent.h"
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <LyShine/Bus/UiCanvasManagerBus.h>
#include <LyShine/Bus/UiCanvasBus.h>
#include <AzCore/Console/ILogger.h>

namespace metapulseWorld {
	void UIAdminComponent::Init()
	{
	}

	void UIAdminComponent::Activate()
	{
		StartingPointInput::InputEventNotificationBus::MultiHandler::BusConnect(ToggleIngameMenu);
	}

	void UIAdminComponent::Deactivate()
	{
	}

	void UIAdminComponent::Reflect(AZ::ReflectContext* context)
	{
		if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context)) {
			serializeContext->Class<UIAdminComponent, AZ::Component>()
				->Version(1)
				->Field("Ingame Menu Canvas Path", &UIAdminComponent::m_ingameMenuPath)
				;

			if (AZ::EditContext* editContext = serializeContext->GetEditContext()) {
				editContext->Class<UIAdminComponent>("UIAdminComponent", "[loads and unloads ui elements")
					->ClassElement(AZ::Edit::ClassElements::EditorData, "")
					->Attribute(AZ::Edit::Attributes::Category, "UI")
					->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
					->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
					->DataElement(AZ::Edit::UIHandlers::Default, &UIAdminComponent::m_ingameMenuPath,"Ingame Menu Canvas Path", "Path relative to root folder where the ingame manu canvas is located")
					;
			}
		}
	}
	void UIAdminComponent::OnPressed([[maybe_unused]] float value)
	{
		AZLOG_INFO("Escape Key was pressed");
		const StartingPointInput::InputEventNotificationId* inputId = StartingPointInput::InputEventNotificationBus::GetCurrentBusId();

		if (inputId == nullptr) {
			return;
		}

		if (*inputId == ToggleIngameMenu) {

			UiCanvasManagerBus::BroadcastResult(m_ingameMenuEntityId, &UiCanvasManagerBus::Events::FindLoadedCanvasByPathName, m_ingameMenuPath, false);

			// The unloading will be done inside the canvas, since it consumes all the input.
			if (!m_ingameMenuEntityId.IsValid()) {
				UiCanvasManagerBus::Broadcast(&UiCanvasManagerBus::Events::LoadCanvas, m_ingameMenuPath);
			}
		}
	}
	void UIAdminComponent::OnReleased([[maybe_unused]] float value)
	{

	}
	void UIAdminComponent::OnHeld([[maybe_unused]] float value)
	{
	}
}