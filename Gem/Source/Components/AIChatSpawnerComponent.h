#include <AzCore/Component/Component.h>
#include <HttpRequestor/HttpTypes.h>
#include <Components/Interfaces/AIChatSpawnerBus.h>
#include <AzCore/Math/Uuid.h>
#include <LyShine/Bus/UiSpawnerBus.h>

namespace metapulseWorld {
	class AIChatSpawnerComponent
		: public AZ::Component
		, public AIChatSpawnerBus::Handler
		, public UiSpawnerNotificationBus::Handler {
	public:
		AZ_COMPONENT(metapulseWorld::AIChatSpawnerComponent, "{4DE87D9F-FFF8-4918-896A-A9E7084641C3}", AZ::Component);

		// Component Overrides
		void Init() override;

		void Activate() override;

		void Deactivate() override;

		static void Reflect(AZ::ReflectContext* context);

		// AIchatspawnerbus overrides
		void SpawnMessage(AZStd::pair<size_t, AZStd::string> message) override;

		void ClearMessages() override;

		// uispawner override
		void OnEntitySpawned(const AzFramework::SliceInstantiationTicket& /*ticket*/, const AZ::EntityId& /*spawnedEntity*/) override;
	private:
		AZ::EntityId m_spawnerEntityId;
		AZ::EntityId m_messagesList;

		AZStd::map<AZ::u64, AZStd::pair<size_t, AZStd::string>> m_spawnMap;
		AZStd::map<AZ::EntityId, AZStd::pair<size_t, AZStd::string>> m_itemMap;

		double m_prevTime = 0;
		const double m_cooldown = 5;
	};
}