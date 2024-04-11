#include "ChatBoxComponent.h"
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <LyShine/Bus/UiButtonBus.h>
#include <AzCore/Console/ILogger.h>
#include <LyShine/Bus/UiCanvasManagerBus.h>
#include <LyShine/Bus/UiDraggableBus.h>
#include <LyShine/Bus/UiElementBus.h>
#include <aws/core/http/HttpResponse.h>
#include <HttpRequestor/HttpRequestorBus.h>
#include <HttpRequestor/HttpTypes.h>
#include <Components/Interfaces/APIRequestsBus.h>
#include <LyShine/Bus/UiTextBus.h>

namespace metapulseWorld {
	void ChatBoxComponent::Init()
	{
	}
	void ChatBoxComponent::Activate()
	{
		UiButtonBus::Event(m_closeButtonEntityId, &UiButtonInterface::SetOnClickCallback,
			[]([[maybe_unused]] AZ::EntityId buttonEntityId, [[maybe_unused]] AZ::Vector2 position) {
				AZLOG_INFO("Close ui callback triggered");
				AZ::EntityId canvasEntityId;
				// It sounds like an awful idea to directly pass the canvasPath as a hardcoded string, however,
				// currently trying to pass an outer string variable to this lambda that contains the string that the member
				// of this class has crashes evertyhing. 
				AZStd::string canvasPath = "assets/ui/chat_box.uicanvas";
				UiCanvasManagerBus::BroadcastResult(canvasEntityId, &UiCanvasManagerBus::Events::FindLoadedCanvasByPathName, canvasPath, false);
				AZLOG_INFO("unloading canvas");
				UiCanvasManagerBus::Broadcast(&UiCanvasManagerBus::Events::UnloadCanvas, canvasEntityId);
			});

		FetchMessages();
	}
	void ChatBoxComponent::Deactivate()
	{
		
	}
	void ChatBoxComponent::Reflect(AZ::ReflectContext* context)
	{
		if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context)) {

			serializeContext->Class<ChatBoxComponent, AZ::Component>()
				->Version(1)
				->Field("Close Button Entity", &ChatBoxComponent::m_closeButtonEntityId)
				->Field("Send Message Button Entity", &ChatBoxComponent::m_sendButtonEntityId)
				->Field("Message Text Input Entity", &ChatBoxComponent::m_messageInputTextEntityId)
				;

			if (AZ::EditContext* editContext = serializeContext->GetEditContext()) {
				editContext->Class<ChatBoxComponent>("ChatBoxComponent", "Manages behavior of the chat box")
					->ClassElement(AZ::Edit::ClassElements::EditorData, "")
					->Attribute(AZ::Edit::Attributes::Category, "UI")
					->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
					->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("CanvasUI"))
					->DataElement(AZ::Edit::UIHandlers::Default, &ChatBoxComponent::m_closeButtonEntityId, "Close Button Entity Id", "The id of the button used to close this UI")
					->DataElement(AZ::Edit::UIHandlers::Default, &ChatBoxComponent::m_sendButtonEntityId, "Send Message Button Entity", "The id of the button used to send a message")
					->DataElement(AZ::Edit::UIHandlers::Default, &ChatBoxComponent::m_messageInputTextEntityId, "Message Text Input Entity", "The id of the text input that contains the message")
					;
			}
		}
	}
	void ChatBoxComponent::FetchMessages()
	{
		AZStd::string accountsServerUrl, username;
		APIRequestsBus::BroadcastResult(accountsServerUrl, &APIRequestsBus::Events::getUrl);
		APIRequestsBus::BroadcastResult(username, &APIRequestsBus::Events::getUsername);

		//AZ::EntityId spawnerEntity = m_spawnerEntityId;

		if (!username.empty() && !accountsServerUrl.empty()) {
			HttpRequestor::HttpRequestorRequestBus::Broadcast(&HttpRequestor::HttpRequestorRequests::AddRequestWithHeaders,
				accountsServerUrl + "/chat/getMessages",
				Aws::Http::HttpMethod::HTTP_GET,
				AZStd::map<AZStd::string, AZStd::string>({ {"Content-Type", "application/x-www-form-urlencoded"} }),
				[this](const Aws::Utils::Json::JsonView& json, Aws::Http::HttpResponseCode responseCode) {
					AZLOG_INFO("Executing fetch messages callback...");
					if (responseCode == Aws::Http::HttpResponseCode::OK) {
						AzFramework::SliceInstantiationTicket messageInstantiationTicket;
						AZ::EntityId messageEntityId;

						Aws::Utils::Array<Aws::Utils::Json::JsonView> messages = json.AsArray();

						for (size_t i = 0; i < messages.GetLength(); i++) {
							Aws::Utils::Json::JsonView message = messages.GetItem(i);

							UiSpawnerBus::EventResult(messageInstantiationTicket, m_spawnerEntityId, &UiSpawnerBus::Events::Spawn);

							m_spawnMap[messageInstantiationTicket.GetRequestId()] = AZStd::make_pair((size_t)message.GetInteger("id"), AZStd::string(message.GetString("content").c_str()));
						}
					}
					else {
						AZLOG_ERROR("Failed fetching messages");
					}
				}
			);
		}
		else {
			AZLOG_ERROR("Could not get username or server url from APIRequests Bus");
		}
	}

	void ChatBoxComponent::OnEntitySpawned([[maybe_unused]] const AzFramework::SliceInstantiationTicket& ticket, const AZ::EntityId& spawnedEntity) 
	{
		m_itemMap[spawnedEntity] = AZStd::make_pair(m_spawnMap[ticket.GetRequestId()].first, m_spawnMap[ticket.GetRequestId()].second);
		m_spawnMap.erase(ticket.GetRequestId());

		UiTextBus::Event(spawnedEntity, &UiTextBus::Events::SetText, m_itemMap[spawnedEntity].second);

		UiElementBus::Event(spawnedEntity, &UiElementBus::Events::ReparentByEntityId, m_unequippedItemsListEntityId, AZ::EntityId());
	}
}