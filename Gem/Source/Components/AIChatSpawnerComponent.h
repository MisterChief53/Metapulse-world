#include <AzCore/Component/Component.h>
#include <HttpRequestor/HttpTypes.h>
#include <Components/Interfaces/AIChatSpawnerBus.h>
#include <AzCore/Math/Uuid.h>
#include <LyShine/Bus/UiSpawnerBus.h>

namespace metapulseWorld {

	/*
	* Component that is in charge of managing the spawned messages inside the 
	* message list for a player's chat with the AI
	*/
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
		/*
		* Our implementation calls the UI spawnerbus and spawns the message, while adding it
		* to the spawnmap so that we can then retroactively re-parent the messages when they
		* are spawned.
		*/
		void SpawnMessage(AZStd::pair<size_t, AZStd::string> message) override;

		/*
		* This implementation destroys all the elements holding the messages and clears
		* the itemMap
		*/
		void ClearMessages() override;

		// uispawner override

		/*
		* When a message is spawned, we change the text held by the corresponding entry in the
		* itemMap, and reparent it to the list that will hold the messages.
		*/
		void OnEntitySpawned(const AzFramework::SliceInstantiationTicket& /*ticket*/, const AZ::EntityId& /*spawnedEntity*/) override;
	private:
		AZ::EntityId m_spawnerEntityId;
		AZ::EntityId m_messagesList;

		/*
		* Holds the items that have been instructed to be spawned, but haven't been reparented.
		* This is a pair with a u64, which is a unique identifier for the instantiationTicket
		* And the second element is another pair with the id of the message, and the message
		* content.
		*/
		AZStd::map<AZ::u64, AZStd::pair<size_t, AZStd::string>> m_spawnMap;
		/*
		* Holds the items that have been spawned, so that later whenever we decide to delete
		* all, we can access their EntityIds. This holds the entityId of the item (in this
		* class they are messages) and a pair with the message id and the string.
		*/
		AZStd::map<AZ::EntityId, AZStd::pair<size_t, AZStd::string>> m_itemMap;
	};
}