
#pragma once

#include <AuthClient/AuthClientTypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace AuthClient
{
    class AuthClientRequests
    {
    public:
        AZ_RTTI(AuthClientRequests, AuthClientRequestsTypeId);
        virtual ~AuthClientRequests() = default;
        // Put your public methods here
    };

    class AuthClientBusTraits
        : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using AuthClientRequestBus = AZ::EBus<AuthClientRequests, AuthClientBusTraits>;
    using AuthClientInterface = AZ::Interface<AuthClientRequests>;

} // namespace AuthClient
