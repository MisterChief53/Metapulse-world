#pragma once

#include <AzCore/Component/Component.h>
#include <AuthClient/AuthClientHandlerInterface.h>
#include <AzCore/Interface/Interface.h>

namespace AuthClient {
	class HandlerClient {
	public:
		int requestToken();
	};
}