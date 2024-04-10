#include <AzCore/Component/Component.h>
#include <LyShine/Bus/UiSpawnerBus.h>
#include <AzCore/Component/TickBus.h>
#include <Components/Interfaces/UserMenuBus.h>

namespace metapulseWorld {
	class UserMenuComponent
		: public AZ::Component
		, public UiSpawnerNotificationBus::Handler
		, public AZ::TickBus::Handler
		, public UserMenuBus::Handler
	{
	public:
		AZ_COMPONENT(metapulseWorld::UserMenuComponent, "{FFBE6B9C-B0F8-4467-A98C-1B4857D37765}", AZ::Component);

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

		// Tick bus overrides

		void OnTick(float deltaTime, AZ::ScriptTimePoint time) override;
		int GetTickOrder() override;

		// User menu bus overrides
		void SetHasBeenClicked() override;

	private:
		AZ::EntityId m_closeButtonEntityId;
		AZ::EntityId m_userListEntityId;
		AZ::EntityId m_spawnerEntityId;

		AZStd::vector<AZStd::string> m_userVector;

		AZStd::queue<AZStd::string> m_userQueue;

		void FetchUsers();


		double m_prevTime = 0;
		const double m_cooldown = 8;

		bool m_hasBeenClicked = false;
	};
}