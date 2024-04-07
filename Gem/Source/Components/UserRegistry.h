#pragma once

#include <Source/AutoGen/UserRegistry.AutoComponent.h>
#include <Components/Interfaces/UserRegistryBus.h>

namespace metapulseWorld
{

    class UserRegistryController
        : public UserRegistryControllerBase
        , public metapulseWorld::UserRegistryBus::Handler
    {
    public:
        explicit UserRegistryController(UserRegistry& parent);

        void OnActivate(Multiplayer::EntityIsMigrating entityIsMigrating) override;
        void OnDeactivate(Multiplayer::EntityIsMigrating entityIsMigrating) override;

        // user registry overrides

        void RegisterUser(const AZ::EntityId& entityId) override;

    protected:

        AZStd::vector<AZ::EntityId> m_userEntityIdVector;
    };
}
