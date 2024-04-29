#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzCore/std/string/string.h>

namespace metapulseWorld {
	/*
	* This bus serves as the interface for communication with whomever is
	* handling item execution.
	*/
	class ItemRequests
		: public AZ::EBusTraits {
	public:
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;

		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

		/*
		* This tells the handler to "execute" the item with name itemName.
		*/
		virtual void executeItem(AZStd::string itemName) = 0;
	};

	using ItemBus = AZ::EBus<ItemRequests>;
}