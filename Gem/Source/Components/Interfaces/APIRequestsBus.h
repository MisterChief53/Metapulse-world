#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzCore/std/string/string.h>

namespace metapulseWorld {
	/*
	* This is the interface (EBus) to communicate with our APIRequests component.
	* The component provides useful information to make it easier to make requests
	* to the accounts server.
	* 
	* There is only a single handler, so this is invoked using Broadcast()
	*/
	class APIRequestsInterface
		: public AZ::EBusTraits {
	public:
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;

		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

		/*
		* This sets the token used for subsequent requests.
		*/
		virtual void setToken(const AZStd::string& token) = 0;

		/*
		* Returns the token, so that we can use it for a request.
		*/
		virtual AZStd::string getToken() = 0;

		/*
		* Sets the user's username.
		*/
		virtual void setUsername(const AZStd::string& username) = 0;

		/*
		* Retrieves the logged in user's username.
		*/
		virtual AZStd::string getUsername() = 0;

		/*
		* Returns the URL (only with the domain) of the accounts server, so 
		* that the requests we make only need to know the enpoints, and if we
		* deploy the accounts server somewhere else, we just change a single reflection
		* on the API requests component.
		*/
		virtual AZStd::string getUrl() = 0;
	};

	using APIRequestsBus = AZ::EBus<APIRequestsInterface>;
}