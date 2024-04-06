#include <AzCore/Component/Component.h>
#include <LyShine/Bus/UiDropTargetBus.h>
#include <HttpRequestor/HttpTypes.h>
#include <LyShine/Bus/UiSpawnerBus.h>
#include <AzCore/Math/Uuid.h>

namespace metapulseWorld {
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
		void OnDrop(AZ::EntityId draggable) override;

		// UI spawner notification bus overrides
		void OnSpawnBegin(const AzFramework::SliceInstantiationTicket& /*ticket*/) override;
		void OnEntitySpawned(const AzFramework::SliceInstantiationTicket& /*ticket*/, const AZ::EntityId& /*spawnedEntity*/) override;
		void OnEntitiesSpawned(const AzFramework::SliceInstantiationTicket& /*ticket*/, const AZStd::vector<AZ::EntityId>& /*spawnedEntities*/) override;
		void OnTopLevelEntitiesSpawned(const AzFramework::SliceInstantiationTicket& /*ticket*/, const AZStd::vector<AZ::EntityId>& /*spawnedEntities*/) override;
		void OnSpawnEnd(const AzFramework::SliceInstantiationTicket& /*ticket*/) override;
		void OnSpawnFailed(const AzFramework::SliceInstantiationTicket& /*ticket*/) override;

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
		// AZStd::queue<AZStd::pair<size_t, AZStd::string>> m_spawnQueue;
		AZStd::map<AZ::u64, AZStd::pair<size_t, AZStd::string>> m_spawnMap;
		AZStd::map<AZ::EntityId, AZStd::pair<size_t, AZStd::string>> m_itemMap;

		void DisplayMoney();
	};
}