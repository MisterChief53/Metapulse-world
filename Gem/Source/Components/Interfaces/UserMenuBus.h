#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzCore/std/string/string.h>

namespace metapulseWorld {

	/*
	* Interface to communicate with the user menu.
	*/
	class UserMenuRequests
		: public AZ::EBusTraits {
	public:
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

		/*
		* This tells the component that a button has been clicked. This has many uses,
		* like having the component perform certain actions only while the button has been
		* clicked.
		*/
		virtual void SetHasBeenClicked() = 0;

	};

	using UserMenuBus = AZ::EBus<UserMenuRequests>;
}