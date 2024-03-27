#pragma once

#include <AzCore/EBus/EBus.h>

namespace metapulseWorld {
	class UIAdminRequests
		: public AZ::EBusTraits {
	public:
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;

		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

		virtual void UnloadStartMenu() = 0;

		virtual void LoadInventoryMenu() = 0;
	};

	using UIAdminBus = AZ::EBus<UIAdminRequests>;
}