#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzCore/Component/ComponentBus.h>

namespace metapulseWorld {

	/*
	* An interface to handle interaction with the start menu 
	* (where login or signup happens)
	*/
	class StartMenuRequests
		: public AZ::EBusTraits{
	public:
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;

		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

		/*
		* This closes the start menu.
		*/
		virtual void closeStartMenu() = 0;
	};

	using StartMenuBus = AZ::EBus<StartMenuRequests>;
}