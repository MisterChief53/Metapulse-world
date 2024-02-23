
#include <Metapulse-Auth-Client-gem/Metapulse-Auth-Client-gemTypeIds.h>
#include <Metapulse-Auth-Client-gemModuleInterface.h>
#include "Metapulse-Auth-Client-gemSystemComponent.h"

namespace Metapulse_Auth_Client_gem
{
    class Metapulse_Auth_Client_gemModule
        : public Metapulse_Auth_Client_gemModuleInterface
    {
    public:
        AZ_RTTI(Metapulse_Auth_Client_gemModule, Metapulse_Auth_Client_gemModuleTypeId, Metapulse_Auth_Client_gemModuleInterface);
        AZ_CLASS_ALLOCATOR(Metapulse_Auth_Client_gemModule, AZ::SystemAllocator);
    };
}// namespace Metapulse_Auth_Client_gem

AZ_DECLARE_MODULE_CLASS(Gem_Metapulse_Auth_Client_gem, Metapulse_Auth_Client_gem::Metapulse_Auth_Client_gemModule)
