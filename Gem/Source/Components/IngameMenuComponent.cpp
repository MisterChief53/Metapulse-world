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
					;
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
}