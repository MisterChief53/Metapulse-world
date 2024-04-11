#include "UserMenuComponent.h"
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <LyShine/Bus/UiButtonBus.h>
#include <AzCore/Console/ILogger.h>
#include <LyShine/Bus/UiCanvasManagerBus.h>
#include <Components/Interfaces/UserRegistryBus.h>
#include <LyShine/Bus/UiSpawnerBus.h>
#include <LyShine/Bus/UiElementBus.h>
#include <LyShine/Bus/UiTextBus.h>
#include <Components/Interfaces/TradeNotificationBus.h>
#include <Multiplayer/NetworkEntity/NetworkEntityHandle.h>
#include <Components/TradeNotification.h>
#include <HttpRequestor/HttpRequestorBus.h>
#include <Components/Interfaces/APIRequestsBus.h>

void metapulseWorld::UserMenuComponent::Init()
{
}

void metapulseWorld::UserMenuComponent::Activate()
{
	UiButtonBus::Event(m_closeButtonEntityId, &UiButtonInterface::SetOnClickCallback,
		[]([[maybe_unused]] AZ::EntityId buttonEntityId, [[maybe_unused]] AZ::Vector2 position) {
			AZLOG_INFO("Close ui callback triggered");
			AZ::EntityId canvasEntityId;
			// It sounds like an awful idea to directly pass the canvasPath as a hardcoded string, however,
			// currently trying to pass an outer string variable to this lambda that contains the string that the member
			// of this class has crashes evertyhing. 
			AZStd::string canvasPath = "assets/ui/user_selection.uicanvas";
			UiCanvasManagerBus::BroadcastResult(canvasEntityId, &UiCanvasManagerBus::Events::FindLoadedCanvasByPathName, canvasPath, false);
			AZLOG_INFO("unloading canvas");
			UiCanvasManagerBus::Broadcast(&UiCanvasManagerBus::Events::UnloadCanvas, canvasEntityId);
		});

	UiSpawnerNotificationBus::Handler::BusConnect(m_spawnerEntityId);

	/*UserRegistryBus::BroadcastResult(m_userVector, &UserRegistryBus::Events::GetUserVector);*/
	AZStd::string user;
	UserRegistryGettersBus::BroadcastResult(user, &UserRegistryGettersBus::Events::BusGetPlayer1);
	m_userVector.push_back(user);
	UserRegistryGettersBus::BroadcastResult(user, &UserRegistryGettersBus::Events::BusGetPlayer2);
	m_userVector.push_back(user);

	FetchUsers();

	AZ::TickBus::Handler::BusConnect();
	UserMenuBus::Handler::BusConnect();

}

void metapulseWorld::UserMenuComponent::Deactivate()
{
	UiSpawnerNotificationBus::Handler::BusDisconnect(m_spawnerEntityId);
	AZ::TickBus::Handler::BusDisconnect();
	UserMenuBus::Handler::BusDisconnect();
}

void metapulseWorld::UserMenuComponent::Reflect(AZ::ReflectContext* context)
{
	if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
	{
		serializeContext->Class<UserMenuComponent, AZ::Component>()
			->Version(1)
			->Field("Close Button Entity", &UserMenuComponent::m_closeButtonEntityId)
			->Field("User List Entity", &UserMenuComponent::m_userListEntityId)
			->Field("Spawner Entity", &UserMenuComponent::m_spawnerEntityId)
			;

		if (AZ::EditContext* editContext = serializeContext->GetEditContext())
		{
			editContext->Class<UserMenuComponent>("User Menu Component", "[Implements logic for user menu]")
				->ClassElement(AZ::Edit::ClassElements::EditorData, "")
				->Attribute(AZ::Edit::Attributes::Category, "ComponentCategory")
				->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
				->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("CanvasUI"))
				->DataElement(AZ::Edit::UIHandlers::Default, &UserMenuComponent::m_closeButtonEntityId, "Close Button Entity Id", "The id of the button used to close this UI")
				->DataElement(AZ::Edit::UIHandlers::Default, &UserMenuComponent::m_userListEntityId, "User List Entity Id", "The id of the user list to put users on")
				->DataElement(AZ::Edit::UIHandlers::Default, &UserMenuComponent::m_spawnerEntityId, "Spawner Entity", "The id of the spawner that spawns user buttons")
				;
		}
	}
}

void metapulseWorld::UserMenuComponent::OnSpawnBegin([[maybe_unused]] const AzFramework::SliceInstantiationTicket&)
{
}

void metapulseWorld::UserMenuComponent::OnEntitySpawned([[maybe_unused]] const AzFramework::SliceInstantiationTicket&, [[maybe_unused]] const AZ::EntityId& spawnedEntity)
{

	
}

void metapulseWorld::UserMenuComponent::OnEntitiesSpawned([[maybe_unused]] const AzFramework::SliceInstantiationTicket&, [[maybe_unused]] const AZStd::vector<AZ::EntityId>&)
{
}

