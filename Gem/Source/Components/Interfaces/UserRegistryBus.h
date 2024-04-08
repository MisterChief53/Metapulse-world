#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzCore/Component/EntityId.h>

namespace metapulseWorld {
	class UserRegistryRequests
		: public AZ::EBusTraits {
	public:

		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;

		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

		virtual void RegisterUser(const AZ::EntityId& entityId) = 0;

		virtual void UnregisterUser(const AZStd::string& entityId) = 0;

		virtual AZStd::vector<AZStd::string> GetUserVector() = 0;
	};

	using UserRegistryBus = AZ::EBus<UserRegistryRequests>;
}