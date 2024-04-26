#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzCore/std/string/string.h>
#include <AzCore/std/utility/pair.h>

namespace metapulseWorld {
	class AIChatSpawnerInterface
		: public AZ::EBusTraits {
	public:
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;

		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

		virtual void SpawnMessage(AZStd::pair<size_t, AZStd::string> message) = 0;

		virtual void ClearMessages() = 0;

	};

	using AIChatSpawnerBus = AZ::EBus<AIChatSpawnerInterface>;
}