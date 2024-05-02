#include "TradeMenuComponent.h"
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <Components/Interfaces/APIRequestsBus.h>
#include <HttpRequestor/HttpRequestorBus.h>
#include <AzCore/Console/ILogger.h>
#include <LyShine/Bus/UiButtonBus.h>
#include <LyShine/Bus/UiCanvasManagerBus.h>
#include <LyShine/Bus/UiElementBus.h>
#include <LyShine/Bus/UiTextBus.h>

void metapulseWorld::TradeMenuComponent::Init()
{
}

void metapulseWorld::TradeMenuComponent::Activate()
{
	UiButtonBus::Event(m_closeButtonEntityId, &UiButtonInterface::SetOnClickCallback,
		[]([[maybe_unused]] AZ::EntityId buttonEntityId, [[maybe_unused]] AZ::Vector2 position) {
			AZLOG_INFO("Close ui callback triggered");
			AZ::EntityId canvasEntityId;
			// It sounds like an awful idea to directly pass the canvasPath as a hardcoded string, however,
			// currently trying to pass an outer string variable to this lambda that contains the string that the member
			// of this class has crashes evertyhing. 
			AZStd::string canvasPath = "assets/ui/trade_menu.uicanvas";
			UiCanvasManagerBus::BroadcastResult(canvasEntityId, &UiCanvasManagerBus::Events::FindLoadedCanvasByPathName, canvasPath, false);
			AZLOG_INFO("unloading canvas");
			UiCanvasManagerBus::Broadcast(&UiCanvasManagerBus::Events::UnloadCanvas, canvasEntityId);
		});

	UiDropTargetNotificationBus::MultiHandler::BusConnect(m_offeredDropTargetEntityId);
	UiDropTargetNotificationBus::MultiHandler::BusConnect(m_unofferedDropTargetEntityId);

	UiSpawnerNotificationBus::Handler::BusConnect(m_inventorySpawnerEntityId);

	FetchInventory();
	RegisterAcceptButton();
	RegisterRejectButton();
	RegisterTradeMoneyButton();
}

void metapulseWorld::TradeMenuComponent::Deactivate()
{
	UiDropTargetNotificationBus::MultiHandler::BusDisconnect(m_offeredDropTargetEntityId);
	UiDropTargetNotificationBus::MultiHandler::BusDisconnect(m_unofferedDropTargetEntityId);

	UiSpawnerNotificationBus::Handler::BusConnect(m_inventorySpawnerEntityId);

	// unset all the offered items on the database
	for (auto item : m_offeredItemsSet) {
		UiElementBus::Event(item, &UiElementBus::Events::ReparentByEntityId, m_unofferedItemsListEntityId, AZ::EntityId());
		UiElementBus::Event(item, &UiElementBus::Events::DestroyElement);

		AZStd::string accountsServerUrl, token;
		APIRequestsBus::BroadcastResult(accountsServerUrl, &APIRequestsBus::Events::getUrl);
		APIRequestsBus::BroadcastResult(token, &APIRequestsBus::Events::getToken);

		AZLOG_INFO("Unsetting tradeable on item id: %d", m_itemMap[item].first);
		AZLOG_INFO("Tostring: %s", AZStd::to_string(m_itemMap[item].first).c_str());

		if (!token.empty() && !accountsServerUrl.empty()) {
			HttpRequestor::HttpRequestorRequestBus::Broadcast(&HttpRequestor::HttpRequestorRequests::AddTextRequestWithHeaders,
				accountsServerUrl + "/trade/tradeItem?itemId=" + AZStd::to_string(m_itemMap[item].first),
				Aws::Http::HttpMethod::HTTP_POST,
				AZStd::map<AZStd::string, AZStd::string>({
					{"Authorization", token},
					{"Content-Type", "application/x-www-form-urlencoded"},
					}),
					[]([[maybe_unused]] const AZStd::string& text, Aws::Http::HttpResponseCode responseCode) {
					AZLOG_INFO("Executing set treadable callback...");
					if (responseCode == Aws::Http::HttpResponseCode::OK) {
						AZLOG_INFO("Correctly unset item as tradeable");
					}
					else {
						AZLOG_ERROR("Failed unsetting item as tradeable");
					}
				}
			);
		}
		else {
			AZLOG_ERROR("Could not get username or server url from APIRequests Bus");
		}
	}
}

