#include <AuthClient/AuthClientHandlerInterface.h>
#include <AzCore/Interface/Interface.h>

namespace AuthClient {
	class AuthClientInterfaceImplementation
		: AuthClient::AuthClientHandlerRequests{
	public:
		AZ_RTTI(AuthClientInterfaceImplementation, "{EAD6EE7E-CBDE-48B4-AC0C-811196C880A0}", AuthClient::AuthClientHandlerRequests);

		AuthClientInterfaceImplementation() {
			AZ::Interface<AuthClient::AuthClientHandlerRequests>::Register(this);
		}

		virtual ~AuthClientInterfaceImplementation() {
			AZ::Interface<AuthClient::AuthClientHandlerRequests>::Unregister(this);
		}

		int getToken() override;
	};
}