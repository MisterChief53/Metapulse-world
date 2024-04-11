#include <AzCore/Component/Component.h>
#include <HttpRequestor/HttpTypes.h>
#include <LyShine/Bus/UiSpawnerBus.h>
#include <AzCore/Math/Uuid.h>
#include <AzCore/Component/TickBus.h>

namespace metapulseWorld {
	class OtherUserTradeSpawner
		: public AZ::Component
		, public UiSpawnerNotificationBus::Handler
		, public AZ::TickBus::Handler {
	public:
		AZ_COMPONENT(metapulseWorld::OtherUserTradeSpawner, "{3DA5B064-01AB-46D6-A8A5-031D919524DF}", AZ::Component);

		// Component Overrides
		void Init() override;
		void Activate() override;
		void Deactivate() override;
		static void Reflect(AZ::ReflectContext* context);

		// UI spawner notification bus overrides
		void OnSpawnBegin(const AzFramework::SliceInstantiationTicket& /*ticket*/) override;
		void OnEntitySpawned(const AzFramework::SliceInstantiationTicket& /*ticket*/, const AZ::EntityId& /*spawnedEntity*/) override;
		void OnEntitiesSpawned(const AzFramework::SliceInstantiationTicket& /*ticket*/, const AZStd::vector<AZ::EntityId>& /*spawnedEntities*/) override;
		void OnTopLevelEntitiesSpawned(const AzFramework::SliceInstantiationTicket& /*ticket*/, const AZStd::vector<AZ::EntityId>& /*spawnedEntities*/) override;
		void OnSpawnEnd(const AzFramework::SliceInstantiationTicket& /*ticket*/) override;
		void OnSpawnFailed(const AzFramework::SliceInstantiationTicket& /*ticket*/) override;

		void FetchItems();

		// Tick bus overrides

		void OnTick(float deltaTime, AZ::ScriptTimePoint time) override;

		int GetTickOrder() override;

	private:
		AZ::EntityId m_spawnerEntityId;

		// pairs contain itemId, itemName
		AZStd::map<AZ::u64, AZStd::pair<size_t, AZStd::string>> m_spawnMap;
		AZStd::map<AZ::EntityId, AZStd::pair<size_t, AZStd::string>> m_itemMap;

		double m_prevTime = 0;
		const double m_cooldown = 5;
	};
}