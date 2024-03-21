#include "StartMenuComponent.h"
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <LyShine/Bus/UiButtonBus.h>
#include <AzCore/Console/ILogger.h>
#include <LyShine/Bus/UiCanvasBus.h>
#include <LyShine/Bus/UiCanvasManagerBus.h>
#include <Components/Interfaces/APIRequestsBus.h>
#include <LyShine/Bus/UiTextBus.h>

namespace metapulseWorld {

	void metapulseWorld::StartMenuComponent::Init()
	{
	}

	void metapulseWorld::StartMenuComponent::Activate()
	{
		metapulseWorld::StartMenuBus::Handler::BusConnect();

		InitializeButton(m_loginButtonEntity, OnLoginButtonPressed, m_canvasEntity, m_usernameInputTextEntityId);
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
				->Field("Login Button Entity", &StartMenuComponent::m_loginButtonEntity)
				->Field("SignUpButtonEntity", &StartMenuComponent::m_signupButtonEntity)
				->Field("Status Text Entity", &StartMenuComponent::m_statusTextEntityId)
				->Field("Username Text Entity", &StartMenuComponent::m_usernameInputTextEntityId)
				->Field("Password Text Entity", &StartMenuComponent::m_passwordInputTextEntityId)
				;

			if (AZ::EditContext* editContext = serializeContext->GetEditContext())
			{
				editContext->Class<StartMenuComponent>("StartMenuComponent", "[manages startmenu logic]")
					->ClassElement(AZ::Edit::ClassElements::EditorData, "")
					->Attribute(AZ::Edit::Attributes::Category, "ComponentCategory")
					->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
					->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("CanvasUI"))
					->DataElement(AZ::Edit::UIHandlers::Default, &StartMenuComponent::m_loginButtonEntity, "Login Button Entity", "The Login Button's Entity")
					->DataElement(AZ::Edit::UIHandlers::Default, &StartMenuComponent::m_signupButtonEntity, "SignUp Button Entity", "The SignUp Button's Entity")
					->DataElement(AZ::Edit::UIHandlers::Default, &StartMenuComponent::m_statusTextEntityId, "Status Text Entity", "For displaying results of calling API")
					->DataElement(AZ::Edit::UIHandlers::Default, &StartMenuComponent::m_usernameInputTextEntityId, "Username Text", "Username text to send to API")
					->DataElement(AZ::Edit::UIHandlers::Default, &StartMenuComponent::m_passwordInputTextEntityId, "Password Text", "Password text to sned to API")
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

	void StartMenuComponent::OnLoginButtonPressed(AZ::EntityId& canvasEntity, AZ::EntityId& usernameInputTextEntityId)
	{

		AZStd::string username, password, response;
		bool succeed;
		UiTextBus::BroadcastResult(username, &UiTextBus::Events::GetText, usernameInputTextEntityId);
		

		// Perform login request
		APIRequestsBus::Broadcast(&APIRequestsBus::Events::login, response, succeed, username, password);

		if (succeed) {



			AZStd::string pathname = "assets/ui/start_menu.uicanvas";
			UiCanvasManagerBus::BroadcastResult(canvasEntity, &UiCanvasManagerBus::Events::FindLoadedCanvasByPathName, pathname, false);

			if (!canvasEntity.IsValid()) {
				AZLOG_INFO("invalid canvas entityid");
			}


			AZLOG_INFO("unloading canvas...");
			UiCanvasManagerBus::Broadcast(&UiCanvasManagerBus::Events::UnloadCanvas, canvasEntity);
		}

		
	}

	void StartMenuComponent::closeStartMenu()
	{
		AZLOG_INFO("Closing Start Menu...");
		this->Deactivate();
	}
}


