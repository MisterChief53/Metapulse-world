#include <AzCore/Component/Component.h>
#include <HttpRequestor/HttpTypes.h>
#include <Components/Interfaces/APIRequestsBus.h>
#include <LyShine/Bus/UiSpawnerBus.h>
#include <AzCore/Math/Uuid.h>
#include <AzCore/Component/TickBus.h>
#include <Components/Interfaces/ChatBus.h>

namespace metapulseWorld {
	class ChatBoxComponent
		: public AZ::Component
		, public UiSpawnerNotificationBus::Handler
		, public AZ::TickBus::Handler
		, public ChatBus::Handler {
	public:
		AZ_COMPONENT(metapulseWorld::ChatBoxComponent, "{d5cac6c7-afbc-4dbf-b941-f75d22e276ba}", AZ::Component);

		// Component Overrides
		void Init() override;
		void Activate() override;
		void Deactivate() override;
		static void Reflect(AZ::ReflectContext* context);

		// uispawner override

		/*
		* On the entity being spawned, we set the text to what is in the 
		* spawn map, and we register its entityId in the itemMap along with the
		* text we set. We then reparent it to the list (the message list) so that
		* it is displayed correctly.
		*/
		void OnEntitySpawned(const AzFramework::SliceInstantiationTicket& /*ticket*/, const AZ::EntityId& /*spawnedEntity*/) override;

		/*
		* Gets the user's credentials from the apirequestscomponent, fetches a list
		* of messages (ID and content), and calls the spawner bus to spawn them.
		* This works on both the AI chat and the friend chat, and calls
		* different spawner buses depending on which one we are fetching from.
		*/
		void FetchMessages();

		// Tick bus overrides

		/*
		* Every m_cooldown seconds, we clear the messages by destroying all the 
		* entities and clearing the itemMap.
		*/
		void OnTick(float deltaTime, AZ::ScriptTimePoint time) override;

		/*
		* We set the tickOrder to TICK_UI since we are dealing with UI operations
		*/
		int GetTickOrder() override;

		// Chat Bus Overrides
		/*
		* Returns the currect active chat, so that we know where to send messages
		*/
		AZ::EntityId GetActiveChat() override;

		/*
		* Changes the active chat to now be the entity that receives this function
		*/
		void SetActiveChat(AZ::EntityId newActiveChat) override;

	private:
		AZ::EntityId m_closeButtonEntityId;
		AZ::EntityId m_sendButtonEntityId;
		AZ::EntityId m_messageInputTextEntityId;
		AZ::EntityId m_messagesList;
		AZ::EntityId m_spawnerEntityId;
		AZ::EntityId m_aiChat;
		AZ::EntityId m_friendChat;
		AZ::EntityId m_friendButton;
		AZ::EntityId m_aiButton;
		AZ::EntityId m_aiMessagesList;
		AZ::EntityId m_sendAIButtonEntityId;
		AZ::EntityId m_aiMessageInputTextEntityId;

		AZ::EntityId m_activeChat;

		/*
		* The map that holds the messages that have been instructed to be spawned.
		* This has as a key the InstantiationTicket, which is represented as an
		* AZ::u64, and a value a pair holding the message id and the message
		* content.
		*/
		AZStd::map<AZ::u64, AZStd::pair<size_t, AZStd::string>> m_spawnMap;

		/*
		* This map holds the messages that have now been spawned. We store
		* their EntityId and the message information itself since we need
		* the IDs when we decide to destroy them before fetching them again.
		*/
		AZStd::map<AZ::EntityId, AZStd::pair<size_t, AZStd::string>> m_itemMap;

		/*
		* This function registers both send buttons, each concerning with
		* sending either to the friend who is connected, or the AI
		*/
		void RegisterSendButton();

		/*
		* This function registers a chat button, which is the button that decides
		* which chat is to be active.
		*/
		void RegisterChatButton(AZ::EntityId buttonEntityId, AZ::EntityId chatEntityId);

		/*
		* Holds the previous time, which is treated as a checkpoint to make the 
		* cooldown work.
		*/
		double m_prevTime = 0;

		/*
		* Decides how often to refresh the message list. This is a double representing
		* seconds.
		*/
		const double m_cooldown = 5;

		/*
		* This is the id of the chat in the Accounts server, used so know where
		* to send/fetch messages from
		*/
		AZStd::string m_chatId;
	};
}