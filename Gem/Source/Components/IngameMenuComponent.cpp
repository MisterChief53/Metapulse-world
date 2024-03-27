#include "IngameMenuComponent.h"
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <LyShine/Bus/UiCanvasManagerBus.h>
#include <LyShine/LyShineBus.h>
#include <AzCore/Console/ILogger.h>
#include <LyShine/Bus/UiButtonBus.h>
#include <AzCore/Console/IConsole.h>
#include <LyShine/Bus/UiSliderBus.h>
#include <AzFramework/Components/CameraBus.h>
#include <AzCore/Component/ComponentApplicationBus.h>
#include <LyShine/Bus/UiCanvasBus.h>
#include <AzFramework/Windowing/WindowBus.h>
#include <Components/Interfaces/UIAdminBus.h>

namespace metapulseWorld {
	void IngameMenuComponent::Init()
	{
	}

	void IngameMenuComponent::Activate()
	{
		UiButtonBus::Event(m_closeButtonEntityId, &UiButtonInterface::SetOnClickCallback, 
			[]([[maybe_unused]] AZ::EntityId buttonEntityId, [[maybe_unused]] AZ::Vector2 position) {
				AZLOG_INFO("Close ui callback triggered");
				AZ::EntityId canvasEntityId;
				// It sounds like an awful idea to directly pass the canvasPath as a hardcoded string, however,
				// currently trying to pass an outer string variable to this lambda that contains the string that the member
				// of this class has crashes evertyhing. 
				AZStd::string canvasPath = "assets/ui/ingame_menu.uicanvas";
				UiCanvasManagerBus::BroadcastResult(canvasEntityId, &UiCanvasManagerBus::Events::FindLoadedCanvasByPathName, canvasPath, false);
				AZLOG_INFO("unloading canvas");
				UiCanvasManagerBus::Broadcast(&UiCanvasManagerBus::Events::UnloadCanvas, canvasEntityId);
			});

		UiButtonBus::Event(m_logoutButtonEntityId, &UiButtonInterface::SetOnClickCallback,
			[]([[maybe_unused]] AZ::EntityId buttonEntityId, [[maybe_unused]] AZ::Vector2 position) {
				const auto console = AZ::Interface<AZ::IConsole>::Get();

				if (!console) {
					AZ_Assert(false, "UiStartMenuComponent attempting to use console commands before AZ::Console is available.");
					return;
				}

				console->PerformCommand("quit");
			});

		setupSlider();

		//connect to dropdown bus to handle events
		UiDropdownNotificationBus::Handler::BusConnect(m_resolutionDropdownEntityId);

		setupInventoryButton();
		
	}

	void IngameMenuComponent::Deactivate()
	{
	}

