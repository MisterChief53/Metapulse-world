#pragma once

#include <AzCore/EBus/EBus.h>
#include <Multiplayer/NetworkEntity/NetworkEntityHandle.h>

namespace metapulseWorld {
	/*
	* legacy interface to spawn a notification. Still kept in case in the future
	* we want to change the way notifications are handled.
	*/
	class TradeNotificationRequests
		: public AZ::EBusTraits {
	public:
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;

		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

		/*
		* Gets the handler of the component that implements this bus.
		*/
		virtual Multiplayer::NetworkEntityHandle GetHandleBus() = 0;
	};

	using TradeNotificationBus = AZ::EBus<TradeNotificationRequests>;
}