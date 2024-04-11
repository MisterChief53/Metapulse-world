#include <AzCore/Component/Component.h>
#include <HttpRequestor/HttpTypes.h>
#include <Components/Interfaces/APIRequestsBus.h>
#include <LyShine/Bus/UiSpawnerBus.h>
#include <AzCore/Math/Uuid.h>

namespace metapulseWorld {
	class ChatBoxComponent
		: public AZ::Component
		, public UiSpawnerNotificationBus::Handler {
	public:
		AZ_COMPONENT(metapulseWorld::ChatBoxComponent, "{d5cac6c7-afbc-4dbf-b941-f75d22e276ba}", AZ::Component);

		// Component Overrides
		void Init() override;
		void Activate() override;
		void Deactivate() override;
		static void Reflect(AZ::ReflectContext* context);

		// uispawner override
		void OnEntitySpawned(const AzFramework::SliceInstantiationTicket& /*ticket*/, const AZ::EntityId& /*spawnedEntity*/) override;

		void FetchMessages();

	private:
		AZ::EntityId m_closeButtonEntityId;
		AZ::EntityId m_sendButtonEntityId;
		AZ::EntityId m_messageInputTextEntityId;

		AZ::EntityId m_messagesList;

		AZ::EntityId m_spawnerEntityId;

		AZStd::map<AZ::u64, AZStd::pair<size_t, AZStd::string>> m_spawnMap;
		AZStd::map<AZ::EntityId, AZStd::pair<size_t, AZStd::string>> m_itemMap;

		void RegisterSendButton();
	};
}