#include "StartMenuComponent.h"
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <LyShine/Bus/UiButtonBus.h>
#include <AzCore/Console/ILogger.h>
#include <LyShine/Bus/UiCanvasBus.h>
#include <LyShine/Bus/UiCanvasManagerBus.h>

namespace metapulseWorld {

	void metapulseWorld::StartMenuComponent::Init()
	{
	}

	void metapulseWorld::StartMenuComponent::Activate()
	{
		metapulseWorld::StartMenuBus::Handler::BusConnect();

		InitializeButton(loginButtonEntity, OnLoginButtonPressed, m_canvasEntity);
	}

	void metapulseWorld::StartMenuComponent::Deactivate()
	{
		metapulseWorld::StartMenuBus::Handler::BusDisconnect();
	}

	void StartMenuComponent::Reflect(AZ::ReflectContext* context)
	{
		if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
		{
			serializeContext->Class<StartMenuComponent, AZ::Component>()
				->Version(1)
				->Field("Login Button Entity", &StartMenuComponent::loginButtonEntity)
				->Field("SignUpButtonEntity", &StartMenuComponent::signupButtonEntity)
				;

			if (AZ::EditContext* editContext = serializeContext->GetEditContext())
			{
				editContext->Class<StartMenuComponent>("StartMenuComponent", "[manages startmenu logic]")
					->ClassElement(AZ::Edit::ClassElements::EditorData, "")
					->Attribute(AZ::Edit::Attributes::Category, "ComponentCategory")
					->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
					->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("CanvasUI"))
					->DataElement(AZ::Edit::UIHandlers::Default, &StartMenuComponent::loginButtonEntity, "Login Button Entity", "The Login Button's Entity")
					->DataElement(AZ::Edit::UIHandlers::Default, &StartMenuComponent::signupButtonEntity, "SignUp Button Entity", "The SignUp Button's Entity")
					;
			}
		}
	}

	void StartMenuComponent::InitializeButton(AZ::EntityId buttonEntity, AZStd::function<void(AZ::EntityId&)> buttonUpdateFunc, AZ::EntityId& canvasEntity) {
		buttonUpdateFunc(canvasEntity);

		UiButtonBus::Event(buttonEntity, &UiButtonInterface::SetOnClickCallback,
			[&canvasEntity, buttonUpdateFunc]([[maybe_unused]] AZ::EntityId buttonEntityId, [[maybe_unused]] AZ::Vector2 position ) {
				AZLOG_INFO("callback called");
				buttonUpdateFunc(canvasEntity);
			});
	}

	void StartMenuComponent::OnLoginButtonPressed(AZ::EntityId& canvasEntity)
	{

		AZStd::string pathname = "assets/ui/start_menu.uicanvas";
		UiCanvasManagerBus::BroadcastResult(canvasEntity, &UiCanvasManagerBus::Events::FindLoadedCanvasByPathName, pathname, false);

		if (!canvasEntity.IsValid()) {
			AZLOG_INFO("invalid canvas entityid");
		}


		AZLOG_INFO("unloading canvas...");
		UiCanvasManagerBus::Broadcast(&UiCanvasManagerBus::Events::UnloadCanvas, canvasEntity);
	}

	void StartMenuComponent::closeStartMenu()
	{
		AZLOG_INFO("Closing Start Menu...");
		this->Deactivate();
	}
}


