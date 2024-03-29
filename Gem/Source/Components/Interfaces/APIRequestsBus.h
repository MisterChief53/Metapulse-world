#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzCore/std/string/string.h>

namespace metapulseWorld {
	class APIRequestsInterface
		: public AZ::EBusTraits {
	public:
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;

		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

		virtual void setToken(const AZStd::string& token) = 0;
		virtual AZStd::string getToken() = 0;

		virtual void setUsername(const AZStd::string& username) = 0;
		virtual AZStd::string getUsername() = 0;

		virtual AZStd::string getUrl() = 0;
	};

	using APIRequestsBus = AZ::EBus<APIRequestsInterface>;
}