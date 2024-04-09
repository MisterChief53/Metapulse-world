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
		StartingPointInput::InputEventNotificationBus::MultiHandler::BusConnect(ToggleUserMenu);
		metapulseWorld::UIAdminBus::Handler::BusConnect();
	}

	void UIAdminComponent::Deactivate()
	{
		StartingPointInput::InputEventNotificationBus::MultiHandler::BusDisconnect(ToggleIngameMenu);
		StartingPointInput::InputEventNotificationBus::MultiHandler::BusDisconnect(ToggleUserMenu);
		metapulseWorld::UIAdminBus::Handler::BusDisconnect();
	}

	void UIAdminComponent::Reflect(AZ::ReflectContext* context)
	{
		if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context)) {
			serializeContext->Class<UIAdminComponent, AZ::Component>()
				->Version(1)
				->Field("Ingame Menu Canvas Path", &UIAdminComponent::m_ingameMenuPath)
				->Field("Inventory Menu Canvas Path", &UIAdminComponent::m_inventoryMenuPath)
				->Field("User Menu Canvas Path", &UIAdminComponent::m_userMenuPath)
				;

			if (AZ::EditContext* editContext = serializeContext->GetEditContext()) {
				editContext->Class<UIAdminComponent>("UIAdminComponent", "[loads and unloads ui elements")
					->ClassElement(AZ::Edit::ClassElements::EditorData, "")
					->Attribute(AZ::Edit::Attributes::Category, "UI")
					->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
					->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
					->DataElement(AZ::Edit::UIHandlers::Default, &UIAdminComponent::m_ingameMenuPath,"Ingame Menu Canvas Path", "Path relative to root folder where the ingame manu canvas is located")
					->DataElement(AZ::Edit::UIHandlers::Default, &UIAdminComponent::m_inventoryMenuPath, "Inventory Menu Canvas Path", "Path relative to root folder where the inventory menu canvas is located")
					->DataElement(AZ::Edit::UIHandlers::Default, &UIAdminComponent::m_userMenuPath, "User Menu Canvas Path", "Path relative to root folder where the user menu canvas is located")
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

			// The unloading will be done inside the canvas, since it consumes all the input, it would never get triggered here.
			if (!m_ingameMenuEntityId.IsValid()) {
				UiCanvasManagerBus::Broadcast(&UiCanvasManagerBus::Events::LoadCanvas, m_ingameMenuPath);
			}
		}

		if (*inputId == ToggleUserMenu) {
			UiCanvasManagerBus::BroadcastResult(m_userMenuEntityId, &UiCanvasManagerBus::Events::FindLoadedCanvasByPathName, m_userMenuPath, false);
			AZLOG_INFO("Loading user selection menu");
			// The unloading will be done inside the canvas, since it consumes all the input, it would never get triggered here.
			if (!m_userMenuEntityId.IsValid()) {
				UiCanvasManagerBus::Broadcast(&UiCanvasManagerBus::Events::LoadCanvas, m_userMenuPath);
			}
		}
	}
	void UIAdminComponent::OnReleased([[maybe_unused]] float value)
	{

	}
	void UIAdminComponent::OnHeld([[maybe_unused]] float value)
	{
	}

	void UIAdminComponent::UnloadStartMenu()
	{
		UiCanvasManagerBus::BroadcastResult(m_ingameMenuEntityId, &UiCanvasManagerBus::Events::FindLoadedCanvasByPathName, m_ingameMenuPath, false);

		// if the canvas is in a valid entity, unload it.
		if (m_ingameMenuEntityId.IsValid()) {
			UiCanvasManagerBus::Broadcast(&UiCanvasManagerBus::Events::UnloadCanvas, m_ingameMenuEntityId);
		}
	}
	
	void UIAdminComponent::LoadInventoryMenu()
	{
		AZLOG_INFO("Unloading Ingame Menu");
		// it is expected that if we want to load the inventory menu, we are unloading the start menu
		UnloadStartMenu();

		UiCanvasManagerBus::BroadcastResult(m_inventoryMenuEntityId, &UiCanvasManagerBus::Events::FindLoadedCanvasByPathName, m_inventoryMenuPath, false);

		// if the canvas is not a valid entity, meaning it is not loaded, load it.
		if (!m_inventoryMenuEntityId.IsValid()) {
			UiCanvasManagerBus::Broadcast(&UiCanvasManagerBus::Events::LoadCanvas, m_inventoryMenuPath);
		}
	}
}