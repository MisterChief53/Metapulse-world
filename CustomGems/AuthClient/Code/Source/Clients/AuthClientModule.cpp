
#include <AuthClient/AuthClientTypeIds.h>
#include <AuthClientModuleInterface.h>
#include "AuthClientSystemComponent.h"

namespace AuthClient
{
    class AuthClientModule
        : public AuthClientModuleInterface
    {
    public:
        AZ_RTTI(AuthClientModule, AuthClientModuleTypeId, AuthClientModuleInterface);
        AZ_CLASS_ALLOCATOR(AuthClientModule, AZ::SystemAllocator);
    };
}// namespace AuthClient

AZ_DECLARE_MODULE_CLASS(Gem_AuthClient, AuthClient::AuthClientModule)
