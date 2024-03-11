
#pragma once

#include <metapulseWorld/metapulseWorldTypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace metapulseWorld
{
    class metapulseWorldRequests
    {
    public:
        AZ_RTTI(metapulseWorldRequests, metapulseWorldRequestsTypeId);
        virtual ~metapulseWorldRequests() = default;
        // Put your public methods here
    };

    class metapulseWorldBusTraits
        : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using metapulseWorldRequestBus = AZ::EBus<metapulseWorldRequests, metapulseWorldBusTraits>;
    using metapulseWorldInterface = AZ::Interface<metapulseWorldRequests>;

} // namespace metapulseWorld
