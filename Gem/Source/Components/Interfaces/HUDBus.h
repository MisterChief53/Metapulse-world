#pragma once

#include <AzCore/EBus/EBus.h>

namespace metapulseWorld {
	class HUDRequests
		: public AZ::EBusTraits {
	public:
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;

		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

		virtual void DisplayMoney() = 0;
	};

	using HUDBus = AZ::EBus<HUDRequests>;
}