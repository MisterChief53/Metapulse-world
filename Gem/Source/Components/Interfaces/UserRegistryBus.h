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

		// I pray to the gods I am never forced to do something like this again.
		// Maybe generate our own struct container like the multiplayer sample does it?
		// for MVP purposes, right not it is not worth the effort. 
		virtual AZStd::string BusGetPlayer1() = 0;
		virtual AZStd::string BusGetPlayer2() = 0;
	};

	using UserRegistryBus = AZ::EBus<UserRegistryRequests>;
}