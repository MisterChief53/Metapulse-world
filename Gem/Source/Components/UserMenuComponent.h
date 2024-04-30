#include <AzCore/Component/Component.h>
#include <LyShine/Bus/UiSpawnerBus.h>
#include <AzCore/Component/TickBus.h>
#include <Components/Interfaces/UserMenuBus.h>

namespace metapulseWorld {

	/*
	* Component that controls the behavior of the user menu, which is the menu
	* that provides a listing of the connected users' EntityIds and the
	* hability to perform requests on them.
	*/
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

		/*
		* We perform an initial fetch of the usres and register the close menu callback
		*/
		void Activate() override;

		void Deactivate() override;

		static void Reflect(AZ::ReflectContext* context);

		// UI spawner notification bus overrides
		void OnSpawnBegin(const AzFramework::SliceInstantiationTicket& /*ticket*/) override;
		void OnEntitySpawned(const AzFramework::SliceInstantiationTicket& /*ticket*/, const AZ::EntityId& /*spawnedEntity*/) override;
		void OnEntitiesSpawned(const AzFramework::SliceInstantiationTicket& /*ticket*/, const AZStd::vector<AZ::EntityId>& /*spawnedEntities*/) override;
		
		/*
		* We use this function from the notification bus since the list of users will contain
		* buttons, which themselves contain child elements, so we want to set button 
		* callbacks and text only whenever a top level entity is spawned.
		* This makes it so that we can start trading whenever a user is clicked.
		*/
		void OnTopLevelEntitiesSpawned(const AzFramework::SliceInstantiationTicket& /*ticket*/, const AZStd::vector<AZ::EntityId>& /*spawnedEntities*/) override;
		void OnSpawnEnd(const AzFramework::SliceInstantiationTicket& /*ticket*/) override;
		void OnSpawnFailed(const AzFramework::SliceInstantiationTicket& /*ticket*/) override;

		// Tick bus overrides

		/*
		* This makes it so that for a certain ammount of time after a user has been click,
		* we send a request to trade with them, which will eventually expire.
		*/
		void OnTick(float deltaTime, AZ::ScriptTimePoint time) override;

		/*
		* We use TICK_UI since we are using the tickbus when processing UI elements.
		*/
		int GetTickOrder() override;

		// User menu bus overrides
		void SetHasBeenClicked() override;

	private:
		AZ::EntityId m_closeButtonEntityId;
		AZ::EntityId m_userListEntityId;
		AZ::EntityId m_spawnerEntityId;

		/*
		* This is where we store the EntityIds of the users on component activation.
		*/
		AZStd::vector<AZStd::string> m_userVector;

		/*
		* We use this to keep track of the users that we are to spawn whenever we
		* set their callbacks and text.
		*/
		AZStd::queue<AZStd::string> m_userQueue;

		/*
		* Extracts the users from the userVector and spawns them.
		*/
		void FetchUsers();

		/*
		* Keeps track of the previous time that was recorded.
		*/
		double m_prevTime = 0;
		/*
		* Sets how long the requests for notification should last.
		*/
		const double m_cooldown = 8;

		/*
		* Tells us if a user button has been clicked, so that we can
		* know if we should be spawning notifications.
		*/
		bool m_hasBeenClicked = false;
	};
}