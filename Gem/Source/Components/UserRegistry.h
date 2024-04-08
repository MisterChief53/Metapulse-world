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
        void UnregisterUser(const AZStd::string& entityId) override;

        AZStd::vector<AZStd::string> GetUserVector() override;

    protected:
        AZStd::unordered_map<AZStd::string, AZ::EntityId> m_userEntityIdMap;
    };
}
