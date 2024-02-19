
#pragma once

#include <Metapulse-world/Metapulse-worldTypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace Metapulse_world
{
    class Metapulse_worldRequests
    {
    public:
        AZ_RTTI(Metapulse_worldRequests, Metapulse_worldRequestsTypeId);
        virtual ~Metapulse_worldRequests() = default;
        // Put your public methods here
    };

    class Metapulse_worldBusTraits
        : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using Metapulse_worldRequestBus = AZ::EBus<Metapulse_worldRequests, Metapulse_worldBusTraits>;
    using Metapulse_worldInterface = AZ::Interface<Metapulse_worldRequests>;

} // namespace Metapulse_world
