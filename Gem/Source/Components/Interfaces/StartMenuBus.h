#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzCore/Component/ComponentBus.h>

namespace metapulseWorld {
	class StartMenuRequests
		: public AZ::EBusTraits{
	public:
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;

		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

		virtual void closeStartMenu() = 0;
	};

	using StartMenuBus = AZ::EBus<StartMenuRequests>;
}