#pragma once

#include <AzCore/EBus/EBus.h>

namespace metapulseWorld {
	/*
	* The main interface to interact with the many menus that can be invoked by
	* the user directly.
	*/
	class UIAdminRequests
		: public AZ::EBusTraits {
	public:
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;

		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

		/*
		* When invoked, unloads the start menu if it is loaded.
		*/
		virtual void UnloadStartMenu() = 0;

		/*
		* Loads the inventory menu if it is unloaded.
		*/
		virtual void LoadInventoryMenu() = 0;
	};

	using UIAdminBus = AZ::EBus<UIAdminRequests>;
}