void metapulseWorld::TradeMenuComponent::Reflect(AZ::ReflectContext* context)
{
	if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
	{
		serializeContext->Class<TradeMenuComponent, AZ::Component>()
			->Version(1)
			->Field("Close Button Entity", &TradeMenuComponent::m_closeButtonEntityId)
			->Field("Unoffered List Entity", &TradeMenuComponent::m_unofferedItemsListEntityId)
			->Field("Unoffered Drop Target", &TradeMenuComponent::m_unofferedDropTargetEntityId)
			->Field("Offered List Entity", &TradeMenuComponent::m_offeredItemsListEntityId)
			->Field("Offered Drop Target", &TradeMenuComponent::m_offeredDropTargetEntityId)
			->Field("Spawner Entity", &TradeMenuComponent::m_inventorySpawnerEntityId)
			->Field("Accept Trade Button", &TradeMenuComponent::m_acceptTradeButtonEntityId)
			->Field("Reject Trade Button", &TradeMenuComponent::m_rejectTradeButtonEntityId)
			->Field("Trade Money Button", &TradeMenuComponent::m_tradeMoneyButtonEntityId)
			->Field("Trade Money Text", &TradeMenuComponent::m_tradeMoneyTextEntityId)
			->Field("Status Text Entity", &TradeMenuComponent::m_statusTextEntityId)
			;

		if (AZ::EditContext* editContext = serializeContext->GetEditContext())
		{
			editContext->Class<TradeMenuComponent>("Trade Menu Component", "[Implements logic for the trade menu]")
				->ClassElement(AZ::Edit::ClassElements::EditorData, "")
				->Attribute(AZ::Edit::Attributes::Category, "ComponentCategory")
				->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
				->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("CanvasUI"))
				->DataElement(AZ::Edit::UIHandlers::Default, &TradeMenuComponent::m_closeButtonEntityId, "Close Button Entity Id", "The id of the button used to close this UI")
				->DataElement(AZ::Edit::UIHandlers::Default, &TradeMenuComponent::m_unofferedItemsListEntityId, "Unoffered List Entity", "The id of the list that contains unoffered items")
				->DataElement(AZ::Edit::UIHandlers::Default, &TradeMenuComponent::m_unofferedDropTargetEntityId, "Unoffered Drop Target Entity", "The id of the drop target that will store unoffered items")
				->DataElement(AZ::Edit::UIHandlers::Default, &TradeMenuComponent::m_offeredItemsListEntityId, "Offered List Entity", "The id of the list that contains offered items")
				->DataElement(AZ::Edit::UIHandlers::Default, &TradeMenuComponent::m_offeredDropTargetEntityId, "Offered Drop Target Entity", "The id of the drop target that will contain offered items")
				->DataElement(AZ::Edit::UIHandlers::Default, &TradeMenuComponent::m_inventorySpawnerEntityId, "Inventory Spawner Entity", "The id of the spawner that spawns inventory items")
				->DataElement(AZ::Edit::UIHandlers::Default, &TradeMenuComponent::m_acceptTradeButtonEntityId, "Accept Trade Button Id", "The id of the button used to accept the trade")
				->DataElement(AZ::Edit::UIHandlers::Default, &TradeMenuComponent::m_rejectTradeButtonEntityId, "Reject Trade Button Id", "The id of the button used to reject the trade")
				->DataElement(AZ::Edit::UIHandlers::Default, &TradeMenuComponent::m_tradeMoneyButtonEntityId, "Trade Money Button Id", "The id of the button used to add money to the trade")
				->DataElement(AZ::Edit::UIHandlers::Default, &TradeMenuComponent::m_tradeMoneyTextEntityId, "Trade Money Text Id", "The id of the text used to add money to the trade")
				->DataElement(AZ::Edit::UIHandlers::Default, &TradeMenuComponent::m_statusTextEntityId, "Status Text Entity", "For displaying results of calling API")
				;
		}
	}
}

void metapulseWorld::TradeMenuComponent::OnDropHoverStart([[maybe_unused]] AZ::EntityId draggable)
{
}