	void IngameMenuComponent::Reflect(AZ::ReflectContext* context)
	{
		if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
		{
			serializeContext->Class<IngameMenuComponent, AZ::Component>()
				->Version(1)
				->Field("Close Button Entity", &IngameMenuComponent::m_closeButtonEntityId)
				->Field("Canvas Path", &IngameMenuComponent::m_canvasPath)
				->Field("Logout Button Entity", &IngameMenuComponent::m_logoutButtonEntityId)
				->Field("Fov Slider Entity", &IngameMenuComponent::m_fovSliderEntityId)
				->Field("Resolution Dropdown Entity", &IngameMenuComponent::m_resolutionDropdownEntityId)
				->Field("Resolution Button Entity 1", &IngameMenuComponent::m_resolutionOption1)
				->Field("Resolution Button Entity 2", &IngameMenuComponent::m_resolutionOption2)
				->Field("Resolution Button Entity 3", &IngameMenuComponent::m_resolutionOption3)
				->Field("Inventory Button Entity", &IngameMenuComponent::m_inventoryButtonEntityId)
				;

			if (AZ::EditContext* editContext = serializeContext->GetEditContext())
			{
				editContext->Class<IngameMenuComponent>("Ingame Menu Component", "[Implements logic for ingame pause menu]")
					->ClassElement(AZ::Edit::ClassElements::EditorData, "")
					->Attribute(AZ::Edit::Attributes::Category, "ComponentCategory")
					->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
					->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("CanvasUI"))
					->DataElement(AZ::Edit::UIHandlers::Default, &IngameMenuComponent::m_closeButtonEntityId, "Close Button Entity Id", "The id of the button used to close this UI")
					->DataElement(AZ::Edit::UIHandlers::Default, &IngameMenuComponent::m_canvasPath, "Canvas Path", "The path to the canvas file relative from the root")
					->DataElement(AZ::Edit::UIHandlers::Default, &IngameMenuComponent::m_logoutButtonEntityId, "Logout Button Entity", "The id of the button used to logout")
					->DataElement(AZ::Edit::UIHandlers::Default, &IngameMenuComponent::m_fovSliderEntityId, "Fov Slider Entity", "The id of the slider used to change fov")
					->DataElement(AZ::Edit::UIHandlers::Default, &IngameMenuComponent::m_resolutionDropdownEntityId, "Resolution Dropdown Entity", "The id of the dropdown that changes res")
					->DataElement(AZ::Edit::UIHandlers::Default, &IngameMenuComponent::m_resolutionOption1, "Resolution Button Entity 1", "The id of button #1 in the dropdown")
					->DataElement(AZ::Edit::UIHandlers::Default, &IngameMenuComponent::m_resolutionOption2, "Resolution Button Entity 2", "The id of button #2 in the dropdown")
					->DataElement(AZ::Edit::UIHandlers::Default, &IngameMenuComponent::m_resolutionOption3, "Resolution Button Entity 3", "The id of button #3 in the dropdown")
					->DataElement(AZ::Edit::UIHandlers::Default, &IngameMenuComponent::m_inventoryButtonEntityId, "Inventory Button Entity", "The id of the button used to open inventory")
					;
			}
		}
	}

	void IngameMenuComponent::OnDropdownExpanded()
	{
	}

	void IngameMenuComponent::OnDropdownCollapsed()
	{
	}

	void IngameMenuComponent::OnDropdownValueChanged([[maybe_unused]] AZ::EntityId option)
	{
		AZLOG_INFO("An option in the dropdown was chosen");

		size_t resolutionIdx = 0;

		bool canChangeResolution = false;

		if (option == m_resolutionOption1) {
			AZLOG_INFO("Resolution 1 chosen");
			resolutionIdx = 0;
			canChangeResolution = true;
		}
		else if (option == m_resolutionOption2) {
			AZLOG_INFO("Resolution 2 chosen");
			resolutionIdx = 1;
			canChangeResolution = true;
		}
		else if (option == m_resolutionOption3) {
			AZLOG_INFO("Resolution 3 chosen");
			resolutionIdx = 2;
			canChangeResolution = true;
		}

		if (AZ::IConsole* console = AZ::Interface<AZ::IConsole>::Get(); console)
		{
			AZLOG_INFO("Getting window handle...");
			AzFramework::NativeWindowHandle windowHandle = nullptr;
			AzFramework::WindowSystemRequestBus::BroadcastResult(
				windowHandle,
				&AzFramework::WindowSystemRequestBus::Events::GetDefaultWindowHandle);

			if (windowHandle)
			{
				AZLOG_INFO("Window handle was valid, now changing resolution...");
				AzFramework::WindowRequestBus::Event(windowHandle,
					&AzFramework::WindowRequestBus::Events::ResizeClientArea,
					AzFramework::WindowSize(m_resolutionsVector[resolutionIdx].first, m_resolutionsVector[resolutionIdx].second),
					AzFramework::WindowPosOptions()
				);
				
				// This comment is left here, since this is how we would do it
				// if we were in fullscreen. For future reference!
				/*AzFramework::WindowRequestBus::Event(
					windowHandle,
					&AzFramework::WindowRequestBus::Events::SetRenderResolution,
					AzFramework::WindowSize(m_resolutionsVector[0].first, m_resolutionsVector[0].second)
				);*/
			}
		}
	}

	AZ::Entity* IngameMenuComponent::GetActiveCamera()
	{
		using namespace AZ;
		using namespace Camera;

		EntityId activeCameraId;
		CameraSystemRequestBus::BroadcastResult(activeCameraId, &CameraSystemRequestBus::Events::GetActiveCamera);

		auto ca = Interface<ComponentApplicationRequests>::Get();
		return ca->FindEntity(activeCameraId);
	}
	
	void IngameMenuComponent::setupSlider() {
		float currentFov;
		AZ::EntityId activeCameraId;

		Camera::CameraSystemRequestBus::BroadcastResult(activeCameraId, &Camera::CameraSystemRequestBus::Events::GetActiveCamera);

		Camera::CameraRequestBus::EventResult(currentFov, activeCameraId, &Camera::CameraRequestBus::Events::GetFovDegrees);

		UiSliderBus::Event(m_fovSliderEntityId, &UiSliderInterface::SetValue, currentFov);

		UiSliderBus::Event(m_fovSliderEntityId, &UiSliderInterface::SetValueChangingCallback,
			[]([[maybe_unused]] AZ::EntityId entityId, [[maybe_unused]] float newValue) {
				AZLOG_INFO("Performing fov change to: %0.2f", newValue);
				AZ::EntityId activeCameraId;
				// It might seem redundant to get the active camera Id twice. However, we need to *request* it again if we want to make
				// the changes to the fov. So, once up there to get the current fov, and once in this callback to change it.
				Camera::CameraSystemRequestBus::BroadcastResult(activeCameraId, &Camera::CameraSystemRequestBus::Events::GetActiveCamera);

				Camera::CameraRequestBus::Event(activeCameraId, &Camera::CameraRequestBus::Events::SetFovDegrees, newValue);
			});
	}

	void IngameMenuComponent::setupInventoryButton() {
		UiButtonBus::Event(m_inventoryButtonEntityId, &UiButtonBus::Events::SetOnClickCallback,
			[]([[maybe_unused]] AZ::EntityId buttonEntityId, [[maybe_unused]] AZ::Vector2 position) {
				AZLOG_INFO("Inventory Menu Callback Triggered");
				UIAdminBus::Broadcast(&UIAdminBus::Events::LoadInventoryMenu);
			});
	}
}