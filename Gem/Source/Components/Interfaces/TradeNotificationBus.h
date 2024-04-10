#pragma once

#include <AzCore/EBus/EBus.h>
#include <Multiplayer/NetworkEntity/NetworkEntityHandle.h>

namespace metapulseWorld {
	class TradeNotificationRequests
		: public AZ::EBusTraits {
	public:
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;

		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

		virtual Multiplayer::NetworkEntityHandle GetHandleBus() = 0;
	};

	using TradeNotificationBus = AZ::EBus<TradeNotificationRequests>;
}