#include <AzCore/Component/Component.h>
#include <LyShine/Bus/UiDropTargetBus.h>
#include <LyShine/Bus/UiSpawnerBus.h>
#include <AzCore/Math/Uuid.h>
#include <HttpRequestor/HttpTypes.h>
#include <AzCore/std/containers/set.h>

namespace metapulseWorld {
	/*
	* This component controls the behavior of the trade menu, offerring and unoffering items,
	* fetching the other users's items, accepting and rejecting trades, as well as fetching
	* a user's items.
	*/
	class TradeMenuComponent
		: public AZ::Component
		, public UiDropTargetNotificationBus::MultiHandler
		, public UiSpawnerNotificationBus::Handler
	{
	public:
		AZ_COMPONENT(metapulseWorld::TradeMenuComponent, "{8109FA40-547C-467D-8EDE-89F6431275F8}", AZ::Component);

		// Component Overrides
		void Init() override;
		void Activate() override;
		/*
		* On the trade menu component's deactivation, we cancel the items that were offered
		* from being offered, so, we make a request for each one to the server telling it
		* that we are no longer offering these items.
		*/
		void Deactivate() override;
		static void Reflect(AZ::ReflectContext* context);

		// Drop target overrides
		void OnDropHoverStart(AZ::EntityId draggable) override;
		void OnDropHoverEnd(AZ::EntityId draggable) override;

		/*
		* Since we are a multihandler to onDrop events, we need to know where we are
		* dragging our items to. So, we check if we are dragging our items to the list
		* of items we are offering or the list of items we are not offering, and depending
		* on that, we make different requests to the accounts server to notify the other
		* player in the trade.
		*/
		void OnDrop(AZ::EntityId draggable) override;

		// UI spawner notification bus overrides
		void OnSpawnBegin(const AzFramework::SliceInstantiationTicket& /*ticket*/) override;

		/*
		* The entities we will be spawning are the items that we own, so, we will use the ticket
		* to check which entity is being spawned, use the item name to change the entitie's text,
		* reparent it, and then add it to the item map so that we can have the entityIds for 
		* later use.
		*/
		void OnEntitySpawned(const AzFramework::SliceInstantiationTicket& /*ticket*/, const AZ::EntityId& /*spawnedEntity*/) override;
		void OnEntitiesSpawned(const AzFramework::SliceInstantiationTicket& /*ticket*/, const AZStd::vector<AZ::EntityId>& /*spawnedEntities*/) override;
		void OnTopLevelEntitiesSpawned(const AzFramework::SliceInstantiationTicket& /*ticket*/, const AZStd::vector<AZ::EntityId>& /*spawnedEntities*/) override;
		void OnSpawnEnd(const AzFramework::SliceInstantiationTicket& /*ticket*/) override;
		void OnSpawnFailed(const AzFramework::SliceInstantiationTicket& /*ticket*/) override;
	
	private:
		/*
		* Fetches the whole inventory for this user. This means that we request the accounts server
		* for the list of items, then we use the spawner the spawn those items on the unequipped
		* items list.
		*/
		void FetchInventory();

		/*
		* This function registers the callback for the reject button in the trade. This button
		* makes a request to the accounts server saying that the current active trade has been
		* rejected, and now we need both users to accept again.
		*/
		void RegisterRejectButton();

		/*
		* Registers the callback for the accept button, which sends a request to the accounts
		* server saying that the trade has been accepted.
		*/
		void RegisterAcceptButton();

		void RegisterTradeMoneyButton();

		AZ::EntityId m_closeButtonEntityId;

		AZ::EntityId m_unofferedItemsListEntityId;
		AZ::EntityId m_unofferedDropTargetEntityId;

		AZ::EntityId m_offeredItemsListEntityId;
		AZ::EntityId m_offeredDropTargetEntityId;

		AZ::EntityId m_inventorySpawnerEntityId;

		AZ::EntityId m_acceptTradeButtonEntityId;
		AZ::EntityId m_rejectTradeButtonEntityId;

		AZ::EntityId m_tradeMoneyButtonEntityId;
		AZ::EntityId m_tradeMoneyTextEntityId;

		AZ::EntityId m_statusTextEntityId;


		// pairs contain itemId, itemName
		/*
		* This map contains the items that are to be spawned, and their
		* instantiation ticket. This is so that we later can reparent
		* the corresponding item and change the text on the elements
		*/
		AZStd::map<AZ::u64, AZStd::pair<int, AZStd::string>> m_spawnMap;

		/*
		* This map contains the entityid of the elements mapped to the item.
		* This is so that we can later refresh or manipulate them directly.
		*/
		AZStd::map<AZ::EntityId, AZStd::pair<int, AZStd::string>> m_itemMap;

		/*
		* This map contains the items that are to be spawned, and their
		* instantiation ticket. This is so that we later can reparent
		* the corresponding item and change the text on the elements
		*/
		AZStd::map<AZ::u64, AZStd::pair<int, AZStd::string>> m_otherSpawnMap;

		/*
		* This map contains the entityid of the elements mapped to the item.
		* This is so that we can later refresh or manipulate them directly.
		*/
		AZStd::map<AZ::EntityId, AZStd::pair<int, AZStd::string>> m_otherItemMap;

		/*
		* We also save the offered items in a set, so that we can check if an item
		* is offered quickly.
		*/
		AZStd::set<AZ::EntityId> m_offeredItemsSet;
	};
}