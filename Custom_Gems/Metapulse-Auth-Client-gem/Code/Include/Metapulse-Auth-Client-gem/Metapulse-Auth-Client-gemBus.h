
#pragma once

#include <Metapulse-Auth-Client-gem/Metapulse-Auth-Client-gemTypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace Metapulse_Auth_Client_gem
{
    class Metapulse_Auth_Client_gemRequests
    {
    public:
        AZ_RTTI(Metapulse_Auth_Client_gemRequests, Metapulse_Auth_Client_gemRequestsTypeId);
        virtual ~Metapulse_Auth_Client_gemRequests() = default;
        // Put your public methods here
    };

    class Metapulse_Auth_Client_gemBusTraits
        : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using Metapulse_Auth_Client_gemRequestBus = AZ::EBus<Metapulse_Auth_Client_gemRequests, Metapulse_Auth_Client_gemBusTraits>;
    using Metapulse_Auth_Client_gemInterface = AZ::Interface<Metapulse_Auth_Client_gemRequests>;

} // namespace Metapulse_Auth_Client_gem
