#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzCore/std/string/string.h>

namespace metapulseWorld {
	class UserMenuRequests
		: public AZ::EBusTraits {
	public:
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

		virtual void SendUsers(const AZStd::vector<AZStd::string> userVector) = 0;

		virtual AZStd::vector<AZStd::string> GetUsers() = 0;
	};

	using UserMenuBus = AZ::EBus<UserMenuRequests>;
}