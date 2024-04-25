#include "OtherUserTradeSpawner.h"
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <HttpRequestor/HttpRequestorBus.h>
#include <Components/Interfaces/APIRequestsBus.h>
#include <AzCore/Console/ILogger.h>
#include <LyShine/Bus/UiCanvasManagerBus.h>
#include <LyShine/Bus/UiTextBus.h>
#include <LyShine/Bus/UiElementBus.h>

void metapulseWorld::OtherUserTradeSpawner::Init()
{
}

void metapulseWorld::OtherUserTradeSpawner::Activate()
{
	m_spawnerEntityId = this->GetEntityId();
	UiSpawnerNotificationBus::Handler::BusConnect(m_spawnerEntityId);
	AZ::TickBus::Handler::BusConnect();
	//FetchItems();
}

void metapulseWorld::OtherUserTradeSpawner::Deactivate()
{
	UiSpawnerNotificationBus::Handler::BusDisconnect(m_spawnerEntityId);
	AZ::TickBus::Handler::BusDisconnect();
}

void metapulseWorld::OtherUserTradeSpawner::Reflect(AZ::ReflectContext* context)
{
	if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context)) {
		serializeContext->Class<OtherUserTradeSpawner, AZ::Component>()
			->Version(1)
			;

		if (AZ::EditContext* editContext = serializeContext->GetEditContext()) {
			editContext->Class<OtherUserTradeSpawner>("OtherUserTradeSpawner", "Listens for changes on spawner dealing with other user's items")
				->ClassElement(AZ::Edit::ClassElements::EditorData, "")
				->Attribute(AZ::Edit::Attributes::Category, "UI")
				->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
				->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("UI"))
				;
		}
	}
}

void metapulseWorld::OtherUserTradeSpawner::OnSpawnBegin([[maybe_unused]] const AzFramework::SliceInstantiationTicket&)
{
}

void metapulseWorld::OtherUserTradeSpawner::OnEntitySpawned([[maybe_unused]] const AzFramework::SliceInstantiationTicket& ticket, [[maybe_unused]] const AZ::EntityId& spawnedEntity)
{
	m_itemMap[spawnedEntity] = AZStd::make_pair(m_spawnMap[ticket.GetRequestId()].first, m_spawnMap[ticket.GetRequestId()].second);
	m_spawnMap.erase(ticket.GetRequestId());

	UiTextBus::Event(spawnedEntity, &UiTextBus::Events::SetText, m_itemMap[spawnedEntity].second);

	UiElementBus::Event(spawnedEntity, &UiElementBus::Events::ReparentByEntityId, this->GetEntityId(), AZ::EntityId());
}

void metapulseWorld::OtherUserTradeSpawner::OnEntitiesSpawned([[maybe_unused]] const AzFramework::SliceInstantiationTicket&, [[maybe_unused]] const AZStd::vector<AZ::EntityId>&)
{
}

void metapulseWorld::OtherUserTradeSpawner::OnTopLevelEntitiesSpawned([[maybe_unused]] const AzFramework::SliceInstantiationTicket&, [[maybe_unused]] const AZStd::vector<AZ::EntityId>&)
{
}

void metapulseWorld::OtherUserTradeSpawner::OnSpawnEnd([[maybe_unused]] const AzFramework::SliceInstantiationTicket&)
{
}

void metapulseWorld::OtherUserTradeSpawner::OnSpawnFailed([[maybe_unused]] const AzFramework::SliceInstantiationTicket&)
{
}

