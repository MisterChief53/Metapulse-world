#include <AzCore/Component/Component.h>
#include <Components/Interfaces/HUDBus.h>
#include <HttpRequestor/HttpTypes.h>
#include <LyShine/Bus/UiSpawnerBus.h>

namespace metapulseWorld {
	class HUDComponent
		: public AZ::Component
		, public metapulseWorld::HUDBus::Handler
		, public UiSpawnerNotificationBus::Handler {
	public:
		AZ_COMPONENT(metapulseWorld::HUDComponent, "{5D064869-46DB-4BD8-9C41-019316F580F6}", AZ::Component);

		// Component Overrides
		void Init() override;
		void Activate() override;
		void Deactivate() override;
		static void Reflect(AZ::ReflectContext* context);

		// HUD Bus Overrides
		void DisplayMoney() override;
		void SpawnNotification(const AZStd::string& text) override;

		// UI spawner notification bus overrides
		void OnSpawnBegin(const AzFramework::SliceInstantiationTicket& /*ticket*/) override;
		void OnEntitySpawned(const AzFramework::SliceInstantiationTicket& /*ticket*/, const AZ::EntityId& /*spawnedEntity*/) override;
		void OnEntitiesSpawned(const AzFramework::SliceInstantiationTicket& /*ticket*/, const AZStd::vector<AZ::EntityId>& /*spawnedEntities*/) override;
		void OnTopLevelEntitiesSpawned(const AzFramework::SliceInstantiationTicket& /*ticket*/, const AZStd::vector<AZ::EntityId>& /*spawnedEntities*/) override;
		void OnSpawnEnd(const AzFramework::SliceInstantiationTicket& /*ticket*/) override;
		void OnSpawnFailed(const AzFramework::SliceInstantiationTicket& /*ticket*/) override;

	public:
		AZ::EntityId m_moneyDisplayEntityId;
		AZ::EntityId m_spawnerEntityId;
		AZStd::string m_notificationText;

	};
}