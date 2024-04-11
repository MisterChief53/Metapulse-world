#include <AzCore/Component/Component.h>
#include <HttpRequestor/HttpTypes.h>
#include <Components/Interfaces/APIRequestsBus.h>
#include <AzCore/Math/Uuid.h>

namespace metapulseWorld {
	class ChatBoxComponent
		: public AZ::Component {
	public:
		AZ_COMPONENT(metapulseWorld::ChatBoxComponent, "{d5cac6c7-afbc-4dbf-b941-f75d22e276ba}", AZ::Component);

		// Component Overrides
		void Init() override;
		void Activate() override;
		void Deactivate() override;
		static void Reflect(AZ::ReflectContext* context);

		void FetchMessages();

	private:
		AZ::EntityId m_closeButtonEntityId;
		AZ::EntityId m_sendButtonEntityId;
		AZ::EntityId m_messageInputTextEntityId;
	};
}