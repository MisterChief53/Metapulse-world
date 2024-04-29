#include <AzCore/Component/Component.h>
#include <HttpRequestor/HttpTypes.h>
#include <LyShine/Bus/UiSpawnerBus.h>
#include <AzCore/Math/Uuid.h>
#include <AzCore/Component/TickBus.h>

namespace metapulseWorld {

	/*
	* Controls the behavior of the list that displays what the other user is offering
	* in the local trade menu by constanty fetching this items list, like what our 
	* other items lists do
	*/
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
		/*
		* Whenever an entity is spawned, it is reparented to the correspoinding list and is
		* registered in the itemMap.
		*/
		void OnEntitySpawned(const AzFramework::SliceInstantiationTicket& /*ticket*/, const AZ::EntityId& /*spawnedEntity*/) override;
		void OnEntitiesSpawned(const AzFramework::SliceInstantiationTicket& /*ticket*/, const AZStd::vector<AZ::EntityId>& /*spawnedEntities*/) override;
		void OnTopLevelEntitiesSpawned(const AzFramework::SliceInstantiationTicket& /*ticket*/, const AZStd::vector<AZ::EntityId>& /*spawnedEntities*/) override;
		void OnSpawnEnd(const AzFramework::SliceInstantiationTicket& /*ticket*/) override;
		void OnSpawnFailed(const AzFramework::SliceInstantiationTicket& /*ticket*/) override;

		/*
		* When we fetch the items, we make a request to the accounts server, get the other
		* user's items list, and then spawn them and added to the m_spawnMap
		*/
		void FetchItems();

		/*
		* Calls the accounts server to tell it that we should execute the trade, and closes
		* this trade menu
		*/
		void executeTrade();

		// Tick bus overrides
		/*
		* Every cooldown seconds, we destroy all the items listed and remove them from the
		* itemMap, so that we can then fetch new items and not bother about the previous
		* list
		*/
		void OnTick(float deltaTime, AZ::ScriptTimePoint time) override;

		/*
		* Since we are dealing with UI elements, we set the tick order to be TICK_UI
		*/
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