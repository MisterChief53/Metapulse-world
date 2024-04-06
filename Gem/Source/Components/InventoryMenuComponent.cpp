#include "InventoryMenuComponent.h"
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
	void InventoryMenuComponent::Init()
	{
	}
	void InventoryMenuComponent::Activate()
	{
		UiButtonBus::Event(m_closeButtonEntityId, &UiButtonInterface::SetOnClickCallback,
			[]([[maybe_unused]] AZ::EntityId buttonEntityId, [[maybe_unused]] AZ::Vector2 position) {
				AZLOG_INFO("Close ui callback triggered");
				AZ::EntityId canvasEntityId;
				// It sounds like an awful idea to directly pass the canvasPath as a hardcoded string, however,
				// currently trying to pass an outer string variable to this lambda that contains the string that the member
				// of this class has crashes evertyhing. 
				AZStd::string canvasPath = "assets/ui/inventory_menu.uicanvas";
				UiCanvasManagerBus::BroadcastResult(canvasEntityId, &UiCanvasManagerBus::Events::FindLoadedCanvasByPathName, canvasPath, false);
				AZLOG_INFO("unloading canvas");
				UiCanvasManagerBus::Broadcast(&UiCanvasManagerBus::Events::UnloadCanvas, canvasEntityId);
			});

		UiDropTargetNotificationBus::MultiHandler::BusConnect(m_equippedDropTargetEntityId);
		UiDropTargetNotificationBus::MultiHandler::BusConnect(m_unequippedDropTargetEntityId);

		UiSpawnerNotificationBus::Handler::BusConnect(m_spawnerEntityId);

		FetchItems();

		DisplayMoney();
	}
	void InventoryMenuComponent::Deactivate()
	{
		UiDropTargetNotificationBus::MultiHandler::BusDisconnect(m_equippedDropTargetEntityId);
		UiDropTargetNotificationBus::MultiHandler::BusDisconnect(m_unequippedDropTargetEntityId);

		UiSpawnerNotificationBus::Handler::BusDisconnect(m_spawnerEntityId);
	}
	void InventoryMenuComponent::Reflect(AZ::ReflectContext* context)
	{
		if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context)) {
			serializeContext->Class<InventoryMenuComponent, AZ::Component>()
				->Version(1)
				->Field("Close Button Entity", &InventoryMenuComponent::m_closeButtonEntityId)
				->Field("Unequipped List Entity", &InventoryMenuComponent::m_unequippedItemsListEntityId)
				->Field("Unequipped Drop Target", &InventoryMenuComponent::m_unequippedDropTargetEntityId)
				->Field("Equipped List Entity", &InventoryMenuComponent::m_equippedItemsListEntityId)
				->Field("Equipped Drop Target", &InventoryMenuComponent::m_equippedDropTargetEntityId)
				->Field("Spawner Entity", &InventoryMenuComponent::m_spawnerEntityId)
				->Field("Money Quantity", &InventoryMenuComponent::m_moneyDisplayEntityId)
				;

			if (AZ::EditContext* editContext = serializeContext->GetEditContext()) {
				editContext->Class<InventoryMenuComponent>("InventoryMenuComponent", "Manages behavior of the inventory menu")
					->ClassElement(AZ::Edit::ClassElements::EditorData, "")
					->Attribute(AZ::Edit::Attributes::Category, "UI")
					->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
					->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("CanvasUI"))
					->DataElement(AZ::Edit::UIHandlers::Default, &InventoryMenuComponent::m_closeButtonEntityId, "Close Button Entity Id", "The id of the button used to close this UI")
					->DataElement(AZ::Edit::UIHandlers::Default, &InventoryMenuComponent::m_unequippedItemsListEntityId, "Unequipped List Entity", "The id of the list that contains unequipped items")
					->DataElement(AZ::Edit::UIHandlers::Default, &InventoryMenuComponent::m_unequippedDropTargetEntityId, "Unequipped Drop Target", "The id of the drop target for unequipped items")
					->DataElement(AZ::Edit::UIHandlers::Default, &InventoryMenuComponent::m_equippedItemsListEntityId, "Equipped List Entity", "The id of the list that contains equipped items")
					->DataElement(AZ::Edit::UIHandlers::Default, &InventoryMenuComponent::m_equippedDropTargetEntityId, "Equipped Drop Target", "The id of the drop target for equipped items")
					->DataElement(AZ::Edit::UIHandlers::Default, &InventoryMenuComponent::m_spawnerEntityId, "Spawner Entity", "The id of the ui spawner entity that will get the fetched items")
					->DataElement(AZ::Edit::UIHandlers::Default, &InventoryMenuComponent::m_moneyDisplayEntityId, "Money Quantity", "The id of the text entity that will display the money quantity")
					;
			}
		}
	}
	void InventoryMenuComponent::OnDropHoverStart([[maybe_unused]] AZ::EntityId draggable)
	{
	}
	void InventoryMenuComponent::OnDropHoverEnd([[maybe_unused]] AZ::EntityId draggable)
	{
	}
	void InventoryMenuComponent::OnDrop(AZ::EntityId draggable)
	{
		AZ::EntityId draggableParent;
		UiElementBus::EventResult(draggableParent, draggable, &UiElementBus::Events::GetParentEntityId);

		if (draggableParent.IsValid() && draggableParent == m_unequippedItemsListEntityId) {
			// 0 to assign an invalid entityid
			UiElementBus::Event(draggable, &UiElementBus::Events::ReparentByEntityId, m_equippedItemsListEntityId, AZ::EntityId());
		}
		else if (draggableParent.IsValid() && draggableParent == m_equippedItemsListEntityId) {
			UiElementBus::Event(draggable, &UiElementBus::Events::ReparentByEntityId, m_unequippedItemsListEntityId, AZ::EntityId());
		}
	}
	void InventoryMenuComponent::OnSpawnBegin([[maybe_unused]] const AzFramework::SliceInstantiationTicket&)
	{
	}
	void InventoryMenuComponent::OnEntitySpawned([[maybe_unused]] const AzFramework::SliceInstantiationTicket& ticket, const AZ::EntityId& spawnedEntity)
	{
		m_itemMap[spawnedEntity] = AZStd::make_pair(m_spawnMap[ticket.GetRequestId()].first, m_spawnMap[ticket.GetRequestId()].second);
		m_spawnMap.erase(ticket.GetRequestId());

		UiTextBus::Event(spawnedEntity, &UiTextBus::Events::SetText, m_itemMap[spawnedEntity].second);

		UiElementBus::Event(spawnedEntity, &UiElementBus::Events::ReparentByEntityId, m_unequippedItemsListEntityId, AZ::EntityId());
	}
	void InventoryMenuComponent::OnEntitiesSpawned([[maybe_unused]] const AzFramework::SliceInstantiationTicket&, [[maybe_unused]] const AZStd::vector<AZ::EntityId>&)
	{
	}
	void InventoryMenuComponent::OnTopLevelEntitiesSpawned([[maybe_unused]] const AzFramework::SliceInstantiationTicket&, [[maybe_unused]] const AZStd::vector<AZ::EntityId>&)
	{
	}
	void InventoryMenuComponent::OnSpawnEnd([[maybe_unused]] const AzFramework::SliceInstantiationTicket&)
	{
	}
	void InventoryMenuComponent::OnSpawnFailed([[maybe_unused]] const AzFramework::SliceInstantiationTicket&)
	{
	}
	void InventoryMenuComponent::FetchItems()
	{
		AZStd::string accountsServerUrl, username;
		APIRequestsBus::BroadcastResult(accountsServerUrl, &APIRequestsBus::Events::getUrl);
		APIRequestsBus::BroadcastResult(username, &APIRequestsBus::Events::getUsername);

		//AZ::EntityId spawnerEntity = m_spawnerEntityId;

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

							UiSpawnerBus::EventResult(itemInstantiationTicket, m_spawnerEntityId, &UiSpawnerBus::Events::Spawn);

							//m_spawnQueue.push(AZStd::make_pair( (size_t) item.GetInteger("id"), AZStd::string(item.GetString("name").c_str()) ));
							m_spawnMap[itemInstantiationTicket.GetRequestId()] = AZStd::make_pair((size_t)item.GetInteger("id"), AZStd::string(item.GetString("name").c_str()));
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
	void InventoryMenuComponent::DisplayMoney()
	{
		AZStd::string accountsServerUrl, token;
		APIRequestsBus::BroadcastResult(accountsServerUrl, &APIRequestsBus::Events::getUrl);
		APIRequestsBus::BroadcastResult(token, &APIRequestsBus::Events::getToken);

		HttpRequestor::HttpRequestorRequestBus::Broadcast(&HttpRequestor::HttpRequestorRequests::AddRequestWithHeaders,
			accountsServerUrl + "/auth/userInfo",
			Aws::Http::HttpMethod::HTTP_GET,
			AZStd::map<AZStd::string, AZStd::string>({
				{"Authorization", token}
				}),
			[this](const Aws::Utils::Json::JsonView& json, Aws::Http::HttpResponseCode responseCode) {
				AZLOG_INFO("Executing display money callback on invontory menu...");
				if (responseCode == Aws::Http::HttpResponseCode::OK) {
					AZStd::string moneyString = AZStd::to_string((float)json.GetDouble("money"));

					// Ugly hack to get rid of extra zeroes
					moneyString.pop_back();
					moneyString.pop_back();
					moneyString.pop_back();
					moneyString.pop_back();

					UiTextBus::Event(m_moneyDisplayEntityId, &UiTextBus::Events::SetText, moneyString);
				}
				else {
					AZLOG_ERROR("Failed fetching user's money");
				}
			}
		);
	}
}
