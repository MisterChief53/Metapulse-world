#include <AzCore/Component/Component.h>
#include <Components/Interfaces/HUDBus.h>
#include <HttpRequestor/HttpTypes.h>
#include <LyShine/Bus/UiSpawnerBus.h>
#include <AzCore/Component/TickBus.h>

namespace metapulseWorld {
	/*
	* This component controls the behavior of the HUD component, by making fetches
	* to the money endpoints and handling notification spawns.
	*/
	class HUDComponent
		: public AZ::Component
		, public metapulseWorld::HUDBus::Handler
		, public UiSpawnerNotificationBus::Handler
		, public AZ::TickBus::Handler
	{
	public:
		AZ_COMPONENT(metapulseWorld::HUDComponent, "{5D064869-46DB-4BD8-9C41-019316F580F6}", AZ::Component);

		// Component Overrides
		void Init() override;
		void Activate() override;
		void Deactivate() override;
		static void Reflect(AZ::ReflectContext* context);

		// HUD Bus Overrides
		/*
		* Fetches the money from the enpoint and formats it so that it can be shown
		* in the HUD
		*/
		void DisplayMoney() override;

		/*
		* Spawns the notifications by contacting the spawner bus and giving it
		* certain text.
		*/
		void SpawnNotification(const AZStd::string& text) override;

		// UI spawner notification bus overrides
		void OnSpawnBegin(const AzFramework::SliceInstantiationTicket& /*ticket*/) override;
		void OnEntitySpawned(const AzFramework::SliceInstantiationTicket& /*ticket*/, const AZ::EntityId& /*spawnedEntity*/) override;
		void OnEntitiesSpawned(const AzFramework::SliceInstantiationTicket& /*ticket*/, const AZStd::vector<AZ::EntityId>& /*spawnedEntities*/) override;
		
		/*
		* Of the spawner notification bus, we only use this function override.
		* Since the notifications are entities with many child entities, when we are
		* displaying them, we are only concerned about doing something whenever we
		* are spawning the top-level entity. This implementation sets the text for
		* the notification.
		*/
		void OnTopLevelEntitiesSpawned(const AzFramework::SliceInstantiationTicket& /*ticket*/, const AZStd::vector<AZ::EntityId>& /*spawnedEntities*/) override;
		void OnSpawnEnd(const AzFramework::SliceInstantiationTicket& /*ticket*/) override;
		void OnSpawnFailed(const AzFramework::SliceInstantiationTicket& /*ticket*/) override;

		// Tick bus overrides
		/*
		* We check every 5 seconds whether or not to spawn the notification, using
		* the cooldown helper variables.
		*/
		void OnTick(float deltaTime, AZ::ScriptTimePoint time) override;

		/*
		* We set the tick order to TICK_UI, since we are dealing with UI elements
		*/
		int GetTickOrder() override;

	public:
		AZ::EntityId m_moneyDisplayEntityId;
		AZ::EntityId m_spawnerEntityId;
		AZStd::string m_notificationText;

		/*
		* Keeps track of the absolute time in seconds where each
		* event that depends on the OnTick was triggered, after passing
		* through the cooldown
		*/
		double m_prevTime = 0;

		/*
		* Specifies the cooldown in seconds that tells us how often we have to
		* perform OnTick operations.
		*/
		const double m_cooldown = 5;
	};
}