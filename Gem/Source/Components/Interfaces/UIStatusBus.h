#include <AzCore/EBus/EBus.h>
#include <AzCore/Component/ComponentBus.h>

namespace metapulseWorld {
	class UIStatusRequests
		: public AZ::EBusTraits {
	public:
		
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;

		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

		virtual bool canMove() = 0;
	};

	using UIStatusBus = AZ::EBus<UIStatusRequests>;
}