void metapulseWorld::TradeMenuComponent::OnDropHoverEnd([[maybe_unused]] AZ::EntityId draggable)
{
}

void metapulseWorld::TradeMenuComponent::OnDrop(AZ::EntityId draggable)
{
	AZ::EntityId draggableParent;
	UiElementBus::EventResult(draggableParent, draggable, &UiElementBus::Events::GetParentEntityId);

	if (draggableParent.IsValid() && draggableParent == m_unofferedItemsListEntityId) {
		// 0 to assign an invalid entityid
		UiElementBus::Event(draggable, &UiElementBus::Events::ReparentByEntityId, m_offeredItemsListEntityId, AZ::EntityId());

		AZStd::string accountsServerUrl, token;
		APIRequestsBus::BroadcastResult(accountsServerUrl, &APIRequestsBus::Events::getUrl);
		APIRequestsBus::BroadcastResult(token, &APIRequestsBus::Events::getToken);

		AZLOG_INFO("Setting tradeable on item id: %d", m_itemMap[draggable].first);
		AZLOG_INFO("Tostring: %s", AZStd::to_string(m_itemMap[draggable].first).c_str());

		if (!token.empty() && !accountsServerUrl.empty()) {
			m_offeredItemsSet.insert(draggable);
			HttpRequestor::HttpRequestorRequestBus::Broadcast(&HttpRequestor::HttpRequestorRequests::AddTextRequestWithHeaders,
				accountsServerUrl + "/trade/tradeItem?itemId=" + AZStd::to_string(m_itemMap[draggable].first),
				Aws::Http::HttpMethod::HTTP_POST,
				AZStd::map<AZStd::string, AZStd::string>({ 
					{"Authorization", token},
					{"Content-Type", "application/x-www-form-urlencoded"},
					}),
				[]([[maybe_unused]] const AZStd::string& text, Aws::Http::HttpResponseCode responseCode) {
					AZLOG_INFO("Executing set treadable callback...");
					if (responseCode == Aws::Http::HttpResponseCode::OK) {
						AZLOG_INFO("Correctly set item as tradeable");
					}
					else {
						AZLOG_ERROR("Failed setting item as tradeable");
					}
				}
			);
		}
		else {
			AZLOG_ERROR("Could not get username or server url from APIRequests Bus");
		}
	}
	else if (draggableParent.IsValid() && draggableParent == m_offeredItemsListEntityId) {
		UiElementBus::Event(draggable, &UiElementBus::Events::ReparentByEntityId, m_unofferedItemsListEntityId, AZ::EntityId());

		AZStd::string accountsServerUrl, token;
		APIRequestsBus::BroadcastResult(accountsServerUrl, &APIRequestsBus::Events::getUrl);
		APIRequestsBus::BroadcastResult(token, &APIRequestsBus::Events::getToken);

		AZLOG_INFO("Unsetting tradeable on item id: %d", m_itemMap[draggable].first);
		AZLOG_INFO("Tostring: %s", AZStd::to_string(m_itemMap[draggable].first).c_str());

		if (!token.empty() && !accountsServerUrl.empty()) {
			m_offeredItemsSet.erase(draggable);
			HttpRequestor::HttpRequestorRequestBus::Broadcast(&HttpRequestor::HttpRequestorRequests::AddTextRequestWithHeaders,
				accountsServerUrl + "/trade/tradeItem?itemId=" + AZStd::to_string(m_itemMap[draggable].first),
				Aws::Http::HttpMethod::HTTP_POST,
				AZStd::map<AZStd::string, AZStd::string>({
					{"Authorization", token},
					{"Content-Type", "application/x-www-form-urlencoded"},
					}),
					[]([[maybe_unused]] const AZStd::string& text, Aws::Http::HttpResponseCode responseCode) {
					AZLOG_INFO("Executing set treadable callback...");
					if (responseCode == Aws::Http::HttpResponseCode::OK) {
						AZLOG_INFO("Correctly unset item as tradeable");
					}
					else {
						AZLOG_ERROR("Failed unsetting item as tradeable");
					}
				}
			);
		}
		else {
			AZLOG_ERROR("Could not get username or server url from APIRequests Bus");
		}
	}
}