void metapulseWorld::UserMenuComponent::OnTopLevelEntitiesSpawned([[maybe_unused]] const AzFramework::SliceInstantiationTicket&, [[maybe_unused]] const AZStd::vector<AZ::EntityId>& spawnedEntities)
{
	AZ::EntityId child;
	UiElementBus::Event(spawnedEntities[0], &UiElementBus::Events::ReparentByEntityId, m_userListEntityId, AZ::EntityId());
	UiElementBus::EventResult(child, spawnedEntities[0], &UiElementBus::Events::GetChildEntityId, 0);
	UiTextBus::Event(child, &UiTextBus::Events::SetText, m_userQueue.front());

	AZStd::string tempId = m_userQueue.front();
	tempId.erase(tempId.begin());
	tempId.pop_back();

	// unsigned long long id;
	AZ::u64 id = AZStd::stoull(tempId);

	AZLOG_INFO("Trimmed id: %llu", id);

	// setup button callback
	// the id is supposed to be straight up copied to the lambda, not just the address
	UiButtonBus::Event(spawnedEntities[0], &UiButtonBus::Events::SetOnClickCallback,
		[id]([[maybe_unused]] AZ::EntityId buttonEntity, [[maybe_unused]] AZ::Vector2) {
			//m_hasBeenClicked = true;
			UserMenuBus::Broadcast(&UserMenuBus::Events::SetHasBeenClicked);
			AZLOG_INFO("Button callback called!");
			AZ::EntityId userEntityId = AZ::EntityId(id);
			if (userEntityId.IsValid()) {
				AZLOG_INFO("User entity id is valid");
			}
			//TradeNotificationBus::Broadcast(&TradeNotificationBus::Events::UpdateHudValue);
			Multiplayer::NetworkEntityHandle tradeNotificationHandle;
			TradeNotificationBus::BroadcastResult(tradeNotificationHandle, &TradeNotificationBus::Events::GetHandleBus);

			if (tradeNotificationHandle.Exists()) {
				AZLOG_INFO("network entity handle is valid!!");
			}

			AZStd::string accountsServerUrl, token;
			APIRequestsBus::BroadcastResult(accountsServerUrl, &APIRequestsBus::Events::getUrl);
			APIRequestsBus::BroadcastResult(token, &APIRequestsBus::Events::getToken);

			HttpRequestor::HttpRequestorRequestBus::Broadcast(&HttpRequestor::HttpRequestorRequests::AddRequestWithHeaders,
				accountsServerUrl + "/trade/setRequest",
				Aws::Http::HttpMethod::HTTP_POST,
				AZStd::map<AZStd::string, AZStd::string>({
					{"Authorization", token}
					}),
				[]([[maybe_unused]] const Aws::Utils::Json::JsonView& json, Aws::Http::HttpResponseCode responseCode) {
					AZLOG_INFO("Seting up a request to trade...");
					if (responseCode == Aws::Http::HttpResponseCode::OK) {
						AZLOG_INFO("Correctly sent the trade request!");
					}
					else {
						AZLOG_ERROR("Failed setting up a request to trade");
					}
				}
			);

			HttpRequestor::HttpRequestorRequestBus::Broadcast(&HttpRequestor::HttpRequestorRequests::AddTextRequest,
				accountsServerUrl + "/trade/create",
				Aws::Http::HttpMethod::HTTP_POST,
				[]([[maybe_unused]] const AZStd::string& text, Aws::Http::HttpResponseCode responseCode) {
					AZLOG_INFO("Creating a new trade...");
					if (responseCode == Aws::Http::HttpResponseCode::OK) {
						AZLOG_INFO("Correctly created a new trade!");
					}
					else {
						AZLOG_ERROR("Failed creating a new trade");
					}
				}
			);

		});

	m_userQueue.pop();
}

void metapulseWorld::UserMenuComponent::OnSpawnEnd(const AzFramework::SliceInstantiationTicket&)
{
}

void metapulseWorld::UserMenuComponent::OnSpawnFailed(const AzFramework::SliceInstantiationTicket&)
{
}

void metapulseWorld::UserMenuComponent::OnTick([[maybe_unused]] float deltaTime, AZ::ScriptTimePoint time)
{
	if (time.GetSeconds() - m_prevTime >= m_cooldown) {
		if (m_hasBeenClicked == true) {
			AZLOG_INFO("Detected that button has been clicked, so now toggling back to false");
			AZStd::string accountsServerUrl, token;
			APIRequestsBus::BroadcastResult(accountsServerUrl, &APIRequestsBus::Events::getUrl);
			APIRequestsBus::BroadcastResult(token, &APIRequestsBus::Events::getToken);

			HttpRequestor::HttpRequestorRequestBus::Broadcast(&HttpRequestor::HttpRequestorRequests::AddRequestWithHeaders,
				accountsServerUrl + "/trade/setRequest",
				Aws::Http::HttpMethod::HTTP_POST,
				AZStd::map<AZStd::string, AZStd::string>({
					{"Authorization", token}
					}),
				[]([[maybe_unused]] const Aws::Utils::Json::JsonView& json, Aws::Http::HttpResponseCode responseCode) {
					AZLOG_INFO("Seting up a request to trade...");
					if (responseCode == Aws::Http::HttpResponseCode::OK) {
						AZLOG_INFO("Correctly sent the trade request!");
					}
					else {
						AZLOG_ERROR("Failed setting up a request to trade");
					}
				}
			);

			m_hasBeenClicked = false;
		}

		m_prevTime = time.GetSeconds();
	}
}

int metapulseWorld::UserMenuComponent::GetTickOrder()
{
	return AZ::TICK_UI;
}

void metapulseWorld::UserMenuComponent::SetHasBeenClicked()
{
	m_hasBeenClicked = true;
}

void metapulseWorld::UserMenuComponent::FetchUsers()
{
	AZLOG_INFO("Fetching the users for user list...");
	AzFramework::SliceInstantiationTicket itemInstantiationTicket;
	if (m_userVector.empty()) {
		AZLOG_INFO("User vector is empty!");
		return;
	}
	for (auto user : m_userVector) {
		if (!user.empty()) {
			AZLOG_INFO("Got user succesfully: %s!, now spawning...", user.c_str());
			m_userQueue.push(user);
			UiSpawnerBus::EventResult(itemInstantiationTicket, m_spawnerEntityId, &UiSpawnerBus::Events::Spawn);
		}
		else {
			AZLOG_INFO("User is empty!");
		}
	}
}
