#include "StartMenuComponent.h"
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <LyShine/Bus/UiButtonBus.h>
#include <AzCore/Console/ILogger.h>
#include <LyShine/Bus/UiCanvasBus.h>
#include <LyShine/Bus/UiCanvasManagerBus.h>
#include <Components/Interfaces/APIRequestsBus.h>
#include <LyShine/Bus/UiTextBus.h>
#include <aws/core/http/HttpResponse.h>
#include <HttpRequestor/HttpRequestorBus.h>
#include <HttpRequestor/HttpTypes.h>
#include <Components/Interfaces/UserRegistryBus.h>
#include <LyShine/Bus/UiSpawnerBus.h>
#include <Components/Interfaces/HUDBus.h>

namespace metapulseWorld {

	void metapulseWorld::StartMenuComponent::Init()
	{
	}

	void metapulseWorld::StartMenuComponent::Activate()
	{
		metapulseWorld::StartMenuBus::Handler::BusConnect();

		InitializeButton(m_loginButtonEntity, OnLoginButtonPressed, m_usernameInputTextEntityId, m_passwordInputTextEntityId, m_statusTextEntityId, m_canvasEntity);
		InitializeButton(m_signupButtonEntity, OnSignUpButtonPressed, m_usernameInputTextEntityId, m_passwordInputTextEntityId, m_statusTextEntityId, m_canvasEntity);
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

	void StartMenuComponent::InitializeButton(AZ::EntityId buttonEntity, AZStd::function<void(AZ::EntityId&,AZ::EntityId&,AZ::EntityId&,AZ::EntityId&)> buttonUpdateFunc,
		AZ::EntityId& usernameInputTextEntityId,
		AZ::EntityId& passwordInputTextEntityId, 
		AZ::EntityId& statusTextEntityId, 
		AZ::EntityId& canvasEntity) {

		buttonUpdateFunc(usernameInputTextEntityId, passwordInputTextEntityId, statusTextEntityId, canvasEntity);

		UiButtonBus::Event(buttonEntity, &UiButtonInterface::SetOnClickCallback,
			[&usernameInputTextEntityId, &passwordInputTextEntityId, &statusTextEntityId, &canvasEntity, buttonUpdateFunc]([[maybe_unused]] AZ::EntityId buttonEntityId, [[maybe_unused]] AZ::Vector2 position ) {
				AZLOG_INFO("UIButtonInterface Onclick callback called");
				buttonUpdateFunc(usernameInputTextEntityId, passwordInputTextEntityId, statusTextEntityId, canvasEntity);
			});
	}

	void StartMenuComponent::OnLoginButtonPressed(AZ::EntityId& usernameInputTextEntityId,
		AZ::EntityId& passwordInputTextEntityId, AZ::EntityId& statusTextEntityId, AZ::EntityId& canvasEntity)
	{
		AZStd::string username, password, accountsServerUrl;
		APIRequestsBus::BroadcastResult(accountsServerUrl, &APIRequestsBus::Events::getUrl);
		UiTextBus::EventResult(username, usernameInputTextEntityId, &UiTextBus::Events::GetText);
		UiTextBus::EventResult(password, passwordInputTextEntityId, &UiTextBus::Events::GetText);
		APIRequestsBus::Broadcast(&APIRequestsBus::Events::setUsername, username);

		AZLOG_INFO("Performing request...");
		HttpRequestor::HttpRequestorRequestBus::Broadcast(&HttpRequestor::HttpRequestorRequests::AddTextRequestWithHeaders,
			accountsServerUrl + "/auth/login?name=" + username + "&password=" + password,
			Aws::Http::HttpMethod::HTTP_POST,
			AZStd::map<AZStd::string, AZStd::string>({ {"Content-Type", "application/x-www-form-urlencoded"} }),
			[&statusTextEntityId, &canvasEntity](const AZStd::string& response, Aws::Http::HttpResponseCode responseCode) {
				AZLOG_INFO("Executing login callback...");
				if (responseCode == Aws::Http::HttpResponseCode::OK) {
					UiTextBus::Event(statusTextEntityId, &UiTextBus::Events::SetText, "Logged in succesfully!");

					APIRequestsBus::Broadcast(&APIRequestsBus::Events::setToken, response);
					AZLOG_INFO("token: %s", response.c_str());

					// update the money on the HUD
					HUDBus::Broadcast(&HUDBus::Events::DisplayMoney);

					// Now, disable the canvas
					AZStd::string pathname = "assets/ui/start_menu.uicanvas";
					UiCanvasManagerBus::BroadcastResult(canvasEntity, &UiCanvasManagerBus::Events::FindLoadedCanvasByPathName, pathname, false);

					if (!canvasEntity.IsValid()) {
						AZLOG_INFO("invalid canvas entityid");
					}

					AZLOG_INFO("unloading canvas...");
					UiCanvasManagerBus::Broadcast(&UiCanvasManagerBus::Events::UnloadCanvas, canvasEntity);
				}
				else {
					if (!response.empty()) {
						UiTextBus::Event(statusTextEntityId, &UiTextBus::Events::SetText, response);
					}
					else {
						UiTextBus::Event(statusTextEntityId, &UiTextBus::Events::SetText, "Could not initiate request");
					}

				}
			}
		);
	}

	void StartMenuComponent::OnSignUpButtonPressed(AZ::EntityId& usernameInputTextEntityId,
		AZ::EntityId& passwordInputTextEntityId, AZ::EntityId& statusTextEntityId, [[maybe_unused]] AZ::EntityId& canvasEntity)
	{
		AZStd::string username, password, accountsServerUrl;
		APIRequestsBus::BroadcastResult(accountsServerUrl, &APIRequestsBus::Events::getUrl);
		UiTextBus::EventResult(username, usernameInputTextEntityId, &UiTextBus::Events::GetText);
		UiTextBus::EventResult(password, passwordInputTextEntityId, &UiTextBus::Events::GetText);

		if (!username.empty() && !password.empty()) {
			AZLOG_INFO("Performing request...");
			HttpRequestor::HttpRequestorRequestBus::Broadcast(&HttpRequestor::HttpRequestorRequests::AddTextRequestWithHeaders,
				accountsServerUrl + "/auth/register?name=" + username + "&password=" + password,
				Aws::Http::HttpMethod::HTTP_POST,
				AZStd::map<AZStd::string, AZStd::string>({ {"Content-Type", "application/x-www-form-urlencoded"} }),
				[&statusTextEntityId](const AZStd::string& response, Aws::Http::HttpResponseCode responseCode) {
					AZLOG_INFO("Executing signup callback...");
					if (responseCode == Aws::Http::HttpResponseCode::OK) {
						if (!response.empty()) {
							UiTextBus::Event(statusTextEntityId, &UiTextBus::Events::SetText, response);
						}
						else {
							UiTextBus::Event(statusTextEntityId, &UiTextBus::Events::SetText, "User created succesfully");
						}
					}
					else {
						UiTextBus::Event(statusTextEntityId, &UiTextBus::Events::SetText, "Could not create user correctly");
					}
				}
			);
		}
		else {
			UiTextBus::Event(statusTextEntityId, &UiTextBus::Events::SetText, "Both Username and Password must contain characters");
		}
	}

	void StartMenuComponent::closeStartMenu()
	{
		AZLOG_INFO("Closing Start Menu...");
		this->Deactivate();
	}
}


