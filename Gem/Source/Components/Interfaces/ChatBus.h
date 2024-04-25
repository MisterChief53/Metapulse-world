#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzCore/Component/EntityId.h>

namespace metapulseWorld {
	class ChatRequests
		: public AZ::EBusTraits {
	public:
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;

		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

		virtual AZ::EntityId GetActiveChat() = 0;

		virtual void SetActiveChat(AZ::EntityId newActiveChat) = 0;
	};

	using ChatBus = AZ::EBus<ChatRequests>;
}