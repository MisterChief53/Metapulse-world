#include <AzCore/Component/Component.h>
#include <LyShine/Bus/UiDropTargetBus.h>
#include <LyShine/Bus/UiSpawnerBus.h>
#include <AzCore/Math/Uuid.h>
#include <HttpRequestor/HttpTypes.h>
#include <AzCore/std/containers/set.h>

namespace metapulseWorld {
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
	
	private:
		void FetchInventory();
		void FetchTradeData();

		void RegisterAcceptButton();

		AZ::EntityId m_closeButtonEntityId;

		AZ::EntityId m_unofferedItemsListEntityId;
		AZ::EntityId m_unofferedDropTargetEntityId;

		AZ::EntityId m_offeredItemsListEntityId;
		AZ::EntityId m_offeredDropTargetEntityId;

		AZ::EntityId m_inventorySpawnerEntityId;

		AZ::EntityId m_acceptTradeButtonEntityId;
		AZ::EntityId m_rejectTradeButtonEntityId;

		// pairs contain itemId, itemName
		AZStd::map<AZ::u64, AZStd::pair<int, AZStd::string>> m_spawnMap;
		AZStd::map<AZ::EntityId, AZStd::pair<int, AZStd::string>> m_itemMap;

		AZStd::map<AZ::u64, AZStd::pair<int, AZStd::string>> m_otherSpawnMap;
		AZStd::map<AZ::EntityId, AZStd::pair<int, AZStd::string>> m_otherItemMap;

		AZStd::set<AZ::EntityId> m_offeredItemsSet;
	};
}