void metapulseWorld::TradeMenuComponent::OnSpawnBegin([[maybe_unused]] const AzFramework::SliceInstantiationTicket&)
{
}

void metapulseWorld::TradeMenuComponent::OnEntitySpawned([[maybe_unused]] const AzFramework::SliceInstantiationTicket& ticket, [[maybe_unused]] const AZ::EntityId& spawnedEntity)
{
	m_itemMap[spawnedEntity] = AZStd::make_pair(m_spawnMap[ticket.GetRequestId()].first, m_spawnMap[ticket.GetRequestId()].second);
	m_spawnMap.erase(ticket.GetRequestId());

	UiTextBus::Event(spawnedEntity, &UiTextBus::Events::SetText, m_itemMap[spawnedEntity].second);

	UiElementBus::Event(spawnedEntity, &UiElementBus::Events::ReparentByEntityId, m_unofferedItemsListEntityId, AZ::EntityId());
}

void metapulseWorld::TradeMenuComponent::OnEntitiesSpawned([[maybe_unused]] const AzFramework::SliceInstantiationTicket&, [[maybe_unused]] const AZStd::vector<AZ::EntityId>&)
{
}

void metapulseWorld::TradeMenuComponent::OnTopLevelEntitiesSpawned([[maybe_unused]] const AzFramework::SliceInstantiationTicket&, [[maybe_unused]] const AZStd::vector<AZ::EntityId>&)
{
}

void metapulseWorld::TradeMenuComponent::OnSpawnEnd([[maybe_unused]] const AzFramework::SliceInstantiationTicket&)
{
}

void metapulseWorld::TradeMenuComponent::OnSpawnFailed([[maybe_unused]] const AzFramework::SliceInstantiationTicket&)
{
}

void metapulseWorld::TradeMenuComponent::FetchInventory()
{
	AZStd::string accountsServerUrl, username;
	APIRequestsBus::BroadcastResult(accountsServerUrl, &APIRequestsBus::Events::getUrl);
	APIRequestsBus::BroadcastResult(username, &APIRequestsBus::Events::getUsername);

	if (!username.empty() && !accountsServerUrl.empty()) {
		HttpRequestor::HttpRequestorRequestBus::Broadcast(&HttpRequestor::HttpRequestorRequests::AddRequestWithHeaders,
			accountsServerUrl + "/items/getItemsUser?name=" + username,
			Aws::Http::HttpMethod::HTTP_POST,
			AZStd::map<AZStd::string, AZStd::string>({ {"Content-Type", "application/x-www-form-urlencoded"} }),
			[this](const Aws::Utils::Json::JsonView& json, Aws::Http::HttpResponseCode responseCode) {
				AZLOG_INFO("Executing fetch items callback...");
				if (responseCode == Aws::Http::HttpResponseCode::OK) {
					AzFramework::SliceInstantiationTicket itemInstantiationTicket;
					AZ::EntityId itemEntityId;

					Aws::Utils::Array<Aws::Utils::Json::JsonView> items = json.AsArray();

					for (size_t i = 0; i < items.GetLength(); i++) {
						Aws::Utils::Json::JsonView item = items.GetItem(i);

						UiSpawnerBus::EventResult(itemInstantiationTicket, m_inventorySpawnerEntityId, &UiSpawnerBus::Events::Spawn);

						//m_spawnQueue.push(AZStd::make_pair( (size_t) item.GetInteger("id"), AZStd::string(item.GetString("name").c_str()) ));
						m_spawnMap[itemInstantiationTicket.GetRequestId()] = AZStd::make_pair(item.GetInteger("id"), AZStd::string(item.GetString("name").c_str()));
					}
				}
				else {
					AZLOG_ERROR("Failed fetching items");
				}
			}
		);
	}
	else {
		AZLOG_ERROR("Could not get username or server url from APIRequests Bus");
	}
}

