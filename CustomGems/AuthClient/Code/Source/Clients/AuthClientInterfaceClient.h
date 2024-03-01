#pragma once

#include <AzCore/Component/Component.h>
#include <AuthClient/AuthClientHandlerInterface.h>
#include <AzCore/Interface/Interface.h>

namespace AuthClient {
	class AuthClientInterfaceClient
	{
	public:

		AZ_RTTI(AuthClientInterfaceClient, "{69AA6EEB-D6DF-4EC2-B749-3913D89263E3}");

		int requestToken();
	};
}