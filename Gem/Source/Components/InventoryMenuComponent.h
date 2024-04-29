#include <AzCore/Component/Component.h>
#include <LyShine/Bus/UiDropTargetBus.h>
#include <HttpRequestor/HttpTypes.h>
#include <LyShine/Bus/UiSpawnerBus.h>
#include <AzCore/Math/Uuid.h>

namespace metapulseWorld {
	/*
	* Controls the behavior of the inventory menu. This menu retrieves a user's items from
	* the accounts server and triggers the execution of said items on a user's request.
	*/
	class InventoryMenuComponent
		: public AZ::Component
		, public UiDropTargetNotificationBus::MultiHandler
		, public UiSpawnerNotificationBus::Handler {
	public:
		AZ_COMPONENT(metapulseWorld::InventoryMenuComponent, "{9EAAE2FF-9265-4BA0-98EE-ADACEAE57911}", AZ::Component);

		// Component Overrides
		void Init() override;
		void Activate() override;
		void Deactivate() override;
		static void Reflect(AZ::ReflectContext* context);

		// Drop target overrides
		void OnDropHoverStart(AZ::EntityId draggable) override;
		void OnDropHoverEnd(AZ::EntityId draggable) override;

		/*
		* Since we are a multihandler to the UiDropTargetNotificatinBus, we have to do 
		* different things depending on where an entity is dropped. In this case, if the
		* item is droppedo n the equipped items list, it is executed and reparented to it.
		* Else, if it is unequipped, currently it is only reparented to the unequipped
		* items list.
		*/
		void OnDrop(AZ::EntityId draggable) override;

		// UI spawner notification bus overrides
		void OnSpawnBegin(const AzFramework::SliceInstantiationTicket& /*ticket*/) override;

		/*
		* Whenever an item is spawned in the UI, it means that we have fetched it from the 
		* accounts server, so, we have to reparent it to the unequipped items list and change
		* its text to be the item's text.
		*/
		void OnEntitySpawned(const AzFramework::SliceInstantiationTicket& /*ticket*/, const AZ::EntityId& /*spawnedEntity*/) override;
		void OnEntitiesSpawned(const AzFramework::SliceInstantiationTicket& /*ticket*/, const AZStd::vector<AZ::EntityId>& /*spawnedEntities*/) override;
		void OnTopLevelEntitiesSpawned(const AzFramework::SliceInstantiationTicket& /*ticket*/, const AZStd::vector<AZ::EntityId>& /*spawnedEntities*/) override;
		void OnSpawnEnd(const AzFramework::SliceInstantiationTicket& /*ticket*/) override;
		void OnSpawnFailed(const AzFramework::SliceInstantiationTicket& /*ticket*/) override;

		/*
		* We retrieve the items list for this player from the accounts server, and we trigger
		* the spawner so that is spawns an entity, which we will modify later using this class'
		* OnEntitySpawned implementation.
		*/
		void FetchItems();

	private:
		AZ::EntityId m_closeButtonEntityId;

		AZ::EntityId m_unequippedItemsListEntityId;
		AZ::EntityId m_unequippedDropTargetEntityId;

		AZ::EntityId m_equippedItemsListEntityId;
		AZ::EntityId m_equippedDropTargetEntityId;

		AZ::EntityId m_spawnerEntityId;

		AZ::EntityId m_moneyDisplayEntityId;
		
		// pairs contain itemId, itemName

		/*
		* This map holds the information for the items that were spawned, so that
		* we can change their information whenever we received the notificaion that 
		* they were spawned. This includes their item
		*/
		AZStd::map<AZ::u64, AZStd::pair<size_t, AZStd::string>> m_spawnMap;

		/*
		* Holds the items that are now spawned and reparented. This is so that
		* we can delete them whenever we have to refresh the items list.
		*/
		AZStd::map<AZ::EntityId, AZStd::pair<size_t, AZStd::string>> m_itemMap;

		/*
		* Fetches the money from the accounts server, formats it, and displays it
		* in the menu
		*/
		void DisplayMoney();
	};
}