void metapulseWorld::TradeMenuComponent::RegisterAcceptButton() {
	UiButtonBus::Event(m_acceptTradeButtonEntityId, &UiButtonInterface::SetOnClickCallback,
		[]([[maybe_unused]] AZ::EntityId m_acceptTradeButtonEntityId, [[maybe_unused]] AZ::Vector2 position) {
			AZStd::string accountsServerUrl, token;
			APIRequestsBus::BroadcastResult(accountsServerUrl, &APIRequestsBus::Events::getUrl);
			APIRequestsBus::BroadcastResult(token, &APIRequestsBus::Events::getToken);

			HttpRequestor::HttpRequestorRequestBus::Broadcast(&HttpRequestor::HttpRequestorRequests::AddTextRequestWithHeaders,
				accountsServerUrl + "/trade/acceptTrade",
				Aws::Http::HttpMethod::HTTP_PUT,
				AZStd::map<AZStd::string, AZStd::string>({
					{"Authorization", token},
					{"Content-Type", "application/x-www-form-urlencoded"}
					}),
				[]([[maybe_unused]] const AZStd::string& text, Aws::Http::HttpResponseCode responseCode) {
					AZLOG_INFO("Accepting trade...");
					if (responseCode == Aws::Http::HttpResponseCode::OK) {
						AZLOG_INFO("Trade accepted correctly!");
					}
					else {
						AZLOG_ERROR("Failed accepting trade");
					}
				}
			);
		});
}

void metapulseWorld::TradeMenuComponent::RegisterRejectButton() {
	UiButtonBus::Event(m_rejectTradeButtonEntityId, &UiButtonInterface::SetOnClickCallback,
		[]([[maybe_unused]] AZ::EntityId m_rejectTradeButtonEntityId, [[maybe_unused]] AZ::Vector2 position) {
			AZStd::string accountsServerUrl, token;
			APIRequestsBus::BroadcastResult(accountsServerUrl, &APIRequestsBus::Events::getUrl);
			APIRequestsBus::BroadcastResult(token, &APIRequestsBus::Events::getToken);

			HttpRequestor::HttpRequestorRequestBus::Broadcast(&HttpRequestor::HttpRequestorRequests::AddTextRequestWithHeaders,
				accountsServerUrl + "/trade/rejectTrade",
				Aws::Http::HttpMethod::HTTP_PUT,
				AZStd::map<AZStd::string, AZStd::string>({
					{"Authorization", token},
					{"Content-Type", "application/x-www-form-urlencoded"}
					}),
				[]([[maybe_unused]] const AZStd::string& text, Aws::Http::HttpResponseCode responseCode) {
					AZLOG_INFO("Rejecting trade...");
					if (responseCode == Aws::Http::HttpResponseCode::OK) {
						AZLOG_INFO("Trade rejected correctly!");
					}
					else {
						AZLOG_ERROR("Failed rejecting trade");
					}
				}
			);
		});
}


void metapulseWorld::TradeMenuComponent::RegisterTradeMoneyButton()
{
	UiButtonBus::Event(m_tradeMoneyButtonEntityId, &UiButtonInterface::SetOnClickCallback,
		[this]([[maybe_unused]] AZ::EntityId m_tradeMoneyButtonEntityId, [[maybe_unused]] AZ::Vector2 position) {
			AZStd::string accountsServerURL, token, money; 
			APIRequestsBus::BroadcastResult(accountsServerURL, &APIRequestsBus::Events::getUrl);
			APIRequestsBus::BroadcastResult(token, &APIRequestsBus::Events::getToken);
			UiTextBus::EventResult(money, m_tradeMoneyTextEntityId, &UiTextBus::Events::GetText);

			HttpRequestor::HttpRequestorRequestBus::Broadcast(&HttpRequestor::HttpRequestorRequests::AddTextRequestWithHeaders,
				accountsServerURL + "/trade/tradeMoney?money=" + money,  
				Aws::Http::HttpMethod::HTTP_PUT,
				AZStd::map<AZStd::string, AZStd::string>({
					{"Authorization", token},
					{"Content-Type", "application/x-www-form-urlencoded"}
					}),
				[]([[maybe_unused]] const AZStd::string& text, Aws::Http::HttpResponseCode responseCode) {
					AZLOG_INFO("Adding money to the trade...");
					if (responseCode == Aws::Http::HttpResponseCode::OK) {
						AZLOG_INFO("Money added correctly!");
					}
					else if (responseCode == Aws::Http::HttpResponseCode::CONFLICT) {
						AZLOG_INFO("Not a double");
					}
					else {
						AZLOG_ERROR("Failed adding money");
					}
				}
			);
		});
}
