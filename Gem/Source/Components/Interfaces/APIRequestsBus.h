#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzCore/std/string/string.h>

namespace metapulseWorld {
	class APIRequestsInterface
		: public AZ::EBusTraits {
	public:
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;

		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

		//virtual void login(AZStd::string & response, bool& succeed, AZStd::string& token, const AZStd::string& username, const AZStd::string& password) = 0;
	};

	using APIRequestsBus = AZ::EBus<APIRequestsInterface>;
}