void metapulseWorld::OtherUserTradeSpawner::executeTrade()
{
	AZStd::string accountsServerUrl;
	APIRequestsBus::BroadcastResult(accountsServerUrl, &APIRequestsBus::Events::getUrl);

	if (!accountsServerUrl.empty()) {
		HttpRequestor::HttpRequestorRequestBus::Broadcast(&HttpRequestor::HttpRequestorRequests::AddRequestWithHeaders,
			accountsServerUrl + "/trade/execute",
			Aws::Http::HttpMethod::HTTP_POST,
			AZStd::map<AZStd::string, AZStd::string>({
				{"Content-Type", "application/x-www-form-urlencoded"},
				}),
				[](const Aws::Utils::Json::JsonView&, Aws::Http::HttpResponseCode responseCode) {
				AZLOG_INFO("Executing trade from OtherUserTradeSpawner...");
				if (responseCode == Aws::Http::HttpResponseCode::OK) {
					AZLOG_ERROR("Trade executed successfully");
				}
				else if (responseCode == Aws::Http::HttpResponseCode::INTERNAL_SERVER_ERROR) {
					AZLOG_ERROR("Closing Trade interface");

					AZLOG_INFO("Close trade ui callback triggered");
					AZ::EntityId canvasEntityId;
					AZStd::string canvasPath = "assets/ui/trade_menu.uicanvas";
					UiCanvasManagerBus::BroadcastResult(canvasEntityId, &UiCanvasManagerBus::Events::FindLoadedCanvasByPathName, canvasPath, false);
					AZLOG_INFO("unloading canvas");
					UiCanvasManagerBus::Broadcast(&UiCanvasManagerBus::Events::UnloadCanvas, canvasEntityId);
				}
				else {
					AZLOG_ERROR("Failed trying to execute trade");
				}
			}
		);
	}
	else {
		AZLOG_ERROR("Could not get server url from APIRequests Bus");
	}
}



void metapulseWorld::OtherUserTradeSpawner::FetchItems()
{
	AZStd::string accountsServerUrl, token;
	APIRequestsBus::BroadcastResult(accountsServerUrl, &APIRequestsBus::Events::getUrl);
	APIRequestsBus::BroadcastResult(token, &APIRequestsBus::Events::getToken);

	//AZ::EntityId spawnerEntity = m_spawnerEntityId;

	if (!token.empty() && !accountsServerUrl.empty()) {
		HttpRequestor::HttpRequestorRequestBus::Broadcast(&HttpRequestor::HttpRequestorRequests::AddRequestWithHeaders,
			accountsServerUrl + "/trade/tradeInformation",
			Aws::Http::HttpMethod::HTTP_GET,
			AZStd::map<AZStd::string, AZStd::string>({ 
				{"Authorization", token},
				{"Content-Type", "application/x-www-form-urlencoded"},
				}),
			[this](const Aws::Utils::Json::JsonView& json, Aws::Http::HttpResponseCode responseCode) {
				AZLOG_INFO("Executing fetch items of other user callback...");
				if (responseCode == Aws::Http::HttpResponseCode::OK) {
					AzFramework::SliceInstantiationTicket itemInstantiationTicket;
					AZ::EntityId itemEntityId;

					Aws::Utils::Array<Aws::Utils::Json::JsonView> items = json.AsArray();

					for (size_t i = 0; i < items.GetLength(); i++) {
						Aws::Utils::Json::JsonView item = items.GetItem(i);

						UiSpawnerBus::EventResult(itemInstantiationTicket, m_spawnerEntityId, &UiSpawnerBus::Events::Spawn);

						//m_spawnQueue.push(AZStd::make_pair( (size_t) item.GetInteger("id"), AZStd::string(item.GetString("name").c_str()) ));
						m_spawnMap[itemInstantiationTicket.GetRequestId()] = AZStd::make_pair((size_t)item.GetInteger("id"), AZStd::string(item.GetString("name").c_str()));
					}
				}
				else {
					AZLOG_ERROR("Failed fetching items of other user");
				}
			}
		);
	}
	else {
		AZLOG_ERROR("Could not get username or server url from APIRequests Bus");
	}
}

void metapulseWorld::OtherUserTradeSpawner::OnTick([[maybe_unused]] float deltaTime, AZ::ScriptTimePoint time)
{
	if (time.GetSeconds() - m_prevTime >= m_cooldown) {
		for (auto entityPair : m_itemMap) {
			UiElementBus::Event(entityPair.first, &UiElementBus::Events::DestroyElement);
			//m_itemMap.erase(entityPair.first);
		}

		m_itemMap = {};

		FetchItems();
		executeTrade();

		m_prevTime = time.GetSeconds();
	}
}

int metapulseWorld::OtherUserTradeSpawner::GetTickOrder()
{
	return AZ::TICK_UI;
}
